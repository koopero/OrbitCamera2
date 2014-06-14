var H = require('horten'),
	async = require('async'),
	dateformat = require('dateformat');


module.exports = function ( C, W ) {
	var controlPath = H.Path("/control/camera");
	var listener = H.listen( controlPath );

	H.listen( H.Path( controlPath, "shutter" ), function ( v ) {
		if ( v )
			start();
	} );

	H.listen( H.Path( controlPath, "auto" ), function ( v ) {
		if ( v ) {
			listener.set( 1, 'shutter' );
			start();


			var timer = listener.get( 'timer/value' ) || 1000;
			setTimeout( end, timer );


		} else {
			end();
		}
	} );




	var startTime,
		recordPath = C.record.path,
		recordFile = W.file( recordPath ),
		recordPathLocal = recordFile.localPath,
		maxTime = 60 * 1000,
		quant = 200,
		posterTime = 100,
		framesCopied,
		outputDir,
		frame;


	recordFile.mkdir();


	H.set( 200, '/camera/0/quant');
	H.set( recordPathLocal, '/camera/0/path');



	function start ( v ) {
		var now = new Date().getTime();
		frame = null;

		outputDir = W.file ( "/picture/"+timeCode()+"/" );

		quant = listener.get('quant/value') || 0;
		if ( quant < 200 )
			quant = 200;

		H.set( {
			enable: 1,
			path: recordPathLocal,
			quant: 200,
		}, '/camera/0' );


		startTime = quantTime( now, quant );
		framesCopied = 0;

		iterate();
		listener.set( 0, 'error');
	}



	function iterate () {
		var now = quantTime( new Date().getTime() );
		var t = now - startTime;

		var state = listener.get();

		frame = recordedFileAtTime( now, quant );

		

		var output = outputDir.file( 'frames/' +String( now ) + '.png' );

		//console.log ( "STORE", frame, output );

		output.store( frame, { link: true }, function ( err ) {
			if ( err ) {
				console.warn ( "FRAME COPY ERROR")
			} else {
				framesCopied++;
			}
		} );

		console.log( "frame", t, quant );

		if ( state.shutter && t < maxTime ) {
			setTimeout( iterate, quant );
		} else {
			end();
		}

	}

	function end () {
		var now = new Date().getTime();
		var t = now - startTime;

		listener.set( 0, 'shutter' );
		listener.set( 0, 'auto' );

		if ( !outputDir || !startTime || t < 400 || !framesCopied ) {
			listener.set( 1, 'error');
			return;
		}


		var sequenceFile = outputDir.file( 'sequence.json' )

		var sequence = {
			quant: 	quant,
			offset: quantTime( startTime, quant ),
			start: 	startTime,
			loop: 	quantTime( now - startTime, quant ),
			path: 	recordPathLocal
		};

		sequenceFile.storeData ( sequence );

		listener.set( sequence, '/lastPicture/' );


		var posterFrame = recordedFileAtTime( startTime + posterTime, quant );

		var poster = outputDir.file( 'poster.png' );
		poster.store( posterFrame );

		
		console.log( "poster.stream.name", poster.stream.name)
		poster.relatives ( { inputs: true, outputs: true, source: true, affected: true }, function ( err, rel ) {
			console.log( "poster.relatives()", err, rel )
		});
		//console.log( "poster.relative",poster.relative('sequence') )
		
		outputDir.file('frames/').touch();

	}


	function timeCode ( time ) {
		if ( isNaN( time ) )
			time = new Date().getTime();

		time = quantTime( time );
		time = new Date( time );

		return dateformat( time, "yymmddHHMMss" );
	}


	function quantTime ( time, quant ) {
		if ( isNaN( quant ) )
			quant = 100;

		if ( isNaN( time ) )
			time = Math.floor( new Date().getTime() );

		if ( quant ) {
			time = Math.floor( time / quant ) * quant;
		}

		return time;
	}

	function zeroPadTime ( time ) {
		time = String( time );
		while ( time.length < 13 )
			time = '0'+time;

		return time;
	}

	function recordedFileAtTime ( time, quant ) {
		time = quantTime ( time, quant );
		var filename = recordPath;
		filename = filename.replace ( "$t", zeroPadTime( time ) );

		return W.file( filename );
	}

}
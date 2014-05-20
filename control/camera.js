var H = require('horten'),
	async = require('async'),
	dateformat = require('dateformat');


module.exports = function ( C, W ) {
	var controlPath = H.Path("/control/camera");
	var listener = H.listen( controlPath );

	H.listen( H.Path( controlPath, "shutter" ), start );

	var startTime,
		iter,
		_quant = 100;

	function start ( v ) {
		if ( !v )
			return;

		if ( iter )
			return;

		startTime = new Date().getTime();
		iter = setInterval( iterate, 500 );
	}

	function iterate () {
		var now = new Date().getTime();
		var t = now - startTime;

		console.log( "Camera iter", t );

		if ( t > 3000 )
			end();
	}

	function end () {
		clearInterval( iter );
		
		iter = null;

		H.set( {
			start: startTime,
			loop: 3000
		}, "control/screen/input" );

		listener.set( 0, "shutter" );

		var toGif = C.camera.toGif.path.replace( "$t", timeCode( startTime ) );

		console.log( "TO GIF", toGif );

		linkGifSources( function (err) {
			console.log( "GIF Sources Linked" );
		} );

		function linkGifSources ( cb ) {
			var offsets = [ 0, 100, 200, 300, 400 ];
			var i = 0;

			async.mapSeries( offsets, function ( offset, cb ) {
				var png = recordedFileAtTime( startTime + offset );
				var destPath = toGif.replace ( '$i', String(i) );
				var dest = W.file( destPath );
				dest.store( png, { link: true }, cb );
				i++;
			}, cb );
		}

		function saveSpec ( cb ) {

		}

		function savePoster ( cb ) {

		}

		
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
			quant = _quant;

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
		var filename = C.camera.record.path;
		filename = filename.replace ( "$t", zeroPadTime( time ) );

		return W.file( filename );
	}


}
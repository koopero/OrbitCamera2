var H = require('horten');


module.exports = function ( C, W ) {
	var controlPath = H.Path("/control/camera");
	var listener = H.listen( controlPath );

	H.listen( H.Path( controlPath, "shutter" ), start );

	var startTime,
		iter;

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
	}


}
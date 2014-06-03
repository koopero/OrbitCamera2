
var async = require('async');

module.exports = function ( conf, W ) {

	var lastResult = {};
	var lastTime = 0;
	var running = false;

	function getPictures () {
		var t = new Date().getTime();

		if ( t - lastTime > 3000 && !running ) {
			running = true;
			var pictures = W.file('/picture/');


			pictures.readdir( function ( err, dir ) {
				if ( err ) {
					console.warn ( "ERROR READING /picture/")
					return;
				}

				var result = {};

				async.mapSeries( dir, function ( picture, cb ) {
					console.log ( "PCITURE", picture );

					var key = [ picture.substr( 6, 2 ), picture.substr( 8, 2 ), picture.substr( 10, 2 ) ].join(':');
					picture = pictures.file( picture );

					var seqFile = picture.file( 'sequence.json' );
					seqFile.readData ( function ( err, data ) {
						result[key] = data;
						cb( null );
					});

					
				}, function ( err ) {
					running = false;
					lastTime = new Date().getTime();
					lastResult = result;
				} );
			} );
		}

		return lastResult;
	} 

	return {
		all: getPictures
	}

}
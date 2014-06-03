var async = require('async');

module.exports = function ( app, conf, W ) {
	
	app.get( '/picture', function ( req, res ) {
		var pictures = W.file('/picture/');
		pictures.readdir( function ( err, dir ) {
			if ( err ) {
				res.send( 500, err );
				return;
			}

			async.mapSeries( dir, function ( picture, cb ) {
				picture = pictures.file( picture );

				var item = {};
				item.path = String( picture.path.stripTrailingSlash() );
				item.poster = '/files'+ picture.file( 'poster.gif' ).path.toString();


				var seqFile = picture.file( 'sequence.json' );
				seqFile.readData ( function ( err, data ) {
					item.sequence = data;
					cb( null, item );
				});

				
			}, function ( err, results ) {
				res.send( results );
			} );
		} );
	});


	app.delete( '/picture/:pictureId', function ( req, res ) {
		var picture = W.file( '/picture/' + req.params.pictureId+'/' )
		picture.unlink( function ( err, stat ) {
			res.send( 204 );
		} );

		
	});

}
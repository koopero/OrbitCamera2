var async = require('async');

module.exports = function ( app, conf, W ) {
	
	app.get( '/picture', function ( req, res ) {
		getPictures( function ( err, pictures ) {
			if ( err ) {
				res.send( 500, err );
			} else {
				res.send( pictures );
			}
		});
	});


	app.delete( '/picture/:pictureId', function ( req, res ) {
		var picture = W.file( '/picture/' + req.params.pictureId+'/' );
		var moveTo = W.file( '/waste/' + req.params.pictureId+'/' );

		moveTo.store( picture, function ( err, stat ) {
			if ( err ) {
				console.log( "MOVE ERR", err );
				res.send( 500 );
			} else {
				picture.unlink( function ( err ) {
					console.log( "UNLINK ERR", err );
					res.send( err ? 500 : 204 );
				});
			}
		} );

		/*
		picture.unlink( function ( err, stat ) {
			res.send( 204 );
		} );
		*/
	});


	app.set('view engine', 'jade');

	app.get('/manage', function ( req, res ) {
		getPictures( function ( err, pictures ) {
			console.warn ( "MANAGE", err, pictures );
			res.render( 'manage', {
				pictures: pictures
			})
		});

	})

	function getPictures ( cb ) {
		var pictures = W.file('/picture/');
		pictures.readdir( function ( err, dir ) {
			if ( err ) {
				return cb( err );
			}

			async.mapSeries( dir, function ( picture, cb ) {
				picture = pictures.file( picture );

				var item = {};
				item.path = String( picture.path.stripTrailingSlash() );
				item.thumbnail = '/files'+ picture.file( 'thumbnail.gif' ).path.toString();


				var seqFile = picture.file( 'sequence.json' );
				seqFile.readData ( function ( err, data ) {
					item.sequence = data;
					cb( null, item );
				});

				
			}, function ( err, results ) {
				cb( err, results );
			} );
		} );		
	}


}
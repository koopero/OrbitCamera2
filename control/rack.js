module.exports = function ( config, wildcat ) {

	var Rack = require('horten-rack'),
		Path = require('horten').Path;

	var screenPath = 'control/screen';

	for ( var i = 0; i < 3; i ++ ) {
		var layerPath = Path( screenPath, 'layer', i );
		Rack.Colour( Path( layerPath, 'back/colour' ) );
		Rack.Colour( Path( layerPath, 'front/colour' ) );
		new  Rack.Selector( {
			path: Path( layerPath, 'mesh/mode' ),
			options: {
				'Quads': 0,
				'Wire': 1,
				'Points': 2,
				'Scatter': 3
			}
		} );

	}

	var cameraPath = "/camera/0"

	new Rack.Selector( {
		path: Path( cameraPath, 'frameTime' ),
		options: {
			'Off': 0,
			'1 fps': 1000,
			'5 fps': 200,
			'20 fps': 50 
		}
	} )


}
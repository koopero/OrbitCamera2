

module.exports = function ( config, wildcat ) {

	var Rack = require('horten-rack'),
		H = require('horten'),
		Path = H.Path;

	new Rack.Coord( {
		path: '/lookAt'
	});


	var numScreens = 10;


	var Pictures = require('./pictures.js')( config, wildcat );

	for ( var i = 0; i < numScreens; i ++ ) {
		new Rack.Selector( {
			path: Path( 'control/input', i, 'picture' ),
			target: Path( '/screen', i, 'input' ),
			options: Pictures.all
		})
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


	var takeCamera = new Rack.Latch( {
		path: '/control/takeCamera',
		target: '/screen/'
	}).up( {
		input: {
			camera: 1
		}
	}).down( {
		input: {
			camera: 0
		}
	});

/*
	new Rack.Latch( {
		path: '/control/unloop',
		target: '/screen'
	}).up( {
		input: {
			loop: 0
		}
	})
*/

	new Rack.Latch( {
		path: '/control/recent',
		target: '/screen'
	}).up( function () {
		return {
			input: {
				offset: quantTime(),
				start: quantTime() - 1200,
				loop: 0,
				camera: 0,
				quant: 200
			}
		};
	})





	var screenPath = 'control/screen';
	var numLayers = 3;
	

	for ( var i = 0; i < numLayers; i ++ ) {
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

	H.listenPrimitive( '/control/screen/layer/all', function ( v, p ) {
		for ( var i = 0; i < numLayers; i ++ ) {
			var layerPath = Path( screenPath, 'layer', i );
			layerPath.set( v, p );
		}
	})


	new Rack.Selector( {
		path: Path( screenPath, 'mode' ),
		options: {
			'3d': 0,
			'Input': 1,
			'Square': 2,
			'Circle': 3
		}
	});


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


	var writeScreenPreset = Rack.Latch( {
		path: Path( 'control/writeScreenPreset' ),
		target: Path( 'preset/screen' )
	}).up('/control/screen');

	var readScreenPreset = Rack.Latch( {
		path: Path( 'control/readScreenPreset' ),
		target: Path( 'preset/screen' )
	}).up( function ( context ) {

		H.set( context.target.get(), '/control/screen' )
	});


	Rack.Latch( {
		path: Path( 'control/cycleScreenPreset' ),
		target: Path( '/screen/' )
	}).up( function ( context ) {
		context.presetId = parseInt( context.presetId ) || 0;
		context.presetId ++;
		context.presetId %= 4; 
		context.presetId = String( context.presetId );

		

		var preset = Path( '/preset/screen/' + context.presetId ).get();
		//console.warn ( "Screen Preset Id", context.presetId, preset );



		if ( preset.input ) {
			var inputFeedback = preset.input.feedback;
			delete preset.input;
			preset.input = {
				feedback: inputFeedback
			}
		}

		context.target.set( preset );
	});




	var lightControlPath = 'control/lights';
	for ( var i = 0; i < 3; i ++ ) {
		new Rack.Selector( {
			path: Path( lightControlPath, 'func',i,'func' ),
			options: {
				'Sine': 'sine',
				'Half Sine': 'halfsine',
				'Square': 'square',
				'Saw': 'saw',
				'Noise': 'noise'
			}
		})
	}


	var writeLightPreset = Rack.Latch( {
		path: Path( 'control/latch/writeLights' ),
		target: Path( 'preset/lights' )
	})
	writeLightPreset.devices.push( {
		up: '/control/lights'
	});

	var readLightPreset = Rack.Latch( {
		path: Path( 'control/latch/readLights' ),
		target: Path( 'preset/lights' )
	});

	readLightPreset.devices.push( {
		up: function ( context ) {
			H.set( H.get( context.target ), 'control/lights' );
		}
	});


	var cameraControlPath = "/control/camera"

	new Rack.Selector( {
		path: Path( cameraControlPath, 'maxTime' ),
		options: {
			'Off': 0,
			'1 s': 1000,
			'5 s': 5000,
			'30 s': 30000
		}
	} )


	new Rack.Selector( {
		path: Path( cameraControlPath, 'quant' ),
		options: {
			'0.2': 200,
			'0.4': 400,
			'0.8': 800,
			'1.0': 1000
		}
	} )


	new Rack.Selector( {
		path: Path( cameraControlPath, 'timer' ),
		options: {
			'0.6': 600,
			'1.0': 1000,
			'2.0': 2000,
			'5.0': 5000,
			'10.0': 10000
		}
	} )


	var openLights = Rack.Latch( {
		path: Path( '/control/latch/openLights' )
	})

	var closeLights = Rack.Latch( {
		path: Path( '/control/latch/closeLights' )
	})

	H.listen( '/control/camera/shutter', function ( v ) {
		if ( v ) {
			if ( openLights.selected != -1 ) {
				var lightData = H.get( Path( '/preset/lights', openLights.selected ) );
			}
		} else {
			if ( closeLights.selected != -1 ) {
				var lightData = H.get( Path( '/preset/lights', closeLights.selected ) );
			}
		}

		if ( lightData )
			H.set( lightData, '/control/lights' );

	});


	H.listen('/control/camera/lastFrame', function ( v ) {
		H.set( 0, "control/screen/input/camera" );
		H.set( v, "control/screen/input" );
	});


	//
	//	Last Picture
	//	

	var lastPicture = Rack.Latch( {
		path: Path( '/control/lastPicture' ),
		target: '/screen/'
	})

	H.listen( '/control/camera/lastPicture/', function ( v ) {
		lastPicture.setActiveTargets( 0, '/input/camera' );
		lastPicture.setActiveTargets( v, '/input/' );
	})

	//
	//	Screen Preview
	//

	var screenLatch = Rack.Latch( {
		path: Path( '/control/screenPreview' ),
		target: '/screen/'
	});

	H.listenPrimitive( '/control/screen', function ( v, p ) {
		screenLatch.setActiveTargets( v, p );
	});




	//
	//	Mapping
	//

	var controlsPath = '/control/';
	var cellPath = '/screen/';

	var cornerLatch = Rack.Latch( {
		path: controlsPath+'/latch/corner/',
	});
	cornerLatch.devices.push( {
		up: setPad
	})


	var mapLatch = Rack.Latch( {
		path: controlsPath+'/latch/mapping/',
		target: cellPath
	});


	mapLatch.devices.push( {
		up: setPad
	});

	H.listen( H.Path( controlsPath, 'coord' ), function ( value ) {
		if ( cornerLatch.selected == -1 || mapLatch.selected == -1 )
			return;

		var coord = {
			x: value[1],
			y: 1-value[0]
		}

		var cornerId = cornerLatch.selected;

		if ( cornerId >= 0 && cornerId < 4 ) {
			var path = H.Path( cellPath, mapLatch.selected, 'corner', cornerId );
			path.set( coord );
		} else if ( cornerId == 4 ) {
			var path = H.Path( cellPath, mapLatch.selected, 'centre' );
			path.set( coord );
		}
	});


	H.listen( H.Path( controlsPath, 'isCircle' ), function ( value ) {
		if ( mapLatch.selected == -1 )
			return;

		var path = H.Path( cellPath, mapLatch.selected, 'isCircle' );
		path.set( value );
		
	});



	function setPad() {
		//var cornerId = [ 3, 0, 2, 1][ cornerLatch.selected];
		//console.log( "PULL CORNER", cornerLatch.selected, "FROM", mapLatch.selected );
		//H.set( [ Math.random(), Math.random() ], controlsPath+'/coord/' );
	}

	H.listen( H.Path( controlsPath, 'resetMapping'), function ( value ) {
		if ( value ) {
			var cols = 5;
			var rows = 2;
			var xc = 1 / cols;
			var yc = 1 / rows;
			for ( var x = 0; x < cols; x ++ ) {
				for ( var y = 0; y < rows; y ++ ) {
					var c = y * cols + x;
					var d = [
						{ x: xc * x, y: yc * y },
						{ x: xc * x + xc, y: yc * y },
						{ x: xc * x + xc, y: yc * y + yc },
						{ x: xc * x, y: yc * y + yc }	
					];

					H.set( d, H.Path( cellPath, c, 'corner' ) );
				}
			}
		}
	})





}
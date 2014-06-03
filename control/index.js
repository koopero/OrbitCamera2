var H = require('horten'),
	Wildcat = require('wildcat');



var _ = require('underscore'),
	async = require('async'),
	pathlib = require('path'),
	urllib = require('url');

var conf = require('js-yaml').safeLoad( require('fs').readFileSync( pathlib.resolve( __dirname, '../config.yaml'), { encoding: 'utf8'} ) );

//console.log ( "conf", conf );

var W;


async.series( [
	initHorten,
	initWildcat,
	initPersistance,
	initServers,
	initExpress,
	initDevices
	
]);

function initHorten( cb ) {
	H.instance().debug = !!conf.horten.debug;
	cb();
}


function initPersistance( cb ) {
	var state = new (H.MySQL) ( conf.mysql );
	state.pull ( cb );
}


var HServer;
function initServers ( cb ) {
	var hss = H.HSS( conf.hss.port );


	var osc = H.OSC ( {
		port: conf.osc.incoming,
		clientPort: conf.osc.outgoing,
		treatAsArray: [ '/coord/' ]
	} );


	HServer = new (H.Server)( {
		url: conf.control.url
	});

	cb();	
}

function initWildcat( cb ) {
	Wildcat.open( pathlib.resolve( __dirname, "../wildcat.yaml" ), function ( err, router ) {
		if ( err ) throw err;

		W = router;
		cb();
	} );
}



function initExpress ( cb ) {
	var express = require('express');
	var app = express();
	var urlp = urllib.parse( conf.control.url );

	require('./api.js')( app, conf, W );

	app.use( '/horten/', HServer.middleware() );
	app.use( express.static( pathlib.resolve( __dirname, '../manager/' ) ) );

	var Wmiddleware = W.server.middleware();

	app.use( '/files', Wmiddleware.get );
	
	app.listen( conf.manager.port );

	var server = require('http').createServer( app );
	HServer.listenToUpgrade( server );

	server.listen( urlp.port );


	cb();
}



function initDevices ( cb ) {
	require('./rack.js')( conf, W );

	require('./camera.js')( conf.camera, W );





	cb();
}







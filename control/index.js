var H = require('horten'),
	Wildcat = require('wildcat');



var _ = require('underscore'),
	async = require('async'),
	pathlib = require('path');

var conf = require('js-yaml').safeLoad( require('fs').readFileSync( pathlib.resolve( __dirname, '../config.yaml'), { encoding: 'utf8'} ) );

console.log ( "conf", conf );

var W;


async.series( [
	initHorten,
	initPersistance,
	initServers,
	initWildcat
]);

function initHorten( cb ) {
	H.instance().debug = true;
	cb();
}


function initPersistance( cb ) {
	var state = new (H.MySQL) ( conf.mysql );
	state.pull ( cb );
}


function initServers ( cb ) {
	var hss = H.HSS( conf.hss.port );


	var osc = H.OSC ( {
		port: conf.osc.incoming,
		clientPort: conf.osc.outgoing,
		treatAsArray: [ '/coord/' ]
	} );


	var server = new (H.Server)( {
		url: 'http://panjandrum.local:8999/'
	});
	server.listenHttp();

	cb();	
}


function initWildcat( cb ) {
	Wildcat.open( pathlib.resolve( __dirname, "../wildcat.yaml" ), function ( err, router ) {
		if ( err ) throw err;

		W = router;

		var root = W.file('/');
		root.stat( function ( err, stat ) {
			console.warn ( "root", stat );
			cb();
		})
	} );
}




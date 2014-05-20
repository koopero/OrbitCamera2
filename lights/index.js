var H = require('horten');
H.instance().debug = true;

var config = require('js-yaml').safeLoad( require('fs').readFileSync( require('path').resolve( __dirname, '../config.yaml'), { encoding: 'utf8'} ) );

var controlUrl = config.control.url + config.lights.path;
console.log( "controlUrl", controlUrl );

var client = H.Client( controlUrl, {} );
client.pull();

H.listen( '/', function ( v, p ) {
	iterate( v );
});

function iterate( conf ) {
	console.log( 'iterate', conf );
}

/*
var SPI = require('spi');

var spi = new SPI.Spi('/dev/spidev0.0', {
	mode: SPI.MODE['MODE_0'],
	'chipSelect': SPI.CS['none']
}, function ( s ) { s.open(); } );

var numLeds = 56;
var gamma = 2.2;

var buff = new Buffer( numLeds * 3 + 4 );

for ( var i = 0; i < numLeds * 3; i += 3 ) {
	var r = Math.cos( i / 56 * 5 ) / 2 + 0.5;
	var g = 0;
	var b = 0;

	buff.writeUInt8( byteValue( g ), i );
	buff.writeUInt8( byteValue( r ), i + 1);
	buff.writeUInt8( byteValue( b ), i + 2);

	function byteValue ( f ) {
		f = f || 0;
		f = f < 0 ? 0 : f > 1 ? 1 : f;
		return 0x80 | parseInt( Math.pow(f,gamma) * 127 );
	}

}

for ( ; i < numLeds * 3 + 4; i ++ ) {
	buff.writeUInt8( 0, i );
}


spi.write( buff, function ( device, buf2 ) {
	console.log('Wrote');
} );
*/

/*
var leds = require("rpi-ws2801");
leds.connect( 56 );
leds.fill( 255, 0, 255 );
*/
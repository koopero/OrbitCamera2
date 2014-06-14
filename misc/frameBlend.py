import os
import re
import subprocess

quant = 10
extension = '.png'

def loadDir ( dir, mindigits ) :
	distill = {}

	for filename in os.listdir( dir ):
		if filename.endswith( extension ):
			basename = filename.replace( extension, '' )
			if len( basename ) >= mindigits :
				num = int( basename );
				distill[num] = [num];

	return distill

def distillerate ( data, quant ) :
	distill = {}
	for i, v in data.iteritems() :
		i = int( i )
		p = int( i / quant )

		if p not in distill :
			distill[p] = []

		distill[p].extend( v )

	return distill

def filenamefor( i ) :
	return str( i ) + extension

def render ( data ) :
	ret = data

	for i in data :
		v = data[i]
		if len( v ) > 1:
			destfile = filenamefor( i )
			inputs = map( filenamefor, v )

			cmd = ['convert'] + inputs + [ '-alpha', 'Off', '-evaluate-sequence', 'mean' ] + [destfile]
			print cmd
			subprocess.check_call( cmd )

			ret[i] = [i]

	return ret

def renderToGif ( data ) :
	ret = data

	for i in data :
		v = data[i]
		if len( v ) > 1:
			destfile = str(i) + '.gif'
			inputs = map( filenamefor, v )

			cmd = ['convert'] + inputs + [ ] + [destfile]
			print cmd
			subprocess.check_call( cmd )

			ret[i] = [i]

	return ret	

length = 13
while length > 0:
	data = loadDir( '.', length )
	if len( data ): 
		break
	length = length - 1

while True :
	data = distillerate( data, quant )
	#data = render( data )
	data = renderToGif( data )


	if len( data ) <= 1 :
		break









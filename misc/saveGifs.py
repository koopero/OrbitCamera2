import os
from shutil import copy2

destPath = 'progress/'

for path, dirs, files in os.walk('data') :
	
	for file in files:

		if file.endswith( '.gif' ) :
			filePath = os.path.join( path, file )
			time = int( os.path.getmtime( filePath ) )

			( base, dot, ext ) = file.rpartition( '.' )

			newPath = os.path.join( destPath, base + '.' + str(time) +'.'+ext )

			copy2( filePath, newPath )


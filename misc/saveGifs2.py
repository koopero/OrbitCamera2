
import os
import re
from shutil import copy2

import errno

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise


destDir = '../gif'
destDir = os.path.realpath( destDir )

print destDir

for path, dirs, files in os.walk('.') :
	for file in files:
		if file.endswith( '.gif' ) :
			filePath = os.path.join( path, file )

			match = re.match( r"\.\/(\d+)\/(.*?)\.gif", filePath )
			
			if match == None :
				continue

			timecode = match.group(1)
			preset = match.group(2)

			destPath = os.path.join( destDir, preset )

			mkdir_p ( destPath )

			destPath = os.path.join( destPath, timecode+'.gif' )

			copy2( filePath, destPath )



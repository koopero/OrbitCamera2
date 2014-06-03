//
//  FrameCache.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-08.
//
//

#include "FrameCache.h"

bool FrameCache::Frame::load( string url ) {
	
	if ( url.compare( 0, 6, "http:/" ) == 0 ) {
		try {
			frame = loadImage( loadUrl( url ) );
			loaded = true;
		} catch ( ImageIoException e ) {
			cerr << "Failed to load URL " << e.what() << endl;			
		} catch ( UrlLoadExc e ) {
			cerr << "Failed to load URL " << e.what() << endl;
		}
		
	} else {
		try {
			frame = loadImage( url );
			loaded = true;
		} catch ( ImageIoException e ) {
			error = true;
			cerr << "Failed to load frame " << url << endl;
		}
	}
	
	return loaded;
};

void FrameCache::cleanCache() {
	while ( cache.size() > maxSize ) {
		UInt64 oldest = 0;
		string del;
		
		for ( map<string,Frame>::iterator i = cache.begin(); i != cache.end(); ++i ) {
			if ( !del.size() || i->second.atime < oldest ) {
				del = i->first;
				oldest  =i->second.atime;
			}
		}
		
		cache.erase( del );
	}
}

Texture FrameCache::getFrame(const string &url) {
	Frame &frame = cache[url];
	
	if ( !frame.loaded && !frame.error ) {
		frame.load( url );
	}
	
	frame.atime = wallTimeMS();

	return frame.frame;
}
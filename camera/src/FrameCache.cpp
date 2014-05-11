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
			cerr << "Failed to load " << e.what() << endl;
		}
	}
	
	return loaded;
};

Texture FrameCache::getFrame(const string &url) {
	Frame &frame = cache[url];
	
	if ( !frame.loaded ) {
		frame.load( url );
	}
	
	return frame.frame;
}
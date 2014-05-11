//
//  FrameCache.h
//  camera
//
//  Created by Samm Cooper on 2014-05-08.
//
//

#ifndef __camera__FrameCache__
#define __camera__FrameCache__

#include <iostream>
#include <string>

#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::gl;
using namespace std;



class FrameCache {
public:
	Texture getFrame( const string & url );
	
	void cleanCache();
	
protected:
	class Frame {
	public:
		Texture frame;
		bool loaded = false;
		bool load( string url );
	};
	
	map<string,Frame> cache;
};

#endif /* defined(__camera__FrameCache__) */

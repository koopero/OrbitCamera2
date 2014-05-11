//
//  Camera.h
//  camera
//
//  Created by Samm Cooper on 2014-05-04.
//
//

#ifndef __camera__Camera__
#define __camera__Camera__

#include <iostream>
#include <string>

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include "CinderFreenect.h"

#include "horten/Horten.h"
#include "horten/Listener.h"

#include "TimeRanger.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace Horten;
using namespace std::chrono;

class OrbitCamera {
public:
	void		setPath( string path );
	void		setup();
	void		update();
	void		draw();
	void		save();
	Texture		getTexture();
protected:
	KinectRef	kinect;
	Fbo			buffer;
	GlslProg	sRToAlpha;
	Listener	listener;
	
	TimeRanger	time;
	
	Texture		colourTexture, depthTexture;
	
	string		lastFilename;
};

#endif /* defined(__camera__Camera__) */

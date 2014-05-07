//
//  Feedback.h
//  camera
//
//  Created by Samm Cooper on 2014-03-28.
//
//

#ifndef __camera__Feedback__
#define __camera__Feedback__

#include <iostream>

#include "horten/Listener.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include "HortenShader.h"

using namespace ci;
using namespace cinder::gl;
using namespace Horten;

class Feedback {
public:
	void setPath( string path );
	void setup ( int width = 640, int height = 480 );
	void clear ();
	void add ( Texture const &tex );
	void feedback ();
	void bind();
	void unbind();
	
	Texture getTexture();
	
protected:
	int curBuffer = 0;
	Listener listener;
	Fbo buffer[2];
	HortenShader feedbackShader;
};

#endif /* defined(__camera__Feedback__) */

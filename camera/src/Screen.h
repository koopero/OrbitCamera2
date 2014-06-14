//
//  Screen.h
//  camera
//
//  Created by Samm Cooper on 2014-05-03.
//
//

#ifndef __camera__Screen__
#define __camera__Screen__


#include <string>
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/Timer.h"
#include "CinderOpenCV.h"

#include "Layer.h"
#include "Feedback.h"
#include "OrbitCamera.h"
#include "FrameCache.h"
#include "TimeRanger.h"

using namespace std;
using namespace ci;


class Screen {
public:
	void setPath( string path );
	void setup( int numLayers );
	
	void takeInput( OrbitCamera * camera, FrameCache * frames );
	void update();
	void render();
	void draw();
	
	void addInput( gl::TextureRef input );
	
	int screenWidth = 1280;
	int screenHeight = 960;
	
	
	Vec2f rotation;
	
protected:
	vector<Layer> layers;
	Feedback inputBuffer;
	Feedback outputBuffer;
	Listener listener;
	Listener global;
	TimeRanger inputFile;
	
	Perlin::Perlin noise;
	Timer::Timer timer;
	
	VboMesh mesh;
	
	Area bounds = Area( 0, 0, 480, 480) ;
	
		
	void setupMesh();
	
	GlslProg inputAlignShader;
	
	Vec2f corner[4];
};

#endif /* defined(__camera__Screen__) */

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
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "Layer.h"
#include "Feedback.h"

using namespace std;
using namespace ci;


class Screen {
public:
	void setPath( string path );
	void setup( int numLayers );
	
	void setInput( Texture input );
	void update();
	void render();
	void draw();
	
	void addInput( gl::TextureRef input );
	
	int screenWidth = 1280;
	int screenHeight = 960;
	
protected:
	vector<Layer> layers;
	Feedback inputBuffer;
	Feedback outputBuffer;
	Listener listener;
	
	VboMesh mesh;
	
	void setupMesh();
	
	Vec2f corner[4];
};

#endif /* defined(__camera__Screen__) */
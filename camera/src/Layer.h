//
//  Layer.h
//  camera
//
//  Created by Samm Cooper on 2014-05-03.
//
//

#ifndef __camera__Layer__
#define __camera__Layer__

#include <string>
#include <iostream>
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "Layer.h"
#include "Feedback.h"
#include "HortenShader.h"
#include "horten/Listener.h"

#include <iostream>

using namespace std;
using namespace ci;
using namespace Horten;


class Layer {
public:
	void setPath( string path );
	void setup();
	void update();
	void updateMesh ();
	void draw( gl::Texture input );

protected:
	void meshGrid ( int mode, int width, int height, int depth = 1 );
	
	float _meshDetail	= 0;
	int _meshMode = -1;
	
	Area bounds = Area( 0, 0, 480, 480) ;
	Area inputBounds;
	
	Listener listener;
	HortenShader shader;
	VboMesh mesh;
};

#endif /* defined(__camera__Layer__) */

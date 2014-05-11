//
//  HeightField.h
//  camera
//
//  Created by Samm Cooper on 2014-03-28.
//
//

#ifndef __camera__HeightField__
#define __camera__HeightField__

#include <iostream>
#include <vector>
#include "Resources.h"

#include "cinder/app/App.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"

#include "horten/Listener.h"

using namespace cinder;
using namespace std;
using namespace Horten;

class HeightField {
public:
	void update ();
	void updateMesh ();
	void updateShader ();
	void draw ();
	
	Listener listener;
	
	
protected:
	void meshGrid ( int width, int height, int depth = 1 );
	
	float _meshDetail	= 0;
	
	gl::GlslProg		shader;
	gl::VboMeshRef		mesh;
};

#endif /* defined(__camera__HeightField__) */

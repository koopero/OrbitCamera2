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
#include "cinder/gl/Fbo.h"
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
	void draw( gl::Texture input );

protected:
	Listener listener;
	HortenShader shader;
};

#endif /* defined(__camera__Layer__) */

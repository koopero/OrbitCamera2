//
//  PNGWriter.h
//  camera
//
//  Created by Samm Cooper on 2014-03-25.
//
//

#ifndef __camera__PNGWriter__
#define __camera__PNGWriter__

#include <iostream>
#include <string>

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace std;
using namespace ci;



class PNGWrite {
public:
	gl::TextureRef texture;
	string filename;
};

class PNGWriter {
public:

private:
	vector<PNGWrite> queue;
	void writeNext ();
	
};

#endif /* defined(__camera__PNGWriter__) */

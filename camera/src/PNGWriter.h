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





class PNGWriter {
public:

	void save( gl::Texture tx, string filename );
	
private:
	class PNGWrite {
	public:
		gl::Texture texture;
		string filename;
	};
	
	vector<PNGWrite> queue;
	void writeThread ();
	
};

#endif /* defined(__camera__PNGWriter__) */

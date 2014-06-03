//
//  PNGWriter.cpp
//  camera
//
//  Created by Samm Cooper on 2014-03-25.
//
//

#include "PNGWriter.h"

void PNGWriter::save( gl::Texture tex, string filename ) {
	PNGWrite write;
	write.texture = tex;
	write.filename = filename;
	
	queue.push_back( write );
}

void PNGWriter::writeThread() {
	
}
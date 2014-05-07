//
//  Layer.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-03.
//
//

#include "Layer.h"

void Layer::setPath(string path) {
	listener.path = path;
}

void Layer::setup() {
	shader.loadGlslResources( "heightField.vert.glsl", "heightField.frag.glsl" );
}

void Layer::update() {

}

void Layer::draw( Texture input ) {
	shader.bind();
	gl::draw( input );
	shader.unbind();
}
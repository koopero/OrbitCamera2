//
//  Feedback.cpp
//  camera
//
//  Created by Samm Cooper on 2014-03-28.
//
//

#include "Feedback.h"

void Feedback::setPath(string path) {
	listener.path = path;
	feedbackShader.setPath( path );
}

void Feedback::setup( int width, int height ) {
	
	feedbackShader.loadGlslResources( "passthru.vert.glsl", "feedback.frag.glsl" );
	feedbackShader.addVar( ShaderVar( "multColour", Vec4f( 0.8, 0.8, 0.8, 0.8 ) ) );
	feedbackShader.addVar( ShaderVar( "blurAmount", "blur/amount", 0 ) );

	
	Fbo::Format format;
	format.setColorInternalFormat( GL_RGBA32F_ARB );
	format.enableColorBuffer();
	
	for ( int i = 0; i < 2; i ++ ) {
		buffer[i] = Fbo( width, height, format );
	}
	
	clear();
}

void Feedback::clear() {
	for ( int i = 0; i < 2; i ++ ) {
		buffer[i].bindFramebuffer();
		gl::clear( ColorAf( 1,0,1,0.2) );
		buffer[i].unbindFramebuffer();
	}
}

void Feedback::add(const cinder::gl::Texture &tex) {
	int write = curBuffer;
	buffer[write].bindFramebuffer();
	
	
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
	glBlendEquation(GL_FUNC_ADD);
	glBlendColor(0.8, 0.1, 0.9, 0.20);
	
	
	//glDisable(GL_BLEND);
	 
	gl::draw( tex );
	
	buffer[write].unbindFramebuffer();
}

void Feedback::feedback() {
	//return;
	
	int write = curBuffer ^ 1;
	int read = curBuffer ^ 0;
	
	Texture tex = buffer[read].getTexture();
	buffer[write].bindFramebuffer();
	
	//color( 0,0,0,0 );
	//clear();
	
	glDisable(GL_BLEND);
	
	color( 1,1,1,1);
	
	
	feedbackShader.bind();
	draw( tex );
	feedbackShader.unbind();
	
	
	//drawSolidCircle( Vec2f(0,0), 100 );
	
	buffer[write].unbindFramebuffer();
	
	//std::cout << "CurBuffer" << curBuffer << std::endl;
	
	curBuffer = curBuffer ^ 1;
}

void Feedback::bind() {
	int write = curBuffer ^ 0;
	buffer[write].bindFramebuffer();
}

void Feedback::unbind() {
	int write = curBuffer ^ 0;
	buffer[write].unbindFramebuffer();
}


Texture Feedback::getTexture() {
	return buffer[curBuffer ^ 1].getTexture();
}
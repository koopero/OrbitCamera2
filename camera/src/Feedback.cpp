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
	feedbackShader.addVar( ShaderVar( "multColour", Vec4f( 0.8, 0.2, 0.2, 0.2 ) ) );
	feedbackShader.addVar( ShaderVar( "blurAmount", "blur/amount", 0 ) );

	_size = Vec2i( width, height );
	
	Fbo::Format format;
	format.setColorInternalFormat( GL_RGBA32F_ARB );
	format.enableColorBuffer();
	
	for ( int i = 0; i < 2; i ++ ) {
		buffer[i] = Fbo( width, height, format );
	}
	
	clear();
}

Vec2i Feedback::getSize() {
	return _size;
}

void Feedback::clear() {
	for ( int i = 0; i < 2; i ++ ) {
		buffer[i].bindFramebuffer();
		gl::clear( ColorAf( 0,0,0,0) );
		buffer[i].unbindFramebuffer();
	}
}

void Feedback::add(const cinder::gl::Texture &tex) {
	int write = curBuffer;
	buffer[write].bindFramebuffer();
	
	Vec4f mix = listener.getVec4f( "/mix/", Vec4f( 0.5,0.5,0.5,0.5 ) );
	
	float mixPow = 2.5;
	
	mix.x = pow( mix.x,  mixPow );
	mix.y = pow( mix.y,  mixPow );
	mix.z = pow( mix.z,  mixPow );
	mix.w = pow( mix.w,  mixPow );
	
	
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
	glBlendEquation(GL_FUNC_ADD);
	glBlendColor( mix.x, mix.y, mix.z, mix.w );
	
	
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
	
	color( 0,1,0,1 );
	gl::clear();
	
	
	/*
	color( 1,1,1,1 );
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_ONE, GL_ZERO);
	glBlendEquation(GL_FUNC_ADD);
	feedbackShader.bind();
	draw( tex );
	feedbackShader.unbind();
	*/
	
	//drawSolidCircle( Vec2f(0,0), 100 );
	
	buffer[write].unbindFramebuffer();
	
	//std::cout << "CurBuffer" << curBuffer << std::endl;
	
	curBuffer = write;
}

void Feedback::bind() {
	
	
	
	int write = curBuffer ^ 0;
	buffer[write].bindFramebuffer();
	
	gl::pushMatrices();
	//gl::setMatricesWindowPersp( buffer[write].getSize(), 90, -1, 3, true );
	gl::setMatricesWindow( buffer[write].getSize(), false );
	gl::setViewport( Area( Vec2i( 0,0 ), _size ) );
	
	
}

void Feedback::unbind() {
	gl::popMatrices();
	
	int write = curBuffer ^ 0;
	buffer[write].unbindFramebuffer();
}


Texture Feedback::getTexture() {
	return buffer[curBuffer ^ 0].getTexture();
}
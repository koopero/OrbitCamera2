//
//  Camera.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-04.
//
//

#include "OrbitCamera.h"

using namespace ci;


void OrbitCamera::setPath( string path ) {
	listener.path = path;
	time.setPath( path );
}

void OrbitCamera::setup() {
	try {
		sRToAlpha = gl::GlslProg( ci::app::loadResource( "passthru.vert.glsl"  ), ci::app::loadResource( "rToAlpha.frag.glsl" ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	
	gl::Fbo::Format format;
	format.enableColorBuffer();
	
	buffer = gl::Fbo( 640, 480, format );
}

void OrbitCamera::update() {
	
	if ( !kinect ) {
		try {
			kinect = Kinect::create();
		} catch ( Kinect::ExcFailedOpenDevice e ) {
			std::cout << "Couldn't open kinect" << std::endl;
			return;
		}
	}
	
	time.update();
	
	
	kinect->setTilt( listener.getDouble( "/tilt",0 ) );
	
	if ( kinect->checkNewDepthFrame() )
		depthTexture = kinect->getDepthImage();
	
	if ( kinect->checkNewVideoFrame())
		colourTexture = kinect->getVideoImage();
	
	
	if ( !depthTexture || !colourTexture )
		return;
	
	buffer.bindFramebuffer();
	setViewport( buffer.getBounds() );
	setMatricesWindow( 640, 480 );
	Matrix44f flip = Matrix44f::identity();
	flip.scale( Vec2f( 1,-1 ) );
	flip.translate(Vec3f( 0,-480,0));
	
	glEnable( GL_ALPHA );
	gl::multModelView( flip );
	gl::clear( ColorAf( 0,0,0,0));
	
	float alignScale = listener.getDouble( "/align/s", 0.8 );
	Matrix44f align = Matrix44f::identity();
	align.scale( alignScale );
	
	gl::pushModelView();
	glMatrixMode( GL_MODELVIEW );
	gl::translate( listener.getDouble("/align/x",0 ), listener.getDouble("/align/y",0 ) );
	gl::scale( Vec2f( alignScale, alignScale ) );
	//gl::multProjection( align );
	gl::multModelView( align );
	
	
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ZERO, GL_ZERO, GL_ONE, GL_ZERO);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	
	sRToAlpha.bind();
	sRToAlpha.uniform("tex0", 0 );
	gl::draw( depthTexture, depthTexture.getBounds() );
	sRToAlpha.unbind();
	
	gl::popModelView();
	
	
	
	flip.scale( Vec2f( 1,-1) );
	flip.translate( Vec3f( 0,-480,0));
	
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	gl::draw( colourTexture, depthTexture.getBounds() );
	
	buffer.unbindFramebuffer();
}

void OrbitCamera::draw () {
	gl::color(1,1,1,1);
	
	
	glEnable(GL_BLEND);
	glEnable( GL_ALPHA );
	
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	
	if ( buffer ) {
		gl::draw( buffer.getTexture(), buffer.getBounds() );
	}
}

Texture OrbitCamera::getTexture() {
	return buffer.getTexture();
}

void OrbitCamera::save () {
	
	//if ( !listener.getBool( "/record/enable" ))
	//	return;
	

	
	
	
	string filename = time.getFileName();
	
	if ( filename.length() < 1 ) {
		return;
	}
	
	if ( filename == lastFilename ) {
		return;
	}
	
	std::cout << "Filename " << filename << std::endl;
	
	ImageTarget::Options options;
	//try {
	writeImage( filename, buffer.getTexture(), options );
	//} catch( ImageIoException e ) {
	//	cerr << e.what() << endl;
	//}
	
	lastFilename  = filename;
}


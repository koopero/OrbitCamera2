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
	
	buffer = gl::Fbo( 480, 480, format );
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
	
	
	
	//return;
	
	kinect->setTilt( listener.getDouble( "/tilt",0 ) );
	
	if ( kinect->checkNewDepthFrame() )
		depthTexture = kinect->getDepthImage();
	
	if ( kinect->checkNewVideoFrame())
		colourTexture = kinect->getVideoImage();
	
	
	if ( !depthTexture || depthTexture.getWidth() != 640 || !colourTexture || colourTexture.getWidth() != 640 )
		return;
	
	buffer.bindFramebuffer();
	
	setViewport( buffer.getBounds() );
	setMatricesWindow( buffer.getSize() );
	
	Matrix44f flip = Matrix44f::identity();
	flip.scale( Vec2f( 1,-1 ) );
	flip.translate(Vec3f( 0,-480,0));
	
	glEnable( GL_ALPHA );
	glEnable( GL_LINEAR );
	
	gl::multModelView( flip );
	
	gl::clear( ColorAf( 0,0,0,0));
	
	
	
	gl::pushModelView();
	float alignScale = listener.getDouble( "/align/depth/s", 0.8 );
	gl::translate( listener.getDouble("/align/depth/x",0 ) - 80.0, listener.getDouble("/align/depth/y",0 ) );
	gl::scale( Vec2f( alignScale, alignScale ) );
	//gl::multProjection( align );

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ZERO, GL_ZERO, GL_ONE, GL_ZERO);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	
	sRToAlpha.bind();
	sRToAlpha.uniform("tex0", 0 );
	gl::draw( depthTexture, depthTexture.getBounds() );
	sRToAlpha.unbind();
	
	gl::popModelView();
	
	
	gl::pushModelView();
	alignScale = listener.getDouble( "/align/colour/s", 0.8 );
	gl::translate( listener.getDouble("/align/colour/x",0 ) - 80.0, listener.getDouble("/align/colour/y",0 ) );
	gl::scale( Vec2f( alignScale, alignScale ) );
	
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ZERO, GL_ONE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	gl::draw( colourTexture, depthTexture.getBounds() );
	
	gl::popModelView();
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
	
	if ( !buffer ) {
		return;
	}
	
	time.update();
	
	//return;
	
	string filename = time.getFileName();
	//string filename = "/Volumes/hdd/koop/Code/Orbit2/data/camera/00.png";
	//std::cout << "Filename " << filename << std::endl;
	
	//return;
	
	if ( filename.length() < 1 ) {
		return;
	}
	
	if ( filename == lastFilename ) {
		return;
	}
	
	//return;
	
	ImageTarget::Options options;
	//try {
	writeImage( filename, buffer.getTexture(), options );
	//} catch( ImageIoException e ) {
	//	cerr << e.what() << endl;
	//}
	
	lastFilename = filename;
}


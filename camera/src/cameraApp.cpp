#include "Resources.h"

#include <sstream>
#include <ctime>
#include <ratio>
#include <chrono>

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"



#include "HeightField.h"
#include "OrbitCamera.h"
#include "Screen.h"


#include "Horten.h"
#include "horten/Socket.h"
#include "horten/Listener.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cameraApp : public AppBasic {
  public:
	void prepareSettings( Settings* settings );
	void initHorten ();
	void setup();
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
	void update();
	void draw();
	
	void saveBuffer();
	
	Channel32f filterKinectDepth ( const gl::Texture &kinectData );
	
	Socket	socket;
	Listener listener;
	
	OrbitCamera		camera;
	FrameCache		frames;

	gl::Fbo			mBuffer;
	gl::GlslProg	mShader;
	
	int numScreens = 1;
	int numLayers = 3;

	vector<Screen>	screens;
	
private:

};

void cameraApp::prepareSettings( Settings* settings )
{
	settings->setWindowSize( 1024, 1024 );
}

void cameraApp::initHorten () {
	socket.port = "8765";
    socket.hostname = "127.0.0.1";
    socket.start();
	
}

void cameraApp::setup()
{
	initHorten();
	
	camera.setPath("/camera/0");
	camera.setup();

	try {
		mShader = gl::GlslProg( loadResource( "passthru.vert.glsl" ), loadResource( "kinectFilter.frag.glsl" ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	
	screens.resize( numScreens );
	
	for ( int i = 0; i < numScreens; i ++ ) {
		std::ostringstream path;
		//path << "/screen/" << i << "/";
		path << "control/screen/";
		
		screens[i].setPath( path.str() );
		screens[i].setup( numLayers );
	}
	

}

void cameraApp::mouseDown( MouseEvent event )
{
}

void cameraApp::keyDown( KeyEvent event) {
	if ( event.getChar() == 'p' ) {
		camera.save();
	}
}



void cameraApp::update()
{
	camera.update();
	camera.save();
	
	for ( vector<Screen>::iterator screen = screens.begin(); screen != screens.end(); ++screen )
	{
		screen->takeInput( &camera, &frames );
		screen->update();
	}

	
//	/node.update();
}


void cameraApp::draw()
{
	for ( vector<Screen>::iterator screen = screens.begin(); screen != screens.end(); ++screen )
	{
		screen->render();
		//screen->draw();
	}
	
	gl::clear( ColorAf( 0,0,0.2,0));
	
	gl::setViewport( getWindowBounds() );
	gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
	
	/*
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_BLEND );
	
	color( 1,1,1,1);
	drawSolidCircle( Vec2f( 0,0), 100 );
	*/

	

	
	//camera.draw();

	
	
	for ( vector<Screen>::iterator screen = screens.begin(); screen != screens.end(); ++screen ) {
		//screen->render();
		screen->draw();
	}
	

	//node.draw();
	//gl::popMatrices();

}


CINDER_APP_BASIC( cameraApp, RendererGl )

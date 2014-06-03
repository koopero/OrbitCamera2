//
//  Screen.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-03.
//
//

#include "Screen.h"

void Screen::setPath( string path ) {
	listener.path = path;
	
	for ( int i = 0; i < layers.size(); i ++ ) {
		Layer & layer = layers[i];
		stringstream layerPath;
		layerPath << path << "layer/" << i << "/";
		layer.setPath( layerPath.str() );
	}
	
	inputBuffer.setPath( path + "input/feedback/" );
	outputBuffer.setPath( path + "output/feedback/" );
	
	inputFile.setPath( path +"/input" );
}

void Screen::setup( int numLayers ) {
	
	try {
		inputAlignShader = gl::GlslProg( ci::app::loadResource( "passthru.vert.glsl" ), ci::app::loadResource( "inputAlign.frag.glsl" ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		std::cout << "inputAlignShader compile error: " << std::endl;
		std::cout << exc.what();
	}
	
	layers.resize( numLayers );
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		layer->setup( );
	}
	
	
	corner[0] = Vec2f( 0.3,0.3 );
	corner[1] = Vec2f( 1,0 );
	corner[2] = Vec2f( 1,1 );
	corner[3] = Vec2f( 0,1 );
	
	
	setPath( listener.path );
	
	inputBuffer.setup();
	
	outputBuffer.setup( bounds.getWidth(), bounds.getHeight() );
	
	setupMesh();
}

void Screen::takeInput(OrbitCamera *camera, FrameCache *frames) {
	if ( listener.getBool( "/input/camera" ) ) {
		inputBuffer.add( camera->getTexture() );
	} else {
		string url = inputFile.getFileName();
		
		if ( url.length() ) {
			
			//cout << "Taking frame " << url << endl;
			
			Texture tex = frames->getFrame( url );
			if ( tex ) {
				inputBuffer.add( tex );
			}
		}
	}
	
}

void Screen::update() {
	
	inputFile.update();
	
	//inputBuffer.feedback();
	
	Texture input = inputBuffer.getTexture();
	
	
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		//layer->setInput( input );
		layer->update();
	}
	
	
	char cornerPath[1000];
	
    for ( int i = 0; i < 4; i ++ ) {
        sprintf( cornerPath, "/corner/%d/x", i );
        corner[i].x = listener.getDouble(cornerPath, i == 1 || i == 2 ? 1 : 0 );
        sprintf( cornerPath, "/corner/%d/y", i );
        corner[i].y = listener.getDouble(cornerPath, i == 2 || i == 3 ? 1 : 0 );
    }
	
	
	Vec3f lookAt = global.getVec3f( "/lookAt/", Vec3f( 0,0,0 ) );
	Vec3f centre = listener.getVec3f( "/centre/", Vec3f( 0.5,0.5,0 ) );

	lookAt -= Vec3f( 0.5, 0.5, 0 );
	lookAt *= 2.5;
	lookAt.z = -1;
	
	Vec3f offset = centre - lookAt;
	
	cout << "Offset " << offset << endl;
	 
	Vec3f rot = Vec3f(
					  ci::toPolar( offset.yz() ).y / pi * 180 - 90,
					  ci::toPolar( offset.xz() ).y / pi * 180 - 90,
					  0
	);
	
	cout << "Rot " << rot << endl;
	
	rotation = rot.xy();
	
}

void Screen::render() {
	Texture input = inputBuffer.getTexture();
	
	gl::pushMatrices();
	
	outputBuffer.feedback();
	outputBuffer.bind();
	
	//gl::color( 0,0,0,0 );
	//gl::clear();
	
	int mode = (int) listener.getDouble("mode", 2 );
	
	Vec2i outputBounds = outputBuffer.getSize();
	
	
	gl::setMatricesWindowPersp( outputBuffer.getSize(), 50, -1, 3, true );
	
	switch ( mode ) {
		case 1:
			glDisable(GL_ALPHA);
			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE);
			glBlendFunc(GL_ONE, GL_ONE );
			glBlendEquation(GL_FUNC_ADD);
			gl::color( 1, 1, 1, 1 );
			inputAlignShader.bind();
			input.enableAndBind();
			gl::translate( 0, bounds.getHeight() );
			gl::scale( 1, -1 );
			gl::drawSolidRect( bounds );
			input.unbind();
			inputAlignShader.unbind();
			
		break;
			
		case 2:
		case 3:
			glEnable(GL_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE);
			glBlendFunc(GL_ONE, GL_ONE );
			glBlendEquation(GL_FUNC_ADD);
			
			glLineWidth( 16 );
			gl::color( 1, 1, 1, 1 );
			glBlendColor( 1,1,1,1 );
			
			if ( mode == 2 ) {
				gl::drawLine( Vec2i( bounds.getX1(), bounds.getY1() ), Vec2i( bounds.getX1(), bounds.getY2() ) );
				gl::drawLine( Vec2i( bounds.getX2(), bounds.getY1() ), Vec2i( bounds.getX2(), bounds.getY2() ) );
				
				gl::drawLine( Vec2i( bounds.getX1(), bounds.getY1() ), Vec2i( bounds.getX2(), bounds.getY1() ) );
				gl::drawLine( Vec2i( bounds.getX1(), bounds.getY2() ), Vec2i( bounds.getX2(), bounds.getY2() ) );
				
				gl::drawLine( Vec2i( bounds.getX1(), bounds.getY1() ), Vec2i( bounds.getX2(), bounds.getY2() ) );
				gl::drawLine( Vec2i( bounds.getX2(), bounds.getY1() ), Vec2i( bounds.getX1(), bounds.getY2() ) );
			} else {
				gl::drawLine( Vec2i( bounds.getCenter().x, bounds.getY1() ), Vec2i( bounds.getCenter().x, bounds.getY2() ) );
				gl::drawLine( Vec2i( bounds.getX1(), bounds.getCenter().y ), Vec2i( bounds.getX2(), bounds.getCenter().y ) );
				gl::drawStrokedCircle( bounds.getCenter(), bounds.getWidth() / 2 );
			}
			
			gl::drawLine( bounds.getCenter(), Vec2i( bounds.getCenter().x, bounds.getY2() ) );
			
		break;
			
		case 0:
		default:
			glEnable(GL_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE);
			glBlendFunc(GL_ONE, GL_ONE );
			glBlendEquation(GL_FUNC_ADD);
			gl::color( 1,1,1,1 );
			
			//gl::setMatricesWindow( 480, 480 );
			gl::translate( bounds.getCenter() );
			float scale = bounds.getWidth() / 2;
			gl::rotate( Vec3f( rotation.x,rotation.y,0 ) );
			gl::scale( scale, -scale, 100 );
			
			//
			
			for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer ) {
				layer->draw( input );
			}
			
			
			
		break;
	}
	
	outputBuffer.unbind();
	gl::popMatrices();
}

void Screen::draw() {
	
	

	
	cv::Point2f cornerSource[4];
	cornerSource[0] = cv::Point2f( bounds.getX1(), bounds.getY1() );
	cornerSource[1] = cv::Point2f( bounds.getX2(), bounds.getY1() );
	cornerSource[2] = cv::Point2f( bounds.getX2(), bounds.getY2() );
	cornerSource[3] = cv::Point2f( bounds.getX1(), bounds.getY2() );
	
	double w = screenWidth;
	double h = screenHeight;
	
	cv::Point2f cornerDest[4];
	cornerDest[0] = cv::Point2f( corner[0].x * w, corner[0].y * h );
	cornerDest[1] = cv::Point2f( corner[1].x * w, corner[1].y * h );
	cornerDest[2] = cv::Point2f( corner[2].x * w, corner[2].y * h );
	cornerDest[3] = cv::Point2f( corner[3].x * w, corner[3].y * h );

	cv::Mat warp = cv::getPerspectiveTransform( cornerSource, cornerDest );
	
	Matrix44f transform;
	transform[0] = warp.ptr<double>(0)[0];
	transform[4] = warp.ptr<double>(0)[1];
	transform[12] = warp.ptr<double>(0)[2];

	transform[1] = warp.ptr<double>(1)[0];
	transform[5] = warp.ptr<double>(1)[1];
	transform[13] = warp.ptr<double>(1)[2];

	transform[3] = warp.ptr<double>(2)[0];
	transform[7] = warp.ptr<double>(2)[1];
	transform[15] = warp.ptr<double>(2)[2];

	
	gl::pushModelView();
	gl::multModelView( transform );
	
	
	Texture buffer = outputBuffer.getTexture();
	Texture input = inputBuffer.getTexture();
	
	glBlendColor(1, 1, 1, 1);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE );
	glBlendEquation(GL_FUNC_ADD);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	gl::color( 1,1,1,1);
	//gl::draw( buffer );
	
	buffer.enableAndBind();
	gl::drawSolidRect( bounds );
	//gl::drawSolidCircle( bounds.getCenter(), bounds.getWidth() / 2 );
	
	buffer.unbind();
	
	//input.enableAndBind();
	//gl::draw( mesh );
	//input.unbind();
	
	
	
	//glBlendFunc(GL_ONE, GL_ONE );
	//glBlendEquation(GL_FUNC_ADD);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//gl::draw( buffer, Vec2f( 0, 0 ) );
	
	gl::popModelView();

}

void Screen::setupMesh() {
	gl::VboMesh::Layout layout;
    layout.setStaticIndices();
    layout.setStaticPositions();
    layout.setStaticTexCoords2d();
    
    mesh = gl::VboMesh(4, 4, layout, GL_QUADS );
    
    vector<uint32_t> indices;
    vector<Vec2f> texCoords;
	vector<Vec3f> positions;
    
    indices.push_back( 0 );
    indices.push_back( 1 );
    indices.push_back( 3 );
    indices.push_back( 2 );
    
    texCoords.push_back( Vec2f( 0, 0 ) );
    texCoords.push_back( Vec2f( 1, 0 ) );
    texCoords.push_back( Vec2f( 0, 1 ) );
    texCoords.push_back( Vec2f( 1, 1 ) );
	
	positions.push_back( (Vec3f) Vec2f( 0,0 ) );
	positions.push_back( Vec3f( 1024,0,0 ) );
	positions.push_back( Vec3f( 0,1024,0 ) );
	positions.push_back( Vec3f( 1024,1024,0 ) );
	
    
    mesh.bufferIndices( indices );
    mesh.bufferTexCoords2d(0, texCoords );
	mesh.bufferPositions( positions );
	
	mesh.unbindBuffers();
}
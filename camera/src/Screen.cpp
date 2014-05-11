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
	
	inputBuffer.setPath( path + "/input/feedback/" );
	
	inputFile.setPath( path +"/input" );
}

void Screen::setup( int numLayers ) {
	layers.resize( numLayers );
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		layer->setup( );
	}
	
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
			
			cout << "Taking frame " << url << endl;
			
			Texture tex = frames->getFrame( url );
			if ( tex ) {
				inputBuffer.add( tex );
			}
		}
	}
	
}

void Screen::update() {
	
	inputFile.update();
	
	inputBuffer.feedback();
	
	Texture input = inputBuffer.getTexture();
	
	
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		//layer->setInput( input );
		layer->update();
	}
	
	
	char cornerPath[1000];
 
    for ( int i = 0; i < 4; i ++ ) {
        sprintf( cornerPath, "/corner/%d/x", i );
        corner[i].x = listener.getDouble(cornerPath, 0 );
        sprintf( cornerPath, "/corner/%d/y", i );
        corner[i].y = listener.getDouble(cornerPath, 0 );
    }
	
	
}

void Screen::render() {
	Texture input = inputBuffer.getTexture();
	
	//outputBuffer.feedback();
	gl::pushMatrices();
	
	//outputBuffer.add( input );
	outputBuffer.feedback();
	outputBuffer.bind();
	
	
	Vec3f rotate = listener.getVec3f( "rotate" );
	
	
	gl::color( 0,0,0,0 );
	gl::clear();
	
	gl::setMatricesWindowPersp( outputBuffer.getSize(), 90, -1, 3, true );
	
	//gl::setMatricesWindow( 480, 480 );
	gl::translate( bounds.getCenter() );
	float scale = bounds.getWidth() / 2;
	gl::rotate( Vec3f( rotate.x,rotate.y,0 ) );
	gl::scale( scale, -scale, 100 );
	
	//
	
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer ) {
		layer->draw( input );
	}
	
	//gl::draw( input );
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE );
	glBlendEquation(GL_FUNC_ADD);
	
	
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//gl::color( 0,1,1,1 );
	//drawSolidCircle( Vec2f( 0, 30 ), 600 );
	
	outputBuffer.unbind();
	gl::popMatrices();
}

void Screen::draw() {
	
	Texture buffer = outputBuffer.getTexture();
	Texture input = inputBuffer.getTexture();
	
	glBlendColor(1, 1, 1, 1);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE );
	glBlendEquation(GL_FUNC_ADD);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	color( 1,1,1,1);
	//gl::draw( input );
	
	buffer.enableAndBind();
	gl::draw( mesh );
	buffer.unbind();
	
	//gl::draw( buffer );
	
	//input.enableAndBind();
	//gl::draw( mesh );
	//input.unbind();
	
	
	
	//glBlendFunc(GL_ONE, GL_ONE );
	//glBlendEquation(GL_FUNC_ADD);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//gl::draw( buffer, Vec2f( 0, 0 ) );
	

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
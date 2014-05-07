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
}

void Screen::setup( int numLayers ) {
	layers.resize( numLayers );
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		layer->setup( );
	}
	
	setPath( listener.path );
	
	inputBuffer.setup();
	
	outputBuffer.setup( 480, 480 );
	
	setupMesh();
}

void Screen::setInput( Texture input ) {
	inputBuffer.add( input );
}

void Screen::update() {
	
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
	//outputBuffer.feedback();
	outputBuffer.bind();
	gl::clear();
	
	gl::setMatricesWindow( 480, 480 );
	gl::translate( 0, 480 );
	gl::scale( 1, -1 );
	
	//gl::color( 1,1,0,1 );
	
	for ( vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer ) {
		layer->draw( input );
	}
	
	//gl::draw( input );
	gl::enable( GL_ALPHA );
	gl::color( 0,1,0,1 );
	drawSolidCircle( Vec2f( 0, 30 ), 60 );
	
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

	color( 1,1,1,1);
	gl::draw( input );
	
	//gl::draw( buffer, Vec2f( 0, 480 ) );
	//buffer.enableAndBind();
	//gl::draw( mesh );
	//buffer.unbind();
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
	
	positions.push_back( Vec3f( 0,0,0 ) );
	positions.push_back( Vec3f( 1280,0,0 ) );
	positions.push_back( Vec3f( 0,960,0 ) );
	positions.push_back( Vec3f( 1080,760,0 ) );
	
    
    mesh.bufferIndices( indices );
    mesh.bufferTexCoords2d(0, texCoords );
	mesh.bufferPositions( positions );
	
	mesh.unbindBuffers();
}
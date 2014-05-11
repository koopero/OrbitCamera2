//
//  HeightField.cpp
//  camera
//
//  Created by Samm Cooper on 2014-03-28.
//
//

#include "HeightField.h"

void HeightField::update () {
	
	if ( !mesh ) {
		//meshGrid( 16, 12, 1 );
		//meshGrid( 16, 12, 1 );
	}
	updateMesh();
	updateShader();
}



void HeightField::updateShader () {
	if ( !shader ) {
		try {
			shader = gl::GlslProg( app::loadResource( "heightField.vert.glsl" ), app::loadResource( "heightField.frag.glsl" ) );
		} catch ( gl::GlslProgCompileExc &exc ) {
			std::cout << "Shader compile error: " << std::endl;
			std::cout << exc.what();
		}
	}
}

void HeightField::draw() {
	if ( !shader )
		return;
	
	shader.uniform( "image", 0 );
	
	shader.uniform( "zMin", (float)listener.getDouble( "zMin", 0 ) );
	shader.uniform( "zMax", (float)listener.getDouble( "zMin", 1 ) );
	shader.uniform( "frontScale", (float)1.5 );
	shader.uniform( "backScale", (float)1.0 );
	shader.uniform( "backPos", listener.getVec3f( "backPos" ) );
	shader.uniform( "frontPos", listener.getVec3f( "frontPos" ) );
	
	
	
	shader.uniform( "colourDetail",(float) listener.getDouble( "colourDetail", 0.5 ) );
	glPointSize( 600 );
	
	if ( mesh ) {
		glEnable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glBlendEquation(GL_FUNC_ADD);
		
		gl::draw( mesh );
	}
	
	shader.unbind();
}

void HeightField::updateMesh () {
	
	float meshDetail = listener.getDouble("/meshDetail", 1.0 );
	meshDetail = meshDetail > 1.0 ? 1.0 : meshDetail < 0.0 ? 0.0 : meshDetail;
	
	if ( meshDetail == _meshDetail )
		return;
	
	
	int rows = roundf( powf( meshDetail, 2.0 ) * 240 ) + 6;
	int cols = roundf( powf( meshDetail, 2.0 ) * 180 ) + 6;
	
	_meshDetail = meshDetail;
	
	meshGrid( rows, cols, 1 );
}

void HeightField::meshGrid( int width, int height, int depth ) {
	bool quad = true;
	
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticTexCoords3d();
	
	int totalVertices = width * height * depth;
	int totalQuads = ( width - 1 ) * ( height - 1 ) * depth;
	mesh = gl::VboMesh::create( totalVertices, totalQuads * 4, layout, GL_POINTS );
	
	// buffer our static data - the texcoords and the indices
	vector<uint32_t> indices;
	vector<Vec3f> texCoords;
	vector<Vec3f> positions;
	
	
	for ( int z = 0; z < depth; ++z ) {
		for( int y = 0; y < height; ++y ) {
			for( int x = 0; x < width; ++x ) {
				int i = z * width * height + y * width + x;
				// create a quad for each vertex, except for along the bottom and right edges
				if( quad && ( x + 1 < width ) && ( y + 1 < height ) ) {
					indices.push_back( i );
					indices.push_back( i + 1 );
					indices.push_back( i + width + 1 );
					indices.push_back( i + width );
				}
				
				Vec3f index = Vec3f( x, y, z );
				Vec3f tex = index / Vec3f( width - 1, height - 1, 1 );
				Vec3f pos = (index / Vec3f( width - 1, height - 1, 1 ) - Vec3f( 0.5, 0.5, 0 ) ) * Vec3f( 2, 2, 1 );

				texCoords.push_back( tex );
				positions.push_back( pos );
			}
		}
	}
	
	mesh->bufferIndices( indices );
	mesh->bufferTexCoords3d( 0, texCoords );
	mesh->bufferPositions( positions );
	

	
}
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
	shader.setPath( path );
}

void Layer::setup() {
	shader.loadGlslResources( "heightField.vert.glsl", "heightField.frag.glsl" );
	
	
	shader.addVar( ShaderVar( "colour0", "back/colour", Vec4f( 1,1,1,1 ) ) );
	shader.addVar( ShaderVar( "colour1", "front/colour", Vec4f( 1,1,1,1 ) ) );
	shader.addVar( ShaderVar( "depth0", "back/depth", 0 ) );
	shader.addVar( ShaderVar( "depth1", "front/depth", 1 ) );

	shader.addVar( ShaderVar( "perspZMult", 8 ) );
	shader.addVar( ShaderVar( "perspZPow", 1.1 ) );
	shader.addVar( ShaderVar( "perspZAdd", 0.8 ) );
	
	shader.addVar( ShaderVar( "frustZMult", 1 ) );
	shader.addVar( ShaderVar( "frustZAdd", 0.6 ) );

	shader.addVar( ShaderVar( "fishAdd", 0.18 ) );
	shader.addVar( ShaderVar( "fishDiv", 1.3 ) );
	
	shader.addVar( ShaderVar( "colourDetail", 1 ) );
	
	//shader.addVar( ShaderVar( "))
}

void Layer::update() {
	updateMesh();
}

void Layer::draw( Texture input ) {
	
	if ( !listener.getBool( "enable" ) )
		return;
	
	int mode = (int) listener.getDouble("mesh/mode/value", 0 );
	
	inputBounds = input.getBounds();
	
	shader.bind();

	//cout << "ModelView" << endl << getModelView() << endl;
	//cout << "Projection" << endl << getProjection() << endl;
	
	color( 1,1,1,1 );
	glBlendColor( 1, 1, 1, 1 );
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE );
	glBlendEquation(GL_FUNC_ADD);

	
	float pointSize = pow( 3, listener.getDouble( "pointSize", 0 ) * 3 );
	
	glPointSize( pointSize );
	glLineWidth( pointSize  );
	
	switch ( mode ) {
		case 1:
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		break;
			
		case 2:  // Points
			glEnable( GL_POINT_SPRITE );

		break;
			
		default:
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		break;
	}
	
	//gl::draw( input );
	input.enableAndBind();
	gl::draw( mesh );
	input.unbind();
	//drawSolidCircle(Vec2f(240,240), 240 );
	shader.unbind();
	
	
	
	
}


void Layer::updateMesh () {
	
	float meshDetail = listener.getDouble("/mesh/density", 0.1 );
	meshDetail = meshDetail > 1.0 ? 1.0 : meshDetail < 0.0 ? 0.0 : meshDetail;
	
	int mode = (int) listener.getDouble("mesh/mode/value", 0 );
	
	if ( meshDetail == _meshDetail && _meshMode == mode )
		return;
	
	

	
	int rows = roundf( powf( meshDetail, 2.0 ) * 240 ) + 6;
	int cols = roundf( powf( meshDetail, 2.0 ) * 180 ) + 6;
	
	_meshMode = mode;
	_meshDetail = meshDetail;
	
	meshGrid( mode, rows, cols, 1 );
}

void Layer::meshGrid( int mode, int width, int height, int depth ) {
	bool quad = false;
	
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticTexCoords3d();
	
	
	int totalVertices = width * height * depth;

	switch ( mode ) {
		case 2: // Points
			mesh = VboMesh( totalVertices, totalVertices, layout, GL_POINTS );
		break;
		default:
			quad = true;
			int totalQuads;
			totalQuads = ( width - 1 ) * ( height - 1 ) * depth;
			mesh = VboMesh( totalVertices, totalQuads * 4, layout, GL_QUADS );
		break;
	}

	
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
				} else if ( !quad ) {
					indices.push_back( i );
				}
				
				Vec3f index = Vec3f( x, y, z );
				Vec3f tex = index / Vec3f( width - 1, height - 1, 1 );
				Vec3f pos = index / Vec3f( width - 1, height - 1, 1 );
				
				pos -= Vec3f( 0.5, 0.5, 0 );
				pos *= 2;
				
				texCoords.push_back( tex );
				positions.push_back( pos );
			}
		}
	}
	
	mesh.bufferIndices( indices );
	mesh.bufferTexCoords3d( 0, texCoords );
	mesh.bufferPositions( positions );
	mesh.unbindBuffers();
}
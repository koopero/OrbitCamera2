//
//  HortenShader.cpp
//  camera
//
//  Created by Samm Cooper on 2014-04-04.
//
//

#include "HortenShader.h"

ShaderVar::ShaderVar( const ShaderVar &copy ) {
	type = copy.type;
	path = copy.path;
	name = copy.name;
	
	switch ( type ) {
		case Type::Float:
			defaultFloat = copy.defaultFloat;
			break;
		case Type::Vec3:
			defaultVec3f = copy.defaultVec3f;
			break;
		case Type::Vec4:
			defaultVec4f = copy.defaultVec4f;
			break;
	}
};


ShaderVar::ShaderVar( const char * name, float defaultValue ) {
	type = Type::Float;
	ShaderVar::name = string( name );
	ShaderVar::path = string( name );
	defaultFloat = defaultValue;
}

ShaderVar::ShaderVar( const char * name, const char * path, float defaultValue ) {
	type = Type::Float;
	ShaderVar::name = string( name );
	ShaderVar::path = string( path );
	defaultFloat = defaultValue;
}

ShaderVar::ShaderVar( string name, string path, float defaultValue ) {
	type = Type::Float;
	ShaderVar::name = name;
	ShaderVar::path = path;
	defaultFloat = defaultValue;
}

ShaderVar::ShaderVar( const char * name, Vec4f defaultValue ) {
	type = Type::Vec4;
	ShaderVar::name = string( name );
	ShaderVar::path = string( name );
	defaultVec4f = defaultValue;
}

ShaderVar::ShaderVar( const char * name, const char * path, Vec4f defaultValue ) {
	type = Type::Vec4;
	ShaderVar::name = string( name );
	ShaderVar::path = string( path );
	defaultVec4f = defaultValue;
}

ShaderVar::ShaderVar( string name, string path, Vec4f defaultValue ) {
	type = Type::Vec4;
	ShaderVar::name = name;
	ShaderVar::path = path;
	defaultVec4f = defaultValue;
}

void ShaderVar::uniform(cinder::gl::GlslProg &shader, Listener &listener) {
	Value v = listener.get( path );
	
	switch ( type ) {
		case Type::Float: {
			float fVal;
			if ( v.isNumeric() ) {
				fVal = v.asFloat();
			} else {
				fVal = defaultFloat;
			}
			shader.uniform( name, fVal );
			
			//cout << listener.path << path << " " << fVal << endl;
			
		}
		break;
			
		case Type::Vec3: {
			shader.uniform( name, jsonToVec3f( v, defaultVec3f ) );
		}
		break;
			
		case Type::Vec4: {
			Vec4f v4Val = defaultVec4f;
			
			if ( v.isNumeric() ) {
				float fVal = v.asFloat();
				v4Val = Vec4f( fVal, fVal, fVal, fVal );
			} else if ( v.isObject() ) {
				if ( v["x"].isNumeric() ) {
					v4Val.x = v["x"].asFloat();
				} else if ( v["r"].isNumeric() ) {
					v4Val.x = v["r"].asFloat();
				}
				
				if ( v["y"].isNumeric() ) {
					v4Val.y = v["y"].asFloat();
				} else if ( v["g"].isNumeric() ) {
					v4Val.y = v["g"].asFloat();
				}
				
				if ( v["z"].isNumeric() ) {
					v4Val.z = v["z"].asFloat();
				} else if ( v["b"].isNumeric() ) {
					v4Val.z = v["b"].asFloat();
				}
				
				if ( v["w"].isNumeric() ) {
					v4Val.w = v["w"].asFloat();
				} else if ( v["a"].isNumeric() ) {
					v4Val.w = v["a"].asFloat();
				}
			}
			
			//cout << "feedback multColour " << v4Val << endl;
			
			shader.uniform( name, v4Val );
		}
		break;
	}
}

void HortenShader::setPath(string path) {
	//cout << "HortenShader Path " << path << endl;
	listener.path = path;
}

void HortenShader::loadGlslResources(const char *vert, const char *frag) {
	try {
		shader = gl::GlslProg::create( ci::app::loadResource( vert ), ci::app::loadResource( frag ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
}

void HortenShader::addVar( ShaderVar var) {
	vars.push_back( var );
}

void HortenShader::bind() {
	if ( !shader )
		return;
	
	shader->bind();
	for ( vector<ShaderVar>::iterator var = vars.begin(); var != vars.end(); ++var ) {
		var->uniform( *shader, listener );
	}
}

void HortenShader::unbind() {
	if ( !shader )
		return;
	
	shader->unbind();
}


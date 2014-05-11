//
//  HortenShader.h
//  camera
//
//  Created by Samm Cooper on 2014-04-04.
//
//

#ifndef __camera__HortenShader__
#define __camera__HortenShader__

#include <iostream>
#include <vector>
#include "Resources.h"

#include "cinder/app/App.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"

#include "horten/Listener.h"
#include "JsonToCinder.h"

using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace Horten;

class ShaderVar {
public:
	ShaderVar( const ShaderVar &copy );
	ShaderVar( const char * name, float defaultValue );
	ShaderVar( const char * name, const char * path, float defaultValue );
	ShaderVar( string name, string path, float defaultValue );
	
	ShaderVar( string name, string path, Vec4f defaultValue );
	ShaderVar( const char * name, Vec4f defaultValue );
	ShaderVar( const char * name, const char * path, Vec4f defaultValue );
	
	enum class Type {
		Float,
		Vec3,
		Vec4
	};
	

	Type type;
	string path;
	string name;
	
	void uniform ( GlslProg &shader, Listener &listener );
	
protected:
	friend class HortenShader;

	union {
		float defaultFloat;
		Vec3f defaultVec3f;
		Vec4f defaultVec4f;
	};
};

class HortenShader {
public:
	void setPath( string path );
	void loadGlslResources( const char *vert, const char *frag  );
	
	void addVar( ShaderVar var );
	
	void bind();
	void unbind();
	
protected:
	GlslProgRef shader;
	Listener listener;
	vector<ShaderVar> vars;
};




#endif /* defined(__camera__HortenShader__) */

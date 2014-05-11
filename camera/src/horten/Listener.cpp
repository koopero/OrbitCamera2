//
//  Listener.cpp
//  Horten
//
//  Created by Samm Cooper on 2013-09-15.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#include "Listener.h"

namespace Horten {
	Listener::Listener () {
		horten = Horten::Horten::instance();
	};
	
	Json::Value Listener::get() {
		return horten->get( path );
	};
	
	Json::Value Listener::get( const char *path ) {
		return horten->get( appendPath( string ( path ) ) );
	}
	
	Json::Value Listener::get( const string &path ) {
		return horten->get( appendPath( path ) );
	};;
	
    double Listener::getDouble () {
        return horten->getDouble( path );
    };
	
	double Listener::getDouble ( const string &path, double defaultValue ) {
		return horten->getDouble( appendPath( path ), defaultValue );
	}
	
	double Listener::getDouble ( const char * path, double defaultValue ) {
		return horten->getDouble( appendPath( string ( path ) ), defaultValue );
	}
	
	std::string Listener::getString ( const std::string &path, const std::string &defaultValue ) {
		Json::Value value = get( path );
		if ( !value.isString() ) {
			return defaultValue;
		}
		
		return value.asString();
	}
	


	Vec3f Listener::getVec3f ( const char * path, Vec3f defaultValue ) {
		return getVec3f( string( path ), defaultValue );
	}
	
	Vec3f Listener::getVec3f ( const string &path, Vec3f defaultValue ) {
		Value jsonVal = horten->get( appendPath( path ) );
		
		return jsonToVec3f( jsonVal, defaultValue );
	}


	Vec4f Listener::getVec4f ( const char * path, Vec4f defaultValue ) {
		return getVec4f( string( path ), defaultValue );
	}
	
	Vec4f Listener::getVec4f ( const string &path, Vec4f defaultValue ) {
		Value jsonVal = horten->get( appendPath( path ) );
		
		return jsonToVec4f( jsonVal, defaultValue );
	}

	
	bool Listener::getBool ( const char * path ) {
		return get( path ).asBool();
	}

	string Listener::appendPath( const string &path) {
		return Listener::path + string("/") + string( path );
	}
	
	bool Listener::updated() {
		clock_t t = clock();
		bool ret = updatedLastCheck < horten->getTime( path );
		updatedLastCheck = t;
		return ret;
	}
}
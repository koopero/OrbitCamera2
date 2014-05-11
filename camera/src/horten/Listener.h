//
//  Listener.h
//  Horten
//
//  Created by Samm Cooper on 2013-09-15.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#ifndef __Horten__Listener__
#define __Horten__Listener__

#include <iostream>
#include <string>
#include "json/json.h"
#include "Horten.h"
#include "cinder/Cinder.h"
#include "JsonToCinder.h"


using namespace Json;
using namespace std;
using namespace ci;

namespace Horten {
    class Listener {
        public :
            Listener ();
        
            string path = string("/");
            Horten * horten = NULL;
        
        
            Value get ();
            Value get ( const char * path );
			Value get ( const string &path );
            double getDouble ();
            double getDouble ( const char * path, double defaultValue = NAN );
            double getDouble ( const std::string &path, double defaultValue = NAN );
		
			string getString ( const string &path, const string &defaultValue = string() );
		
		
			ci::Vec3f getVec3f ( const char * path, ci::Vec3f defaultValue = ci::Vec3f(0,0,0) );
			ci::Vec3f getVec3f ( const std::string &path, ci::Vec3f defaultValue = ci::Vec3f(0,0,0) );
		
			ci::Vec4f getVec4f ( const char * path, ci::Vec4f defaultValue = ci::Vec4f(0,0,0,1) );
			ci::Vec4f getVec4f ( const std::string &path, ci::Vec4f defaultValue = ci::Vec4f(0,0,0,1) );
		
		
            bool getBool ( const char * path = NULL );
			std::string appendPath ( const std::string &path );
        
            bool updated ( );
        
        private :
            clock_t updatedLastCheck = 0;
        
    };
}

#endif /* defined(__Horten__Listener__) */

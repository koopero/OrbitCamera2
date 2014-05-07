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

using namespace Json;
using namespace std;

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
			ci::Vec3f getVec3f ( const char * path, ci::Vec3f defaultValue = ci::Vec3f(0,0,0) );
			ci::Vec3f getVec3f ( const std::string &path, ci::Vec3f defaultValue = ci::Vec3f(0,0,0) );
		
            bool getBool ( const char * path = NULL );
            string appendPath ( const string &path );
        
            bool updated ( );
        
        private :
            clock_t updatedLastCheck = 0;
        
    };
}

#endif /* defined(__Horten__Listener__) */

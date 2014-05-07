//
//  Horten.h
//  horten-cpp
//
//  Created by Samm Cooper on 2013-08-28.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#ifndef __horten_cpp__Horten__
#define __horten_cpp__Horten__

#include <iostream>
#include <pthread.h>
#include <cmath>
#include <sys/time.h>
#include <map>

#include "json/json.h"

namespace Horten {

using namespace std;
using namespace Json;
    
    
    enum setFlags {
        DEFAULT = 0,
        KEEP_TOPOLOGY = 2,
        REPLACE = 8
    };
    
    class PathIterator {
    public:
        PathIterator ( string path );
        PathIterator ( const PathIterator &b );
        PathIterator operator ++ ();
        PathIterator operator ++ ( int junk );
        
        void next();
        void prev();
        
        bool null();
        const string seg();
        
    private:
        size_t start;
        size_t end;
        size_t len;
        string _path;
    };
    
class Meta {
public:
    clock_t touched = 0;
    map<string,Meta> children;
    Meta * walk( const string &pathSeg, bool create = false );
    Meta * walk( PathIterator path, bool create = false );
    
    ~Meta ();
    
    void print ( int indent = 0 );
    
    void collapse();
};
    
    


class Horten {
public :

    Horten ();
    ~Horten ();
    
    static Horten * instance ();
    
    Json::Value data;
    Meta meta;
    
    
    int set ( const Value &value, const string &path = string(), const setFlags flags = setFlags::DEFAULT );
    
    int set ( const double value, const string &path = string(), const setFlags flags = setFlags::DEFAULT );
    int set ( const int value, const string &path = string(), const setFlags flags = setFlags::DEFAULT );
    int set ( const string &value, const string &path = string(), const setFlags flags = setFlags::DEFAULT );
    int set ( const char *value, const string &path = string(), const setFlags flags = setFlags::DEFAULT );
    
    
    int setJson ( const string &json, const string &path, const setFlags flags = setFlags::DEFAULT );

    //int setJson ( const string &json );
    //int setJson ( const char *json );
    int setJson ( const string &json, const setFlags flags = setFlags::DEFAULT );
    int setJson ( const char *json, const setFlags flags = setFlags::DEFAULT );
    int setJson ( const char *json, const string &path, const setFlags flags = setFlags::DEFAULT );
    int setJson ( const string &json, const char *path, const setFlags flags = setFlags::DEFAULT );
    int setJson ( const char *json, const char *path, const setFlags flags = setFlags::DEFAULT );
    
    
    Json::Value get ();
    Json::Value get ( const std::string &path );
    
    clock_t getTime( const std::string &path );
    
    double getDouble ( const char * path, double defaultValue = NAN );
    double getDouble ( const std::string &path, double defaultValue = NAN );
    
    
    Json::Value * path ( const std::string &path, bool create = false, Meta ** walkMeta = NULL );

    static int merge ( Json::Value * to, const Json::Value from, const setFlags flags = setFlags::DEFAULT );
    
protected :
    pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
    static Horten * s_instance;

};
}

#endif /* defined(__horten_cpp__Horten__) */

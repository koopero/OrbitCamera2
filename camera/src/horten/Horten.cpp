//
//  Horten.cpp
//  horten-cpp
//
//  Created by Samm Cooper on 2013-08-28.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#include "Horten.h"
#include <string>
#include <iostream>

namespace Horten {
Meta::~Meta () {
	collapse();
}
	
void Meta::collapse() {
	children.clear();
}
	
Meta * Meta::walk(const string &pathSeg, bool create ) {
	if ( create )
		return &children[ pathSeg ];
	
	map<string,Meta>::iterator i = children.find( pathSeg );
	if ( i == children.end() )
		return NULL;
	
	return &(i->second );
}
	
	Meta * Meta::walk ( PathIterator path, bool create ) {
		Meta * ret = this;
		for ( ; ret && !path.null(); path ++ ) {
			ret = ret->walk( path.seg(), create );
		}
		return ret;
	}
    
	void Meta::print ( int indent ) {
		cout << "T=" << touched << endl;
		for ( map<string,Meta>::iterator child = children.begin(); child != children.end(); child ++  ) {
			for ( int i = 0; i < indent; i ++ )
				cout << " ";
			
			cout << child->first << " ";
			
			child->second.print( indent + 1 );
		}
	}
	
	
	PathIterator::PathIterator ( string path ) {
		_path = path;
		start = 0;
		end = 0;
		len = path.size();
		
		while ( len > 0 && path[len-1] == '/' )
			len --;
		
		next();
	}
	
	PathIterator::PathIterator ( const PathIterator &b ) {
		_path = b._path;
		start = b.start;
		end = b.end;
		len = b.len;
	}
	
	PathIterator PathIterator::operator++() {
		PathIterator p = PathIterator ( *this );
		p.next();
		return p;
	}
	
	PathIterator PathIterator::operator++( int junk ) {
		next();
		return *this;
	}
	
	void PathIterator::next() {
		
		do {
			start = end;
			end = _path.find_first_of( '/', start );
			if ( end == start ) {
				end ++;
				continue;
			}
			
			if ( end == std::string::npos )
				end = len;
			
			break;
		} while ( true );
	}
	
	void PathIterator::prev () {
		
	}
	
	const string PathIterator::seg() {
		return _path.substr( start, end - start );
	}
	
	bool PathIterator::null() {
		return start >= end ;
	}
	
	
Horten * Horten::s_instance = NULL;

Horten::Horten ()
{
    data = Json::Value ( Json::ValueType::objectValue );
	meta = Meta();
    cout << "New Horten\n";
}

Horten::~Horten ()
{
    //delete data;
}

Json::Value Horten::get () {
    return data;
}

Json::Value Horten::get ( const std::string &path ) {
    return *(Horten::path(path));
}

double Horten::getDouble ( const char *path, double defaultValue ) {
    return getDouble ( string( path ) );
}

double Horten::getDouble ( const std::string &path, double defaultValue ) {
    Json::Value v = get( path );
    return v.isNumeric() ? v.asDouble() : defaultValue;
}
	
	
	clock_t Horten::getTime( const std::string &path ) {
		PathIterator p = PathIterator( path );
		Meta * m = meta.walk( p );
		
		if ( m )
			return m->touched;
		else
			return 0;
	}

Json::Value * Horten::path ( const std::string &path, bool create, Meta ** walkMeta )
{
    size_t start = 0;
    size_t end = 0;
    size_t len = path.size();
    
    while ( len > 0 && path[len-1] == '/' )
        len --;
    
    Json::Value *ret = &data;
    
    if ( !len )
        return ret;
    
    do {
        end = path.find_first_of( '/', start );
        if ( end == start ) {
            start = end + 1;
            continue;
        }
        
        if ( end == std::string::npos )
            end = len;
        
       
        
        std::string seg = path.substr( start, end - start );
        //std::cout << "seg " << seg << std::endl;
        
        bool last = end == len;
        
        if ( ret->type() != Json::ValueType::objectValue ) {
            if ( !create )
                return new Json::Value( Json::ValueType::nullValue );
            
            (*ret) = Json::Value( Json::ValueType::objectValue );
        }
        
        /*
        if ( ret->isObject() && !ret->isMember( seg ) ) {
            if ( create ) {
                (*ret)[seg] = Json::Value( Json::ValueType::nullValue );
            } else {
                return new Json::Value( Json::ValueType::nullValue );
            }
        }
        */
		
		if ( walkMeta && *walkMeta ) {
			*walkMeta = (*walkMeta)->walk( seg, create );
		}
        
        ret = &((*ret)[seg]);
        start = end;
    } while ( start < len );
    
    return ret;
}

	int setRecurse ( Json::Value * to, const Json::Value &from, Meta * meta, const setFlags flags, const clock_t timestamp ) {
		int touched = 0,
        toIsOb = to->isObject(),
        fromIsOb = from.isObject();
		
		
		if ( toIsOb && fromIsOb ) {
			for ( Json::ValueIterator i = from.begin(); i != from.end(); i ++ ) {
				const char *k = i.memberName();
				touched += setRecurse( &((*to)[k]), from[k], meta->walk( k, true ), flags, timestamp );
				
			}
		} else {
			*to = from;
			touched ++;
		}
		
		if ( touched && meta ) {
			meta->touched = timestamp;
		}
		
		return touched;
	};
	
int Horten::set( const Value &value, const string &path, const setFlags flags )
{
	Meta * metaRef = &meta;
    Json::Value * target = Horten::path ( path, true, &metaRef );
	
	clock_t timestamp = clock();
	int touched = setRecurse( target, value, metaRef, flags, timestamp );
	
	if ( touched ) {
		PathIterator p = PathIterator( path );
		metaRef = &meta;
		for ( ; !p.null(); p ++ ) {
			metaRef->touched = timestamp;
			metaRef = metaRef->walk( p.seg() );
		}
	}
	
	return touched;

}
	



int Horten::set ( const double value, const string &path, const setFlags flags )
    { return set( Value(value), path, flags ); }

int Horten::set ( const int value, const string &path, const setFlags flags )
    { return set( Value(value), path, flags ); }

int Horten::set ( const string &value, const string &path, const setFlags flags )
    { return set( Value(value), path, flags ); }

int Horten::set ( const char *value, const string &path, const setFlags flags )
    { return set( Value(value), path, flags ); }



int Horten::setJson(const std::string &json, const std::string &path, const setFlags flags ) {
    Json::Reader reader;
    Json::Value value;
    reader.parse ( json, value, false );
    return set ( value, path, flags );
}

int Horten::setJson( const string &json, const setFlags flags ) {
    return setJson( json, string(), flags );
}


int Horten::setJson(const char * json, const setFlags flags ) {
    return setJson( string(json), string(), flags );
}

int Horten::setJson(const std::string &json, const char *path, const setFlags flags ) {
    return setJson( json, string(path), flags );
}

int Horten::setJson(const char *json, const string &path, const setFlags flags ) {
    return setJson( string(json), path, flags );
}

int Horten::setJson(const char * json, const char *path, const setFlags flags ) {
    return setJson( string(json), string(path), flags );
}

Horten * Horten::instance () {
    if (!Horten::s_instance)
        Horten::s_instance = new Horten;
    return Horten::s_instance;
};

int Horten::merge ( Json::Value * to, const Json::Value from, const setFlags flags ) {
    int touched = 0,
        toIsOb = to->isObject(),
        fromIsOb = from.isObject();
    
    
    if ( toIsOb && fromIsOb ) {
        for ( Json::ValueIterator i = from.begin(); i != from.end(); i ++ ) {
            const char *k = i.memberName();
            merge ( &((*to)[k]), from[k], flags );
        }
    } else {
        *to = from;
        touched ++;
    }
    
    return touched;
};

}

//
//  TimeRanger.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-10.
//
//

#include "TimeRanger.h"

int64_t wallTimeMS() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	
	long long int time = tp.tv_sec;
	time = time * 1000 + tp.tv_usec / 1000;
	
	return time;
}

void TimeRanger::setPath( string path ) {
	listener.path = path;
}

void TimeRanger::update() {
	quant = (int64_t) listener.getDouble( "/quant/value", 1000 );
	loop = (int64_t) listener.getDouble( "/loop", 0 );
	inTime = (int64_t) listener.getDouble( "/start", 0 );
	offset = (int64_t) listener.getDouble( "/offset", 0 );
	
	Json::Value fileVal = listener.get( "/path" );
	
	if ( fileVal.isString() ) {
		filenameTemplate = fileVal.asString();
	} else {
		filenameTemplate = string ();
	}
		
}

int64_t TimeRanger::getTimeMS() {
	int64_t time = wallTimeMS();
	
	time -= offset;
	
	if ( loop ) {
		time = time % loop;
		if ( time < 0 )
			time += loop;
	}
	
	time += inTime;
	
	if ( quant ) {
		time = time / quant;
		time = time * quant;
	}
	
	return time;
}

string TimeRanger::getFileName() {
	int64_t time = getTimeMS();
	
	string filename = filenameTemplate;
	string replace = string("$t");
	size_t pos = filename.find( replace );
	
	if ( pos != string::npos ) {
		
		char num[64];
		sprintf( num, "%013" PRIi64, time );
		
		filename.replace( pos, replace.length(), string(num) );
	}
	
	return filename;
}
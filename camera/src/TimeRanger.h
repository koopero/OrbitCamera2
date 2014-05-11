//
//  TimeRanger.h
//  camera
//
//  Created by Samm Cooper on 2014-05-10.
//
//

#ifndef __camera__TimeRanger__
#define __camera__TimeRanger__

#include <iostream>
#include <string>

#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>

#include "horten/Listener.h"

using namespace std;
using namespace Horten;

class TimeRanger {
public:
	void setPath( string path );
	
	void update ();
	
	int64_t getTimeMS();
	string getFileName();
	
	string filenameTemplate;

private:
	Listener listener;
	
	int64_t offset = 0;
	int64_t inTime = 0;
	int64_t loop = 0;
	int64_t	quant;
	
};

int64_t wallTimeMS ();


#endif /* defined(__camera__TimeRanger__) */

//
//  JsonToCinder.cpp
//  camera
//
//  Created by Samm Cooper on 2014-05-09.
//
//

#include "JsonToCinder.h"

Vec3f jsonToVec3f ( Value v, Vec3f val ) {
	if ( v.isNumeric() ) {
		float f = v.asFloat();
		val = Vec3f( f, f, f );
	} else if ( v.isObject() ) {
		if ( v["x"].isNumeric() ) {
			val.x = v["x"].asFloat();
		} else if ( v["r"].isNumeric() ) {
			val.x = v["r"].asFloat();
		}
		
		if ( v["y"].isNumeric() ) {
			val.y = v["y"].asFloat();
		} else if ( v["g"].isNumeric() ) {
			val.y = v["g"].asFloat();
		}
		
		if ( v["z"].isNumeric() ) {
			val.z = v["z"].asFloat();
		} else if ( v["b"].isNumeric() ) {
			val.z = v["b"].asFloat();
		}
		
	}

	return val;
};

Vec4f jsonToVec4f ( Value v, Vec4f val ) {
	if ( v.isNumeric() ) {
		float f = v.asFloat();
		val = Vec4f( f, f, f, f );
	} else if ( v.isObject() ) {
		if ( v["x"].isNumeric() ) {
			val.x = v["x"].asFloat();
		} else if ( v["r"].isNumeric() ) {
			val.x = v["r"].asFloat();
		}
		
		if ( v["y"].isNumeric() ) {
			val.y = v["y"].asFloat();
		} else if ( v["g"].isNumeric() ) {
			val.y = v["g"].asFloat();
		}
		
		if ( v["z"].isNumeric() ) {
			val.z = v["z"].asFloat();
		} else if ( v["b"].isNumeric() ) {
			val.z = v["b"].asFloat();
		}
		
		if ( v["w"].isNumeric() ) {
			val.w = v["w"].asFloat();
		} else if ( v["a"].isNumeric() ) {
			val.w = v["a"].asFloat();
		}
	}

	return val;
};
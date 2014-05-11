//
//  JsonToCinder.h
//  camera
//
//  Created by Samm Cooper on 2014-05-09.
//
//

#ifndef __camera__JsonToCinder__
#define __camera__JsonToCinder__

#include <iostream>

#include "cinder/Cinder.h"
#include "Horten.h"

using namespace cinder;
using namespace Horten;


Vec3f jsonToVec3f ( Value v, Vec3f defaultValue );
Vec4f jsonToVec4f ( Value v, Vec4f defaultValue );


#endif /* defined(__camera__JsonToCinder__) */

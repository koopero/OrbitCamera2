//
//  Socket.h
//  horten-cpp
//
//  Created by Samm Cooper on 2013-08-29.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#ifndef __horten_cpp__Socket__
#define __horten_cpp__Socket__

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "Horten.h"

namespace Horten {

class Socket {
public:
    char * hostname;
    char * port;
    
    string path = string();
    setFlags flags = setFlags::DEFAULT;
    
    Horten * horten = NULL;
    
    
    void start();
    void stop();
    void socketThread();
    
protected:
    
    
    pthread_t connectThread;
    int socketfd;
};

struct hss_header {
	uint32_t string_bytes;
};
    
}

#endif /* defined(__horten_cpp__Socket__) */

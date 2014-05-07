//
//  Socket.cpp
//  horten-cpp
//
//  Created by Samm Cooper on 2013-08-29.
//  Copyright (c) 2013 Samm Cooper. All rights reserved.
//

#include "Socket.h"

#include <errno.h>

namespace Horten {
void * _startSocketThread ( void * ob ) {
    ((Socket * ) ob)->socketThread();
    return 0;
}


void Socket::start() {
    if ( !horten )
        horten = Horten::instance();
    
    pthread_create( &connectThread, NULL, &_startSocketThread, (void*)this );
}


void Socket::stop() {
}

void Socket::socketThread() {
    Json::Reader reader;
    
	bool running = true;
    int addrinfo_status, connect_status, socket_status = 0;
    
	while ( running ) {
        
		
        
		if ( !socketfd || socket_status ) {
			struct addrinfo host_info;
			struct addrinfo *host_info_list;
            
			// Close the existing socket, if it has been left open.
			if ( socketfd ) {
				close( socketfd );
				socketfd = NULL;
			}
            
			// Establish connection
			memset( &host_info, 0, sizeof host_info );
            
			host_info.ai_family = AF_UNSPEC;
			host_info.ai_socktype = SOCK_STREAM;
            
			addrinfo_status = getaddrinfo( hostname, port, &host_info, &host_info_list );
            
			if ( addrinfo_status ) {
				std::cout << "couldn't get address" << std::endl ;
				sleep(1);
				continue;
			}
            
			socketfd = socket( host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol );
            
			if ( socketfd == -1 ) {
				socket_status = -1;
				std::cout << "socket error" << std::endl ;
				sleep(1);
				continue;
			}
            
			connect_status = connect( socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen );
            
			if ( connect_status ) {
				socket_status = -1;
				std::cout << "connect error" << std::endl ;
				sleep(1);
				continue;
			}
            
			socket_status = 0;
		}
        
		int max_message_size = 1000000;
        
		ssize_t bytes_recieved;
        
		struct hss_header header;
		string buffer;
        
		bytes_recieved = recv( socketfd, &header, sizeof header, 0 );
        
		if ( bytes_recieved == sizeof header ) {
            
			// Parse the header ( what there is of it )
			header.string_bytes = ntohl( header.string_bytes );
            
			if ( header.string_bytes > max_message_size ) {
				// Bad header, fuck it.
				socket_status = -3;
				std::cout << "bad header, disconnecting";
				continue;
			}
            
			buffer.resize( header.string_bytes );
            
			size_t b = recv( socketfd, (void * ) buffer.c_str(), header.string_bytes, MSG_WAITALL );
            
			if ( b != header.string_bytes ) {
                
			}
            
            Json::Value msg;
            
			std::cout << buffer << std::endl;
            reader.parse( buffer, msg );
            
            Json::Value set = msg["set"];
            if ( set.isObject() ) {
                
                Json::Value set = msg["set"];
                for ( Json::ValueIterator i = set.begin(); i != set.end(); i ++ ) {
                    string k = (string) i.memberName();
                    horten->set( set[k], k, flags );
                }
            }

		} else if (bytes_recieved == 0) {
			std::cout << "host shut down." << std::endl ;
			socket_status = -3;
			sleep(1);
		} else if (bytes_recieved == -1 ) {
			std::cout << "receive error! " << errno << std::endl ;
			socket_status = -4;
			sleep(1);
		}
	}
}
}
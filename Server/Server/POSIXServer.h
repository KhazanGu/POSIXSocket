//
//  POSIXServer.h
//  Server
//
//  Created by Khazan on 2023/8/30.
//

#ifndef POSIXServer_h
#define POSIXServer_h

#include <stdio.h>

int server_socket_create(void);

int server_socket_bind(int server_socket, int16_t port);

int server_socket_listen_connections(int server_socket);

void server_socket_accpet_connections_on_child_thread(int server_socket, void(^handler)(int client_socket));


#endif /* POSIXServer_h */

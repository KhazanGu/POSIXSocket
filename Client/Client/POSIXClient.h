//
//  POSIXClient.h
//  Client
//
//  Created by Khazan on 2023/8/30.
//

#ifndef POSIXClient_h
#define POSIXClient_h

#include <stdio.h>

int client_socket_create(void);

int client_socket_connect_server_socket(int client_socket, const char* ip_address, int16_t port);

void client_socket_send_message(int client_socket, const char* string);

void client_socket_receive_message_on_child_thread(int client_socket, void(^disconnect)(void),void(^handler)(const char* string));

#endif /* POSIXClient_h */

//
//  POSIXServer.c
//  Server
//
//  Created by Khazan on 2023/8/30.
//

#include "POSIXServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dispatch/dispatch.h>

int server_socket_create(void) {
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_socket == -1) {
        perror("Server socket create failed");
        return 1;
    }
    
    printf("stream socket created.\n");
    
    return server_socket;
}

int server_socket_bind(int server_socket, int16_t port) {
    
    struct sockaddr_in server_addr;
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Server socket binding failed");
        return 1;
    }
    
    printf("stream socket bound on port: %d.\n", port);

    return 0;
}

int server_socket_listen_connections(int server_socket) {
    
    if (listen(server_socket, 5) == -1) {
        perror("Server socket listening failed");
        return 1;
    }
    
    printf("stream socket listening...\n");

    return 0;
}

int server_socket_accpet_connections(int server_socket) {
    
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    
    if (client_socket == -1) {
        perror("Server socket accepting connection failed");
        return 1;
    }

    printf("Server socket connection accepted from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    return client_socket;
}

void server_socket_accpet_connections_on_child_thread(int server_socket, void(^handler)(int client_socket)) {
    
    dispatch_queue_t accept_queue = dispatch_queue_create("accept_queue", DISPATCH_QUEUE_SERIAL);
    
    dispatch_async(accept_queue, ^{
        while (1) {
            int client_socket = server_socket_accpet_connections(server_socket);
            
            handler(client_socket);
        }
    });
}

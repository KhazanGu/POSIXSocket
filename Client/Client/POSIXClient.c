//
//  POSIXClient.c
//  Client
//
//  Created by Khazan on 2023/8/30.
//

#include "POSIXClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dispatch/dispatch.h>

int client_socket_create(void) {
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client_socket == -1) {
        perror("Stream socket creation failed");
        return 1;
    }
    
    printf("Stream socket creation succeed.\n");

    return client_socket;
}


int client_socket_connect_server_socket(int client_socket, const char* ip_address, int16_t port) {
    
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Stream socket invalid address");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Stream socket connection failed");
        return 1;
    }

    printf("Connected to server...\n");
    
    return 0;
}

void client_socket_send_message(int client_socket, const char* string) {
        
    size_t size = sizeof(string);
    
    ssize_t status = send(client_socket, string, size, 0);

    if (status == -1) {
        perror("Send failed\n");
    }
}

char* client_socket_receive_message(int client_socket) {
        
    char buffer[1024];
    
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (bytes_received == 0) {
        printf("disconnected");
        
        return NULL;
    }
    
    printf("Received: %s length: %zd\n", buffer, bytes_received);
    
    return buffer;
}

void client_socket_receive_message_on_child_thread(int client_socket, void(^disconnect)(void),void(^handler)(const char* string)) {
    
    dispatch_queue_t receive_queue = dispatch_queue_create("receive_queue", DISPATCH_QUEUE_SERIAL);
    
    dispatch_async(receive_queue, ^{
        while (1) {
            
            char *string = client_socket_receive_message(client_socket);
            
            if (string == NULL) {
                disconnect();
                break;
            }
            
            size_t length = strlen(string);
            
            if (length > 0) {
                handler(string);
            }
        }
    });
}


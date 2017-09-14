/*
 * Class: Server
 * Authors: Jeremiah Hanson, Haodong Wang
 * ----------------------------------------------------------------
 * Purpose: A simple server 
 */
 
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>

int main() {
    int s_socket = 0, client_sock, client_addr, read_size;
    struct sockaddr_in server, client;
    char message[2000];
    
    // Create a socket
    if((s_socket = socket(AF_NET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "ERROR: Cannot create the socket/n");
        return 1;
    }
    
    // Initialize  sockaddr_in data structure
    server.sin_family = AF_NET;
    server.sin_port = htons(2000);
    server.sin_addr.s_addr = INADDR_ANY;
    
    // Bind
    if (bind(s_socket, (struct sockaddr *)&server, sizeof(server))) < 0){
        fprintf(stderr, "ERROR: Bind failed!/n");
        return 1;
    }
    
    listen(s_socket, 3);
    
    // Accept any connection
    client_addr = sizeof(struct sockaddr_in);
    
    client_sock = accept(s_socket, (struct sockaddr *)&client, (socklen_t*)&client_addr);
    if (client_sock < 0) {
        fprintf(stderr, "ERROR: Accept failed!/n");
        return 1;
    }
    
    // Recieve a message from the client
    while ((read_size = recv(client_sock, message, 2000, 0)) > 0){
        // Send the message back to the client 
        write(client_sock, message, strlen(message));
    }
    
    return 0;
    
}
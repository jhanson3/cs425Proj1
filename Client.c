/*
 * Class: Client
 * Authors: Jeremiah Hanson, Haodong Wang
 * ----------------------------------------------------------------
 * Purpose: a client used to communicate with a host
 */

#include <sys/socket.h>
#include <stdio.h>
 
int main(){
    
    int s = 0;
    struct sockaddr_in server_addr;
    
    // Create the socket
    if ((s = socket(AF_NET, SOCK_STREAM, 0))<0){ 
        fprintf(stderr, "ERROR: Cannot create the socket/n");
        return 1;
    }
    
    // Initialize  sockaddr_in data structure
    server_addr.sin_family = AF_NET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Connect to the server
    if (connect(s, (struct sockaddr_in *)&server_addr, sizeof(server_addr))<0){
        fprintf(stderr, "ERROR: Cannot connect/n");
        return 1;
    }
    
    send(s, "Hello", strlen("Hello"), 0);
    
    close(s);
    
    
    
    
}
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
#include<stdlib.h>

int main(int argc, char *argv[]) {
  int i;
  int port_num=0;
  int message_length;
  int s_socket = 0, client_sock, client_addr, read_size;
  struct sockaddr_in server, client;
  char* message;

  //read the parameter as the port we use
  for(i=0;i<strlen(argv[1]);i++){
    port_num=port_num*10+(argv[1][i]-'0');
  }
  message=(char*)malloc(1024);

  // Create a socket
  if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "ERROR: Cannot create the socket/n");
    return 1;
  }

  // Initialize  sockaddr_in data structure
  server.sin_family = AF_INET;
  server.sin_port = htons(port_num);
  server.sin_addr.s_addr = INADDR_ANY;

  // Bind
  if (bind(s_socket, (struct sockaddr *)&server, sizeof(server)) < 0){
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
  while (1){
    //keep receive the first 4 bite of the pacage as the length of the string receive latter
    if((read_size = recv(client_sock, &message_length, 4, 0))>0){
      fprintf(stdout, "There are %d of character(s) has received!\n", message_length);
      //we use the length we got before as the lenngth we want to read here
      (read_size = recv(client_sock, message, message_length, 0));
      fprintf(stdout, "%s\n", message);
    }
    else break;
  }
  return 0;
}

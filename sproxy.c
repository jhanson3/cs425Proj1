/*
 * Class: Client
 * Authors: Jeremiah Hanson, Haodong Wang
 * ----------------------------------------------------------------
 * Purpose: a client used to communicate with a host
 */
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/select.h>

int main(int argc, char * argv[]){
  
  int rv=0;
  int s1 = 0;
  int s2 = 0;
  int n,i;
  int length=0;
  int port_num1=0;
  int client_server=0;
  int client_addr=0;
  fd_set readfds;
  struct timeval tv;
  struct sockaddr_in client, telnet;
  char * buf2;
  char * buf1;
  buf2=(char*)malloc(1024);
  buf1=(char*)malloc(1024);
  if( buf2 == NULL ){
    fprintf(stderr,"Unable to allocate buf2\n");
    return 1;
  }
  if( buf1 == NULL ){
    fprintf(stderr,"Unable to allocate buf1\n");
    return 1;
  }
  // Create the socket
  if ((s1 = socket(AF_INET, SOCK_STREAM, 0))<0){
    fprintf(stderr, "ERROR: Cannot create the \"client-server\" socket\n");
    return 1;
  }
  if ((s2 = socket(AF_INET, SOCK_STREAM, 0))<0){
    fprintf(stderr, "ERROR: Cannot create the \"cproxy-client\" socket\n");
    return 1;
  }
  //read the parameter as the port we use
  for(i=0;i<strlen(argv[1]);i++){
    port_num1=port_num1*10+(argv[1][i]-'0');
  }

  // Initialize  sockaddr_in data structure
  telnet.sin_family = AF_INET;
  telnet.sin_port = htons(23);
  telnet.sin_addr.s_addr = inet_addr("127.0.0.1");
  client.sin_family = AF_INET;
  client.sin_port = htons(port_num1);
  client.sin_addr.s_addr = INADDR_ANY;

  // Connect to the server
  if (connect(s2, (const struct sockaddr *)&telnet, sizeof(telnet))<0){
    fprintf(stderr, "ERROR: Cannot connect telnet-cproxy\n");
    return 1;
  }

  // blind the client
  if (bind(s1, (struct sockaddr *)&client, sizeof(client)) < 0){
    fprintf(stderr, "ERROR: Cannot bind cproxy-sproxy\n");
    return 1;
  }

  listen(s1,3);

  // Accept any connection
  client_addr = sizeof(struct sockaddr_in);
  client_server = accept(s1, (struct sockaddr *)&client, (socklen_t*)&client_addr);
  if (client_server < 0) {
    fprintf(stderr, "ERROR: Accept failed!\n");
    return 1;
  }

  while(1){
    //clear the set ahead of time
    FD_ZERO(&readfds);

    //add our descriptors to the set
    FD_SET(client_server, &readfds);
    FD_SET(s2, &readfds);

    // find the largest descriptor, and plus one.
    if (client_server > s2) n = client_server + 1;
    else n = s2 +1;

    // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
    tv.tv_sec = 10;
    tv.tv_usec = 500000;
    rv = select(n, &readfds, NULL, NULL, &tv);

    if(rv==-1){
      perror("select error\n");
    }
     else if(rv == 0){
      printf("Timeout occurred! No data after 10.5 seconds.\n");
    }
    else{
      // one or both of the descriptors have data
      if (FD_ISSET(client_server, &readfds)) {
        if((length=recv(client_server, buf1, 1024, 0))>0){
          if(send(s2,buf1, length,0)<0){
            fprintf(stderr, "ERROR: Send failed!\n");
            return 1;
          }
        }
        else  break;
      }
      if (FD_ISSET(s2, &readfds)) {
        if((length=recv(s2, buf2, 1024, 0))>0){
          if(send(client_server, buf2, length, 0)<0){
            fprintf(stderr, "ERROR: Send failed!\n");
            return 1;
          }
        }
        else  break;
      }
    }
  }
  printf("here\n");
  close(s1);
  close(s2);
  return 0;
}

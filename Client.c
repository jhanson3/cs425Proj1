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
 
int main(int argc, char * argv[]){
  
  int s = 0;
  int i;
  int length;
  int MAX_length=1024;
  int port_num;
  char* ip=argv[1];
  struct sockaddr_in server_addr;
  char *message;
  message=(char*)malloc(1024);
  if( message == NULL ){
    fprintf(stderr,"Unable to allocate buffer");
    return 1;
  }
  // Create the socket
  if ((s = socket(AF_INET, SOCK_STREAM, 0))<0){ 
    fprintf(stderr, "ERROR: Cannot create the socket/n");
    return 1;
  }

  //read the parameter as the port we use
  for(i=0;i<strlen(argv[2]);i++){
    port_num=port_num*10+(argv[2][i]-'0');
  }
  
  // Initialize  sockaddr_in data structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(22074);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Connect to the server
  if (connect(s, (const struct sockaddr *)&server_addr, sizeof(server_addr))<0){
    fprintf(stderr, "ERROR: Cannot connect/n");
    return 1;
  }
  //we here get the input of user
  while( fgets(message,MAX_length,stdin)!=NULL ){
    //trying to delete the last new line symbol at last of the string
    i = strlen(message) -1;
    if (message[i] == '\n')  message[i] = '\0';
    //set up the length
    length=strlen(message);
    //send the length of the string first as requirement
    if (send(s, &length, 4, 0) < 0){
      fprintf(stderr, "ERROR: Send failed!/n");
      return 1;
    }
    //after the length of the string we send the string 
    if(send(s, message,strlen(message),0)<0){
      fprintf(stderr, "ERROR: Send failed!/n");
      return 1;
    }
  }
  close(s);
  return 0;
}

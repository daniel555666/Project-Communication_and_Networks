#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9999
#define SIM_LENGTH 10

int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  socklen_t len; 
  int count;
  
  sock = socket(AF_INET, SOCK_STREAM, 0); //Create a new socket of type SOCK_STREAM(TCP) 
    
  bzero(&serv_name, sizeof(serv_name));  //erases the data in the struct by writing zeros (bytes containing '\0') to that area
  serv_name.sin_family = AF_INET;        //set to IPv4                            
  serv_name.sin_port = htons(PORT);      //set server port

  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name));
  //assigns the address specified by serv_name to the socket referred to by the file descriptor sock

        
  listen(sock, 1); // listen to 1 client 

  len = sizeof(serv_name); // save the size of serv_name
  
  connect_sock = accept(sock,(struct sockaddr *)&serv_name, &len);
  //connect_sock get the socket's file descriptor for connection with the client.

  for (count = 1; count <= SIM_LENGTH; count++)  // send the count number to the client SIM_LENGTH times.
    { write(connect_sock, &count, 4);
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); // close the socket for connection with the client.
  close(sock);         // close the server socket.

}

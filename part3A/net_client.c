
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define SIM_LENGTH 10 
#define PORT 9999 

int main(int argc, char* argv[])
{ 
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 

  if (argc != 2) {  // if we didnt get the hostname so exit
    perror("error: insert hostname");
    exit(1);
  }

  printf("Client is alive and establishing socket connection.\n");

  hostname = argv[1]; //save the hostname

  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { //enter the address(from string) to res struct
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//if error in getaddrinfo we exit
    exit(1);
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr; //save the address in struct sockaddr_in
  hostaddr = inet_ntoa(saddr->sin_addr);  // get string of the ip
  
  sock = socket(AF_INET, SOCK_STREAM, 0);//Create a new socket of type SOCK_STREAM(TCP)
  if (sock < 0)// if errors occur in the socket creation                         
    { perror ("Error opening channel");
      close(sock);// close the client socket
      exit(1);    
    }
      

  bzero(&cli_name, sizeof(cli_name)); //erases the data in the struct by writing zeros (bytes containing '\0') to that area
  cli_name.sin_family = AF_INET; //set to IPv4
  cli_name.sin_addr.s_addr = inet_addr(hostaddr);//set ip address to connect to
  cli_name.sin_port = htons(PORT);//set port


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)//Open a connection on sock FD to peer at cli_name
    { perror ("Error establishing communications");
      close(sock); // close the client socket
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4); // get/read the number that sended from the server, enters this number in to value. Does this SIM_LENGTH times.
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock);  // close the client socket
  exit(0); 

} 

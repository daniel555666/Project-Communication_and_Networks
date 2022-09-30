
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) {  // if we didnt get the hostname so exit
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1]; //save the hostname
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { //enter the address(from string) to res struct
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//if error in getaddrinfo we exit
    exit(1);
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr; //save the address in struct sockaddr_in
  hostaddr = inet_ntoa(saddr->sin_addr);  // get string of the ip

  printf("Address for %s is %s\n", hostname, hostaddr); //print the ip
  exit(0);
}

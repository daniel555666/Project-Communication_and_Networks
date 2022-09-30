#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>


void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s ", str2);
  printf("ip= %s, ",inet_ntoa(s->sin_addr));
  printf("port= %d",ntohs(s->sin_port));
  printf("\n");
}


int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  //socket() creates an endpoint for communication and returns a file descriptor that refers to that endpoint
  // Create a new socket of type SOCK_DGRAM(UDP) in domain AF_INET.

  bzero((char *) &s_in, sizeof(s_in));  

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    
  s_in.sin_port = htons((u_short)0x3333);

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
  //assigns the address specified by s_in to the socket referred to by the file descriptor sockfd.

  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);

    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);
  }
  
  return 0;
}


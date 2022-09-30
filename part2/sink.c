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
#include <stdlib.h>
#include <errno.h>




int main(int argc, char *argv[])
{
  int socket_fd, cc;
  struct sockaddr_in  s_in;
  struct hostent *hostptr;
  struct { char head; int body; char tail;} msg;

  if(argc!=3){
    printf("error: need to insert hostname and port\n");
    exit(1);
  }

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);// Create a new socket of type SOCK_DGRAM(UDP)
  if(socket_fd<0){
    perror("error in socket");
    exit(1);
  }
  
  bzero((char *) &s_in, sizeof(s_in)); //erase the struct data with zeros

  hostptr = gethostbyname(argv[1]);//get hostname
  if (hostptr==NULL)
    {
      perror("error in gethostbyname");
      exit(1);
    }  
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);/* WILDCARD *///for the ip
  s_in.sin_family = (short)AF_INET;
  s_in.sin_port = atoi(argv[2]);

  if(bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in))<0){//assigns s_in to the socket for the port we want
    perror("error: bind");
    exit(1);
  }

  for(;;) { // infinite loop for recv the message
    printf("got message from ip=%d ",s_in.sin_addr.s_addr);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,NULL,0);// get the message from gateway
    if(cc<0){
      perror("error in recvfrom");
      exit(1);
    }
    printf("Got data :%c%d%c\n",msg.head,msg.body,msg.tail); 
  }

  close(socket_fd);
  return 0;
}
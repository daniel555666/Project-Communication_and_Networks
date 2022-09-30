#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[])
{
  int socket_fd,cc,cc2;
  struct sockaddr_in  s_in,s_in2;
  struct hostent *hostptr;
  struct { char head; int body; char tail;} msg;

   if(argc!=4){
    printf("error: need to insert hostname and two ports\n");
    exit(1);
  }

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);// Create a new socket of type SOCK_DGRAM(UDP)
  if(socket_fd<0){
    perror("error in socket");
    exit(1);
  }
  
  bzero((char *) &s_in, sizeof(s_in));//erase the struct data with zeros
  hostptr = gethostbyname(argv[1]); //get hostname
  if (hostptr==NULL)
    {
      perror("error in gethostbyname");
      exit(1);
    }

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);  /* WILDCARD *///for the ip
  s_in.sin_port = atoi(argv[2]); //get the first port

  s_in2.sin_addr.s_addr = htonl(INADDR_ANY);  /* WILDCARD *///for the ip
  s_in2.sin_family = (short)AF_INET;
  s_in2.sin_port = atoi(argv[3]); //get the second port


  if(bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in))<0){  //assigns the address to the socket

    perror("error in bind");
    exit(1);
  }

  srand(time(NULL)); //Seed the random number generator 

  for(;;) { // infinite loop for recv and send the message

    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,NULL,0); //get the message from source
    if(cc<0){
      perror("error in recvfrom");
      exit(1);
    }
    printf("Got message :%c%d%c\n",msg.head,msg.body,msg.tail); 

    if(((float)random())/((float)RAND_MAX)>0.5){ // do random, need >0.5 to send the mmesage

      cc2=sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&s_in2,sizeof(s_in2));//send the message to sink
      if(cc2<0){
      perror("error in sendto");
      exit(1);
      }
      printf("send the message\n\n");
    }

    else{
      printf("didnt send the message\n\n");
    }
  }
  close(socket_fd);
  return 0;
}


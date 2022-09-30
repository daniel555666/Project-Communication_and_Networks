#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; int body; char tail; } msgbuf;

  if(argc!=3){
    printf("error: need to insert hostname and port\n");
    exit(1);
  }

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // Create a new socket of type SOCK_DGRAM(UDP)
  if(socket_fd<0){
    perror("error in socket");
    exit(1);
  }

  bzero((char *) &dest, sizeof(dest));//erase the struct data with zero 
  hostptr = gethostbyname(argv[1]);  //get hostname
  if (hostptr==NULL)
    {
      perror("error in gethostbyname");
      exit(1);
    }
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);//copy to the struct
  dest.sin_port =atoi(argv[2]);

  msgbuf.head = '<'; //set the first meesage
  msgbuf.body=0;
  msgbuf.tail = '>';

  for(;;){  // infinite loop for send the message

    if(sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest))<0){ //send the message
      perror("error in sendto");
      exit(1);
    }
    printf("send message :%c%d%c to gateway \n",msgbuf.head,msgbuf.body,msgbuf.tail);

    (msgbuf.body)++; //add +1 to the number we send
    sleep(1); 

  }

  close(socket_fd);
  return 0;
}

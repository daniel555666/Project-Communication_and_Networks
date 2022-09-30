#include <stdlib.h>
#include <stdio.h>

#include "parseurl.h"

int main( int argc, char* argv[] ) {
  url_data_t* url;
 
  if (argc != 2) 
  {
       fprintf(stderr,"Usage: %s url\n", argv[0]);
       exit(1);
  }

  url = malloc(sizeof(url_data_t));

  if (url==NULL)
  {
       perror("Could not allocate space for url data structure");
       exit (1);
  }

  if (0 != parse_url(url, argv[1]))
  {
       perror("Error: Could not parse URL\n");
       exit(1);
  }
  
  printf( "URL: %s\n", url->url);
  printf( "Protocol: %s\n", url->protocol);
  printf( "Hostname: %s\n", url->hostname);
  printf( "Port: %d\n", url->port);
  printf( "Path: %s\n", url->path);

  free_url_data(url);
  free(url);
  return 0;
}
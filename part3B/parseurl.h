#ifndef __PARSEURL_H__
#define __PARSEURL_H__

#define ERRVAL -1
#define DEFAULT_PORT 80

typedef struct url_data {
  char* url;
  char* protocol;
  char* hostname;
  char* path;
  int port;
} url_data_t;

int parse_url(url_data_t* data, const char* url);
void free_url_data(url_data_t* data);

#endif
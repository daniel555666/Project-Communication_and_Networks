int parse_url(url_data_t* data, const char* url) {
  char* copy;
  char** urlp = &copy;

  bzero(data, sizeof(url_data_t));        /* Clear data */

  data->url = malloc(strlen(url)+1);      /* Make a copy of url to keep */
  strncpy(data->url, url, strlen(url));

  copy = malloc(strlen(url)+1);           /* Make a copy of url to destroy */
  strncpy(copy, url, (strlen(url)));                                                
  /* Parse the protocol */
  data->protocol = strsep(urlp, ":");
  if (*urlp == NULL) return ERRVAL;

  /* String out the "//" */
  strsep(urlp, "/");                      
  if (*urlp == NULL) return ERRVAL;
  strsep(urlp, "/");                    
  if (*urlp == NULL) return ERRVAL;

  /* Check for ':' -- if it is there, parse the hostname and port number
   * Otherwise just parse the hostname and use a default port number 
   */
  if (strstr(*urlp, ":")) {           
    data->hostname = strsep(urlp, ":");
    if (*urlp == NULL) return ERRVAL;
    data->port = atoi(strsep(urlp, "/"));
  } else {                               
    data->hostname = strsep(urlp, "/");
    data->port = DEFAULT_PORT;          
  }
  if (*urlp == NULL)
  {
       /* Trailing "/" not given, so we use an empty null-terminated string
        * as the path */
       data->path = malloc(sizeof(char));
       *(data->path) = '\0';
  }
  else
  {
       /* The path is whatever is left! */
       data->path = *urlp;
  }

  return 0;
}

/* Procedure: free_url_data
 * Purpose:   Free the contents of a url data structure.
 * Parameter: data, a pointer to struct url_data
 * Preconditions:
 *            data is not NULL
 * Postconditions: 
 *            all strings pointed to by data have been freed
 */
void free_url_data(url_data_t* data) {
  if (NULL != data->url) free (data->url);
  if (NULL != data->protocol) free(data->protocol);
}

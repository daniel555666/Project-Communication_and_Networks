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
#include "parseurl.h"

int main(int argc, char *argv[])
{
    url_data_t *url_data;
    struct addrinfo *res;
    char *hostaddr;
    struct sockaddr_in *saddr;
    int sock;
    struct sockaddr_in cli_name;

    if (argc != 2)
    { // if we didnt get the url so exit
        perror("error: insert url");
        exit(1);
    }

    url_data = (url_data_t *)malloc(sizeof(url_data_t));
    if (url_data == NULL)
    {
        perror("error in malooc");
        exit(1);
    }

    if (parse_url(url_data, argv[1])<0)
    {
        perror("error not valid url,exit at parse");
        exit(1);
    }

    if (0 != getaddrinfo(url_data->hostname, NULL, NULL, &res))
    {                                                                  // enter the address(from string) to res struct
        fprintf(stderr, "Error in resolving hostname %s\n", url_data->hostname); // if error in getaddrinfo we exit
        exit(1);
    }

    saddr = (struct sockaddr_in *)res->ai_addr; // save the address in struct sockaddr_in
    hostaddr = inet_ntoa(saddr->sin_addr);      // get string of the ip

    printf("Client is alive and establishing socket connection.\n");

    sock = socket(AF_INET, SOCK_STREAM, 0); // Create a new socket of type SOCK_STREAM(TCP)
    if (sock < 0)                           // if errors occur in the socket creation
    {
        perror("Error opening channel");
        close(sock); // close the client socket
        exit(1);
    }

    bzero(&cli_name, sizeof(cli_name));             // erases the data in the struct by writing zeros (bytes containing '\0') to that area
    cli_name.sin_family = AF_INET;                  // set to IPv4
    cli_name.sin_addr.s_addr = inet_addr(hostaddr); // set ip address to connect to
    cli_name.sin_port = htons(url_data->port);      // set port

    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) // Open a connection on sock FD to peer at cli_name
    {
        perror("Error establishing communications");
        close(sock); // close the client socket
        exit(1);
    }

    char *str = (char *)malloc(strlen("GET / HTTP/1.0\r\nHost: \r\nConnection: close\r\n\r\n") + strlen(url_data->path) + strlen(url_data->hostname) + 1);
    // put the message in str
    sprintf(str, "GET /%s  HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", url_data->path, url_data->hostname);

    if (send(sock, str, strlen(str), 0) < 0)
    { // send the GET request
        perror("Error in send");
        close(sock); // close the client socket
        exit(1);
    }
    printf("send Get request");

    char buf[512];
    bzero(buf, 512);
    int check_error = 0;

    while ((check_error = recv(sock, buf, 512, 0)) > 0) //get the message
    {
        printf("%s", buf);
    }

    if (check_error < 0)
    {
        perror("Error in recv");
        close(sock); // close the client socket
        exit(1);
    }

    printf("Exiting now.\n");

    close(sock); // close the client socket
    free_url_data(url_data);
    free(url_data);
    free(str);
    exit(0);
}

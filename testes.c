#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h> /* getprotobyname */
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 80

int main(int argc, char** argv) {
    char buffer[BUFSIZ];
    enum CONSTEXPR { MAX_REQUEST_LEN = 1024};
    char request[MAX_REQUEST_LEN];
    char request_template[] = "GET / HTTP/1.0\r\nHost: %s\r\n\r\n";
    struct protoent *protoent;
    char *hostname = "example.com";
    in_addr_t in_addr;
    int request_len;
    int server_socket;
    ssize_t nbytes_total, nbytes_last;
    struct hostent *hostent;
    struct sockaddr_in sockaddr_in;

    if (argc > 1)
        hostname = argv[1];

    request_len = snprintf(request, MAX_REQUEST_LEN, request_template, hostname);
    printf("\n\nChamada: %s\n\n", request);

    if (request_len >= MAX_REQUEST_LEN) {
        fprintf(stderr, "request length large: %d\n", request_len);
        exit(EXIT_FAILURE);
    }

    /* Build the socket. */
    protoent = getprotobyname("tcp");
   /* if (protoent == NULL) {
        perror("getprotobyname");
        exit(EXIT_FAILURE);
    }*/
    server_socket = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    /*if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }*/

    /* Build the address. */
    hostent = gethostbyname(hostname);
    /*if (hostent == NULL) {
        fprintf(stderr, "error: gethostbyname(\"%s\")\n", hostname);
        exit(EXIT_FAILURE);
    }*/
    in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
    /*if (in_addr == (in_addr_t)-1) {
        fprintf(stderr, "error: inet_addr(\"%s\")\n", *(hostent->h_addr_list));
        exit(EXIT_FAILURE);
    }*/
    sockaddr_in.sin_addr.s_addr = in_addr;
    sockaddr_in.sin_family = AF_INET;
    sockaddr_in.sin_port = htons(PORT);

    connect(server_socket, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in));

    /* Actually connect. */
    /*if (connect(server_socket, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }*/

    /* Send HTTP request. */
    nbytes_total = 0;
    while (nbytes_total < request_len) {
        nbytes_last = write(server_socket, request + nbytes_total, request_len - nbytes_total);
        /*if (nbytes_last == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }*/
        nbytes_total += nbytes_last;
    }

    /* Read the response. */
    fprintf(stderr, "debug: before first read\n");
    while ((nbytes_total = read(server_socket, buffer, BUFSIZ)) > 0) {
        fprintf(stderr, "debug: after a read\n");
        write(STDOUT_FILENO, buffer, nbytes_total);
    }
    fprintf(stderr, "debug: after last read\n");
    /*if (nbytes_total == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }*/

    close(server_socket);
    exit(EXIT_SUCCESS);
}
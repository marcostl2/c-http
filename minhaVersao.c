#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char *replaceWord(const char *s, const char *oldW, const char *newW) {
  char *result;
  int i, cnt = 0;
  int newWlen = strlen(newW);
  int oldWlen = strlen(oldW);

  // Counting the number of times old word
  // occur in the string
  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], oldW) == &s[i]) {
      cnt++;

      // Jumping to index after the old word.
      i += oldWlen - 1;
    }
  }

  // Making new string of enough length
  result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

  i = 0;
  while (*s) {
    // compare the substring with the result
    if (strstr(s, oldW) == s) {
      strcpy(&result[i], newW);
      i += newWlen;
      s += oldWlen;
    } else
      result[i++] = *s++;
  }

  result[i] = '\0';
  return result;
}

int main(int argc, char *argv[]) {
  struct addrinfo hints, *res;
  int sockfd;
  char buffer[3000];
  char *host;
  char *url;
  char *n;
  char *last;
  FILE *file;

  host = argv[1];

  if (strstr(host, "http://")) {
    host = replaceWord(host, "http://", "");
  }

  printf("\nLast: %s\n", last);

  if (strstr(host, ".html")) {
    host = replaceWord(host, "/.html", "");
    last = strrchr(host, '/'); // pega a ultima barra barra e conteudo a frente
    host = replaceWord(host, last, "");
  }
  last =
      strchr(host, '/'); // nesse caso pega a url restante apos a primeira barra
  url = last;
  printf("\n%s\n", url);

  if (strlen(url) != 1) {

    url = strcat(url, "/");
  }
  host = replaceWord(host, last, "");

  printf("\n%s\n", url);

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  getaddrinfo(host, "80", &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  // connect!

  connect(sockfd, res->ai_addr, res->ai_addrlen);

  char request[3000];
  // char request[3000]="HEAD / HTTP/1.0\r\n\r\n";
  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);
  printf("\n%s", request);

  send(sockfd, request, strlen(request), 0);
  int recv_length = 1;

  file = fopen("file.html", "w");
  recv_length = recv(sockfd, &buffer, 3000, 0);
  while (recv_length > 0) {
    
    fputs(buffer + 8, file);
    printf("\n%d\n", recv_length);
    // printf("The web server is %s\n", buffer + 8);
     
  }
    fclose(file);
  freeaddrinfo(res);
     return 0;

  

  return 0;
}

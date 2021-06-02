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

  // conta quantas vezes a palavra aparece na palavra antiga
  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], oldW) == &s[i]) {
      cnt++;

      i += oldWlen - 1;
    }
  }

  // novo tamanho para a string nova
  result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

  i = 0;
  while (*s) {
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
  char *host, *url, *n, *last;
  FILE *file;

  host = argv[1];

  if (strstr(host, "http://")) {
    host = replaceWord(host, "http://", "");
  }

  if (strstr(host, ".html")) {
    host = replaceWord(host, "/.html", "");
    last = strrchr(host, '/'); // pega a ultima barra barra e conteudo a frente
    host = replaceWord(host, last, "");
  }
  last =
      strchr(host, '/'); // nesse caso pega a url restante apos a primeira barra
  url = last;

  if (strlen(url) != 1) {

    url = strcat(url, "/");
  }
  host = replaceWord(host, last, "");


  memset(&hints, 0, sizeof hints); // zera a struct
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(host, "80", &hints, &res);

  // realiza a criação do socket
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // faz a cone
  connect(sockfd, res->ai_addr, res->ai_addrlen);

  char request[3000];

  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);

  send(sockfd, request, strlen(request), 0);
  int recv_length = 1;

  file = fopen("file.html", "w");

  while (recv_length > 0) {
    recv_length = recv(sockfd, &buffer, sizeof(buffer), 0);
    fputs(buffer + 8, file);
  };
  freeaddrinfo(res);
  fclose(file);

  return 0;
}

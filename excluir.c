#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int main() {
  /*Define socket variables */
  char host[1024] = "www.sipros.pa.gov.br";
  char url[1024] = "/";
  char request[3000];
  struct hostent *server;
  struct sockaddr_in serverAddr;
  int portno = 80;

  printf("Trying to get source of pastebin.com/raw/7y7MWssc ...\n");

  /* Create socket */
  int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (tcpSocket < 0) {
    printf("ERROR opening socket\n");
  } else {
    printf("Socket opened successfully.\n");
  }

  server = gethostbyname(host);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portno);
  if (connect(tcpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <
      0) {
    printf("Can't connect\n");
  } else {
    printf("Connected successfully\n");
  }

  bzero(request, 2000);
  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);
  printf("\n%s", request);

  if (send(tcpSocket, request, strlen(request), 0) < 0) {
    printf("Error with send()");
  } else {
    printf("Successfully sent html fetch request");
  }
  printf("test\n");
}
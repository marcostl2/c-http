#include <stdio.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 1024  /* tamanho do buffer */

int main() {
  char buff[129];

  struct sockaddr_in caddr;
  struct sockaddr_in saddr;

  socklen_t addr_size;

  int server = socket(AF_INET, SOCK_STREAM, 0);
  int client, x;
  int csize = sizeof caddr;

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(5000);

  bind(server, (struct sockaddr *)&saddr, sizeof saddr);
  listen(server, 5);

  while (1) {
    client = accept(server, (struct sockaddr *)&caddr, &csize);
   /* x = recv(client, buff, sizeof buff, 0); */

    recvfrom(server, buff, MAX, 0, (struct sockaddr *)&caddr, &addr_size);

    /* recvfrom(socket, buffer, MAX, 0, (struct sockaddr *)&addr, &addr_size); */

    send(client, buff, x, 0);

    puts(buff);
    fflush(stdout);

    close(client);
  }

  return 0;
}
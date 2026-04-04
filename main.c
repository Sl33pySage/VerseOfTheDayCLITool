#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main() {
  struct sockaddr_in sa;
  inet_pton(AF_INET, "192.168.100.1", &(sa.sin_addr));
}

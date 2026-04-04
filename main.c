#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  struct sockaddr_in address;
  inet_pton(AF_INET, "192.168.100.1", &(address.sin_addr));
}

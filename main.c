#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main() {
  // Getting the current date time
  time_t now = time(NULL);
  printf("The current time is: %s", ctime(&now));

  struct sockaddr_in address;
  inet_pton(AF_INET, "192.168.100.1", &(address.sin_addr));
}

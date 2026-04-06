#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
// #define PORT 80

int main() {
  FILE *fptr;
  char buffer[80];
  time_t raw_time;
  struct tm *time_info;

  // The State Machine:
  // 1. Get current time
  time(&raw_time);

  // 2. Convert to local time
  time_info = localtime(&raw_time);

  // 3. Format the time into a string
  // refer to strftime documentation
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);

  // 4. Open file for writing ("w" overwrites, "a" appends)
  fptr = fopen(".bible_cache", "w");
  if (fptr == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  // 5. Write the formatted string to the file
  fprintf(fptr, "Last Successfully Ran: %s", buffer);

  // 6. Close the file
  fclose(fptr);

  printf("Date and time successfully stored in .bible_cache\n");

  // Networking:
  int getaddrinfo(
      const char *node,    // e.g. "https://bible-api.com/ or IP address"
      const char *service, // e.g. "http" or port number
      const struct addrinfo *hints, struct addrinfo **res);

  // Client:
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo; // Will point to results

  memset(&hints, 0, sizeof(hints)); // Make sure the struct is empty
  hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP Stream Sockets

  // Get ready to Connect
  status = getaddrinfo("https://bible-api.com/", "80", &hints, &servinfo);

  printf("status: %d\n", status);

  // 1. Create a socket
  int socket(int domain, int type, int protocol);
  struct sockaddr_in address;
  // 192.42.93.30
  inet_pton(AF_INET, "192.168.100.1", &(address.sin_addr));
}

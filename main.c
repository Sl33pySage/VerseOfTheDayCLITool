#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
  // int getaddrinfo(
  //    const char *node,    // e.g. "https://bible-api.com/ or IP address"
  //    const char *service, // e.g. "http" or port number
  //    const struct addrinfo *hints, struct addrinfo **res);

  // Client:
>>>>>>> main
  int status;
  struct addrinfo hints, *res, *p;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("bible-api.com", "80", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  };

  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;
    // Extract IP based on address family
    if (p->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }

<<<<<<< HEAD
    // Convert the IP to a string and print
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf(" %s: %s\n", ipver, ipstr);
  }
  freeaddrinfo(res);
  return 0;

=======
  // Client:
>>>>>>> main
  int status;
  struct addrinfo hints, *res, *p;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("bible-api.com", "80", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  };

  for (p = res; p != NULL; p = p->ai_next) {
    printf("PF: %p\n", p->ai_addr);
  }
  freeaddrinfo(res);
  return 0;

<<<<<<< HEAD
>>>>>>> main
  /*
 int getaddrinfo(const char *node,    // e.g., "www.example.com" or IP Address
                 const char *service, // e.g., "http" or port number "80"
                 const struct addrinfo *hints, // criteria for results
                 struct addrinfo **res);       // pointer to the resulting list
 // struct addrinfo hints, *res;
 // struct sockaddr_in address;
 struct addrinfo {
   int ai_flags;
   int ai_family;
   int ai_socktype;
   int ai_protocol;
   socklen_t ai_addrlen;
   struct sockaddr *ai_addr;
   char *ai_canonname;
   struct addrinfo *ai_next;
 };
<<<<<<< HEAD
=======
*/
=======
  // 1. Create a socket
  // int socket(int domain, int type, int protocol);
  // struct sockaddr_in address;
>>>>>>> main
>>>>>>> main
  // 192.42.93.30
  // inet_pton(AF_INET, "192.168.100.1", &(address.sin_addr));
*/
}

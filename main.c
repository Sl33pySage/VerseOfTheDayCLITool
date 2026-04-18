#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
// #include <stdlib.h>
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
  int status;
  struct addrinfo hints, *res, *p;
  int sockfd;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("labs.bible.org", "80", &hints, &res)) != 0) {
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

    // Convert the IP to a string and print
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("ipver: %s\n ipstr: %s\n", ipver, ipstr);
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (connect(sockfd, res->ai_addr, res->ai_addrlen) != -1) {
    char *msg =
        "GET /api/?passage=random&type=text HTTP/1.1\r\nUser-Agent: "
        "Mozilla/5.0 "
        "(Windows NT 10.0; Win64; x64) AppleWebKit/537.36\r\nAccept-Language: "
        "en-US\r\nAccept: "
        "text/html,application/xtml+xml,application/xml;q=0.9,*/"
        "*;q=0.8\r\nAccept-Encoding: gzip,deflate\r\nHost: "
        "labs.bible.org\r\nConnection: Keep-Alive\r\n";
    int len, bytes_sent;

    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);
    // strtol(&msg, &bytes_sent, sizeof len);
    // void parse_url(char *url, char**hostname, char **port, char**, path) {
    // printf("URL: %s\n", url);
    // }
    printf("Connected!\n");
    printf("msg: %s\n", msg);
    printf("bytes_sent: %d\n", bytes_sent);
  };
  char buf[2048];
  int n;
  while ((n = recv(sockfd, buf, sizeof buf - 1, 0)) > 0) {
    buf[n] = '\0';
    printf("buf: %s\n", buf);
  }
  close(sockfd);

  freeaddrinfo(res);
  return 0;
}

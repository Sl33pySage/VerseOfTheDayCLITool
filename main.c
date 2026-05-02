#include "cJSON.c"
#include "cJSON.h"
#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
// #include <stdlib.h>
#include <curl/curl.h>
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
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  printf("Connecting...\n");
  if (connect(sockfd, res->ai_addr, res->ai_addrlen) != -1) {
    char *msg = "GET /api/?passage=random&type=text HTTP/1.1\r\n\r\n";
    int len, bytes_sent;
    len = strlen(msg);

    /* Send Data (request) */
    bytes_sent = send(sockfd, msg, len, 0);
    if (bytes_sent == -1) {
      perror("ERROR SENDING...");
      return -1;
    } else {
      printf("Connected!\n");
      printf("%s\n", msg);
      printf("%d\n", bytes_sent);
      // printf("res->ai_addr: %p\n", (struct sockaddr_in *)&res->ai_addr);

      /* recieve the response */
      char message[1024], response[4096];
      int recieved, total;
      memset(response, 0, sizeof(response));
      total = sizeof(response) - 1;
      recieved = 0;

      int bytes_recieved = recv(sockfd, response, 4096, 0);
      if (bytes_recieved < 1) {
        printf("Connection closed by peer.\n");
      }

      // Libcurl Stuff
      curl_global_init(CURL_GLOBAL_ALL);
      CURL *handle = curl_easy_init();
      if (handle) {

        // cJSON structure:
        typedef struct cJSON {
          struct cJSON *next;
          struct cJSON *prev;
          struct cJSON *child;
          int type;
          char *valuestring;
          // writing valueint is DEPRECATED, use
          // CJSON_SetNumberValue instead
          int valueint;
          double valuedouble;
          char *string;
        } cJSON;

        CURLcode result;
        curl_easy_setopt(handle, CURLOPT_URL,
                         "https://bible-api.com/data/kjv/random");
        result = curl_easy_perform(handle);
        curl_easy_cleanup(handle);
      }
    }
  }

  close(sockfd);
  freeaddrinfo(res);
  return 0;
}

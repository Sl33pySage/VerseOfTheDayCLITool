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
    // inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    // printf("ipver: %s\n ipstr: %s\n", ipver, ipstr);
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (connect(sockfd, res->ai_addr, res->ai_addrlen) != -1) {
    char *msg = "GET /api/?passage=random&type=text HTTP/1.1\r\n\r\n";
    int len, bytes_sent;

    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);

    if (bytes_sent == -1) {
      perror("ERROR SENDING...");
      return -1;
    } else {
      printf("Connected!\n");
      printf("%s\n", msg);
      printf("%d\n", bytes_sent);
      // printf("res->ai_addr: %p\n", (struct sockaddr_in *)&res->ai_addr);
    }
  }
  close(sockfd);

  freeaddrinfo(res);
  return 0;
}

// strtol(&msg, &bytes_sent, sizeof len);

/*  Parsing a URL
 *  We will write a C function to parse a given URL.
 *  The function takes as input a URL. and it returns as output the
 * hosename, the port number, and the document path. To avoid needing to do
 * manual memory management, the outputs are returned as pointers to
 * specific parts of the input URL. The input URL is modified with
 * terminting null pointers as required.
 *
 *  Our function beings by printing the input URL. This is useful for
 * debugging.The code for that is as follows:
 */
// void parse_url(char *url, char **hostname, char **port, char **, path) {
//  printf("URL: %s\n", url);

// The function then attempts to find :// in the URL. If found, it reads
// in the first port of the URL. as a protocol. Our program only supports
// HTTP. If the given protocol is not HTTP, then an error is returned. The
// code for partsing the protocol is as follows:
// char *p;
// p = strstr(url, "://");
// char *protocol = 0;
// if (p) {
// protocol = url;
//*p = 0;
// p += 3;
//} else {
// p = url;
//}
// if (protocol) {
// if (strcmp(protocol, "http")) {
//  fprintf(stderr, "Unkown protocol '/%s'. Only 'http' is supported.\n",
//         protocol);
// exit();
// }
//}
/*
 * In the preceding code, a character pointer,p, is declared. protocol is
 * also declared and set to 0 to indicate that no protocol has been found.
 * strstr() is called to search for :// in the URL. If it is not found,
 * then protocol is set to the beginning of the URL, which contains the
 * protocol. p is set to one after ://, which should be where the hostname
 * begins.
 *
 * if protocol was set the coe then checks that it points to the text
 * http.
 *
 * At this point in the code, p points to the beginning of the hostname.
 * The code can save the hostname into the return variable, hostname. The
 * code must then scan for the end of the hostname by looking for the
 * first colon, slash, or hash. The code for this is as follows:
 */

//*hostname = p;
// while (*p && *p != ':' && *p != '/' && *p != '#')
// ++p;
// Once p has advanced to the end of the hostname, we must check whether a
// port number was found. A port number starts with a colon. If a pot
// number is found, our code returns it in the port variable; otherwise, a
// default port number of 80 is returned. The code to check for a port
// number is as follows:
// port = "80";
// if (*p == ':') {
// *p++ = 0;
//  *port = p;
// }
// while (*p && *p != '/' && *p != '#')
// ++p;

// After the port number, p points to the document path. The function
// returns this part of the URL. in the path variable. Note that our
// function omits the first / in the path. This is for simplicity because
// it allows us to avoid allocating any memory. All document paths start
// with /, so the function caller can easily prepend that when the HTTP
// request is constructed. The code to set the path variable is as
// follows:
//*path = p;
// if (*p == '/')
// *path = p + 1;
//*p = 0;
// The code then attempts to find a hash, if it exists. If it does exist,
// it is overwritten with the terminating null character. This is because
// the hash is never sent to the web server and is ignored by our HTTP
// client. The code that advances to the hash is as follows:
// while (*p && *p != '#')
// ++p;
// if (*p == '#')
//  *p = 0;
// Our function has now parsed out the hostname, port number, and document
// path. It then prints out these values for debugging purposes and
// returns. The final code for the parse_url() function is as follows:
// printf("hostname: %s\n", *hostname);
// printf("port: %s\n", *port);
// printf("path: %s\n", *path);
//}
// Now that we have code to parse a URL, we are one step closer to building
// an entire HTTP client.

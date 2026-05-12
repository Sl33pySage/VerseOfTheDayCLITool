#include "cJSON.c"
#include "cJSON.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <curl/curl.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// State Machine Function
void state_machine() {
  FILE *fptr;
  char buffer[80];
  time_t raw_time;
  struct tm *time_info;

  // The State Machine:
  // 1. Get current time
  time(&raw_time);

  // 2. Convert to local time
  time_info = localtime(&raw_time);

  // 3. Format the time into a string, refer to strftime documentation
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);

  // 4. Open file for writing ("w" overwrites, "a" appends)
  fptr = fopen(".bible_cache", "w");
  if (fptr == NULL) {
    printf("Error opening file!\n");
    return;
  }

  // 5. Write the formatted string to the file
  fprintf(fptr, "Last Successfully Ran: %s", buffer);

  // 6. Close the file
  fclose(fptr);
  printf("Date and time successfully stored in .bible_cache: 'Last Sucessfully "
         "Ran: %s'\n",
         buffer);
}

void lib_curl_func() {
  // Libcurl Stuff
  curl_global_init(CURL_GLOBAL_ALL);
  CURL *handle = curl_easy_init();
  if (handle) {
    CURLcode result;
    curl_easy_setopt(handle, CURLOPT_URL,
                     "https://bible-api.com/data/kjv/random");
    result = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
  }
}

struct memory {
  char *response;
  size_t size;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp) {
  size_t realsize = nmemb;
  struct memory *mem = (struct memory *)clientp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if (!ptr) {
    return 0; // Out of memory.
  }

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
  return realsize;
}

struct memory chunk = {0};
CURLcode result;

void api_call() {
  CURL *curl = curl_easy_init();
  if (curl) {

    // Getting the url
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://bible-api.com/data/kjv/random");

    // Send all data to this function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

    // We pass our 'chunk' struct to the callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Send a request
    result = curl_easy_perform(curl);

    cJSON *jsond = cJSON_Parse(chunk.response);
    cJSON *random_verse =
        cJSON_GetObjectItemCaseSensitive(jsond->child, "random_verse");

    const char *Bible_Data = cJSON_Print(jsond);
    // printf("Bible_Data: %s\n", Bible_Data);

    const char *translation_data = cJSON_Print(jsond->child);
    printf("translation_data: %s\n", translation_data);

    // Translation Data Object
    cJSON *Version =
        cJSON_GetObjectItemCaseSensitive(jsond->child->next, "name");
    const char *version = cJSON_Print(Version);

    // Random Verse Object Data
    const char *random_verse_data = cJSON_Print(jsond->child->next);
    // printf("random_verse_data: %s\n", random_verse_data);

    // Book Name Data
    cJSON *book_Name =
        cJSON_GetObjectItemCaseSensitive(jsond->child->next, "book");
    const char *book = cJSON_Print(book_Name);

    // Chapter Number Data
    cJSON *chapter_Number =
        cJSON_GetObjectItemCaseSensitive(jsond->child->next, "chapter");
    const char *chapter_num = cJSON_Print(chapter_Number);

    // Verse Number Data
    cJSON *number =
        cJSON_GetObjectItemCaseSensitive(jsond->child->next, "verse");
    const char *num = cJSON_Print(number);

    // Verse Text String Data
    cJSON *text = cJSON_GetObjectItemCaseSensitive(jsond->child->next, "text");
    const char *verse = cJSON_Print(text);

    printf("%s\n(%s) %s : %s\n%s\n", book, version, chapter_num, num,
           text->valuestring);

    cJSON *string = cJSON_CreateString(verse);
    const char *s = cJSON_Print(string);

    // fprintf(s, "s: %c");

    // Remember to free the buffer
    free(chunk.response);
    curl_easy_cleanup(curl);
  }
}

int main(void) {
  state_machine();
  api_call();
}

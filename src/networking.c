#include "networking.h"
#include <curl/curl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct curl_string {
    char *ptr;
    size_t len;
};


static size_t remote_writeto_string(void* ptr, size_t size, size_t nmemb, struct curl_string* s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    assert(s->ptr != NULL);
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

char* curl_request(const char* uri)
{
    CURL* curl;
    CURLcode res;
    struct curl_string str;
    str.len = 0;
    str.ptr = strdup("");

    curl =  curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, remote_writeto_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) {
        // TODO: Get error
        free(str.ptr);
        return NULL;
    }
    
    return str.ptr;
}


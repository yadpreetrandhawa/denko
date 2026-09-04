#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

typedef struct {
    char method[16];
    char path[256];
} HttpRequest;

// parse raw sockets -> HttpRequest struct
int http_parse_request(const char* raw_req, HttpRequest* req);

int http_send_all(int client_socket, const void* data, size_t length);

// file serving
void http_handle_request(int client_socket, const HttpRequest* req);

#endif
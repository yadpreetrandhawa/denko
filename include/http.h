#ifndef HTTP_H
#define HTTP_H

typedef struct {
    char method[16];
    char path[256];
} HttpRequest;

// parse raw sockets -> HttpRequest struct
int http_parse_request(const char* raw_req, HttpRequest* req);

// file serving
void http_handle_request(int client_socket, const HttpRequest* req);

#endif
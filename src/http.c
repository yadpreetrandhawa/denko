#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "text/javascript";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
    return "text/plain";
}

int http_parse_request(const char* raw_req, HttpRequest* req) {
    if (!raw_req || !req) return -1;
    
    if (sscanf(raw_req, "%15s %255s", req->method, req->path) != 2) {
        return -1;
    }
    
    // default root route
    if (strcmp(req->path, "/") == 0) {
        strcpy(req->path, "/index.html");
    }
    return 0;
}

void http_handle_request(int client_socket, const HttpRequest* req) {
    // security check
    if (strstr(req->path, "..") != NULL) {
        char err_403[] = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden!";
        send(client_socket, err_403, strlen(err_403), 0);
        return;
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "public%s", req->path);

    FILE* file = fopen(file_path, "rb");
    if (!file) {
        char err_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\n<h1>404 Not Found</h1>";
        send(client_socket, err_404, strlen(err_404), 0);
        return;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // response header
    char header[256];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n\r\n",
        get_mime_type(file_path), size);
    send(client_socket, header, hlen, 0);

    // stream body
    char chunk[1024];
    size_t bytes;
    while ((bytes = fread(chunk, 1, sizeof(chunk), file)) > 0) {
        send(client_socket, chunk, bytes, 0);
    }

    fclose(file);
}
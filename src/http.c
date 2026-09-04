#include "http.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

int http_send_all(int client_socket, const void* data, size_t length) {
    const char* buffer = data;
    size_t sent = 0;

    while (sent < length) {
        ssize_t bytes = send(client_socket, buffer + sent, length - sent, 0);
        if (bytes <= 0) return -1;
        sent += (size_t)bytes;
    }

    return 0;
}

int http_parse_request(const char* raw_req, HttpRequest* req) {
    if (!raw_req || !req) return -1;

    char protocol[16];
    if (sscanf(raw_req, "%15s %255s %15s", req->method, req->path, protocol) != 3 ||
        strncmp(protocol, "HTTP/", 5) != 0) {
        return -1;
    }
    
    // default root route
    if (strcmp(req->path, "/") == 0) {
        strcpy(req->path, "/index.html");
    }
    return 0;
}

static int open_document(const char* request_path) {
    if (!request_path || request_path[0] != '/') {
        errno = EACCES;
        return -1;
    }

    char relative_path[256];
    if (snprintf(relative_path, sizeof(relative_path), "%s", request_path + 1) >=
        (int)sizeof(relative_path)) {
        errno = EACCES;
        return -1;
    }

    int directory_fd = open("public", O_RDONLY | O_DIRECTORY | O_NOFOLLOW);
    if (directory_fd < 0) return -1;

    char* save_pointer;
    char* component = strtok_r(relative_path, "/", &save_pointer);
    if (!component) {
        close(directory_fd);
        errno = EACCES;
        return -1;
    }

    while (component) {
        char* next_component = strtok_r(NULL, "/", &save_pointer);
        if (strcmp(component, ".") == 0 || strcmp(component, "..") == 0) {
            close(directory_fd);
            errno = EACCES;
            return -1;
        }

        if (next_component) {
            int next_directory_fd = openat(directory_fd, component,
                O_RDONLY | O_DIRECTORY | O_NOFOLLOW);
            close(directory_fd);
            if (next_directory_fd < 0) return -1;
            directory_fd = next_directory_fd;
        } else {
            int file_fd = openat(directory_fd, component, O_RDONLY | O_NOFOLLOW);
            close(directory_fd);
            return file_fd;
        }

        component = next_component;
    }

    close(directory_fd);
    errno = EACCES;
    return -1;
}

void http_handle_request(int client_socket, const HttpRequest* req) {
    if (strcmp(req->method, "GET") != 0) {
        char err_405[] = "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\nContent-Length: 18\r\nConnection: close\r\n\r\nMethod Not Allowed";
        http_send_all(client_socket, err_405, strlen(err_405));
        return;
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "public%s", req->path);

    int file_fd = open_document(req->path);
    if (file_fd < 0) {
        if (errno == EACCES || errno == ELOOP) {
            char err_403[] = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden!";
            http_send_all(client_socket, err_403, strlen(err_403));
            return;
        }

        char err_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\n<h1>404 Not Found</h1>";
        http_send_all(client_socket, err_404, strlen(err_404));
        return;
    }

    FILE* file = fdopen(file_fd, "rb");
    if (!file) {
        close(file_fd);
        char err_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\n<h1>404 Not Found</h1>";
        http_send_all(client_socket, err_404, strlen(err_404));
        return;
    }

    struct stat file_info;
    if (fstat(file_fd, &file_info) < 0 || !S_ISREG(file_info.st_mode)) {
        fclose(file);
        char err_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\n<h1>404 Not Found</h1>";
        http_send_all(client_socket, err_404, strlen(err_404));
        return;
    }

    // get file size
    long size = file_info.st_size;

    // response header
    char header[256];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n\r\n",
        get_mime_type(file_path), size);
    http_send_all(client_socket, header, (size_t)hlen);

    // stream body
    char chunk[1024];
    size_t bytes;
    while ((bytes = fread(chunk, 1, sizeof(chunk), file)) > 0) {
        if (http_send_all(client_socket, chunk, bytes) != 0) break;
    }

    fclose(file);
}
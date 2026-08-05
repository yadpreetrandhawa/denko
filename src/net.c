#include "net.h"
#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[2048] = {0};
    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read > 0) {
        HttpRequest req;
        if (http_parse_request(buffer, &req) == 0) {
            http_handle_request(client_socket, &req);
        }
    }

    close(client_socket);
    return NULL;
}

int openPort(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket error");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind error");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 64) < 0) {
        perror("Listen error");
        close(server_fd);
        return -1;
    }

    printf("[denko] Listening: %d\n", port);
    printf("[denko] http://localhost:%d\n", port);

    // connection loop
    while (1) {
        socklen_t len = sizeof(addr);
        int client_socket = accept(server_fd, (struct sockaddr *)&addr, &len);
        if (client_socket >= 0)
        {
            int* new_sock = malloc(sizeof(int));
            *new_sock = client_socket;
        
            // char client_ip[INET_ADDRSTRLEN];
            // binary_ip -> string
            // inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
            // int client_port = ntohs(addr.sin_port);

            // printf("[denko] %s:%d\n", client_ip, client_port);

            pthread_t thread;
            if (pthread_create(&thread, NULL, handle_client, new_sock) == 0) {
                pthread_detach(thread);
            } else {
                free(new_sock);
                close(client_socket);
            }
        }

    }

    close(server_fd);
    return 0;
}
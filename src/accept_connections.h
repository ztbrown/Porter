#ifndef ACCEPT_CONNECTIONS_H
#define ACCEPT_CONNECTIONS_H

#include <sys/socket.h>

void start_listener(int current_socket);

void accept_connection(int *current_socket, int *connecting_socket, socklen_t *addr_size, struct sockaddr_storage *connector);

void handle_connection(int current_socket);

int receive(int connecting_socket);

#endif // ACCEPT_CONNECTIONS_H

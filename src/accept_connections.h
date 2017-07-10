#ifndef ACCEPT_CONNECTIONS_H
#define ACCEPT_CONNECTIONS_H

#include <sys/socket.h>

void start_listener(int current_socket);

void accept_connection(int *current_socket, int *connecting_socket, socklen_t *addr_size, struct sockaddr_storage *connector);

#endif // ACCEPT_CONNECTIONS_H

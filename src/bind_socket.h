#ifndef BIND_SOCKET_H
#define BIND_SOCKET_H

#include <netdb.h>

void bind_socket(int current_socket, struct addrinfo *addr_info);

#endif // BIND_SOCKET_H

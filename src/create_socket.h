#ifndef CREATE_SOCKET_H
#define CREATE_SOCKET_H

#include <netinet/in.h>
#include <netdb.h>

//Create a socket and assign current_socket to the descriptor

void get_network_info(struct addrinfo **res, int port);
void create_socket(int *current_socket, struct addrinfo *address_info);

#endif // CREATE_SOCKET_H

#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

#include "create_socket.h"

void get_network_info(struct addrinfo **res, int port)
{
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char str_port[16];
    snprintf(str_port, sizeof(str_port), "%ld", (long int)port);

    getaddrinfo(NULL, str_port, &hints, res);
}

void create_socket(int *current_socket, struct addrinfo *address_info)
{
	*current_socket = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);

	if ( *current_socket == -1 )
    {
		perror("Create socket");
		exit(-1);
	}
}

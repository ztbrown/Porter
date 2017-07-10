#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "accept_connections.h"
#include "http_utils.h"

void accept_connection(int *current_socket, int *connecting_socket, socklen_t *addr_size, struct sockaddr_storage *connector)
{
    *addr_size = sizeof(*connector);

    *connecting_socket = accept(*current_socket, (struct sockaddr *)connector, addr_size);

    if ( *connecting_socket < 0 )
    {
        perror("Accepting sockets");
        exit(-1);
    }
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "accept_connections.h"
#include "handle_connections.h"
#include "http_utils.h"

#define MAX_CONNECTIONS 10

void start_listener(int current_socket)
{
    if ( listen(current_socket, MAX_CONNECTIONS) < 0 )
    {
        perror("Listen on port");
        exit(-1);
    }
}

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

void handle_connection(int connecting_socket)
{
    if (!fork())
    {
        if (receive((int)connecting_socket) < 0)
        {
            perror("Receive");
            exit(-1);
        }
        close(connecting_socket);
        exit(EXIT_SUCCESS);
    }

    close(connecting_socket);
}

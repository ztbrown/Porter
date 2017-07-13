#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "accept_connections.h"
#include "http_utils.h"

#define BUFFER_SIZE 512

int receive(int connecting_socket)
{
    int msgLen = 0;
    char buffer[BUFFER_SIZE];

    memset (buffer,'\0', BUFFER_SIZE);

    if ((msgLen = recv(connecting_socket, buffer, BUFFER_SIZE, 0)) == -1)
    {
        printf("Error handling incoming request");
        return -1;
    }

    int request = getRequestType(buffer);

    if ( request == 1 )				// GET
    {
        handleHttpGET(buffer, connecting_socket);
    }
    else if ( request == 2 )		// HEAD
    {
        // SendHeader();
    }
    else if ( request == 0 )		// POST
    {
        sendString("501 Not Implemented\n", connecting_socket);
    }
    else							// GARBAGE
    {
        sendString("400 Bad Request\n", connecting_socket);
    }

    return 1;
}

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
    // --- Workflow --- //
    // 1. Receive ( recv() ) the GET / HEAD
    // 2. Process the request and see if the file exists
    // 3. Read the file content
    // 4. Send out with correct mine and http 1.1

    if (receive((int)connecting_socket) < 0)
    {
        perror("Receive");
        exit(-1);
    }

    close(connecting_socket);
}

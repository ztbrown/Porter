#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "receive_connections.h"
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


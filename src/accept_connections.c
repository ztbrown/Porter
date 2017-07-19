#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include "accept_connections.h"
#include "receive_connections.h"
#include "http_utils.h"

#define MAX_CONNECTIONS 10

struct sigaction sa;

static void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

static void setup_sigchld_handler()
{
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void start_listener(int current_socket)
{
    if ( listen(current_socket, MAX_CONNECTIONS) < 0 )
    {
        perror("Listen on port");
        exit(-1);
    }

    setup_sigchld_handler();
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

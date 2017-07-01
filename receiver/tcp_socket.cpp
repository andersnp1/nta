#include "tcp_socket.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace NTA;

TCPSocket::TCPSocket(int port) : Socket(port)
{
}

TCPSocket::~TCPSocket()
{
}

int TCPSocket::initialise()
{
    printf("Initialising TCP socket\n");
    this->socketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketFD == -1)
    {
        perror("Unable to create TCP socket");
        return EXIT_FAILURE;
    }

    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketFD, (struct sockaddr *) &sa, sizeof sa) == -1)
    {
        perror("Unable to bind TCP socket");
        return EXIT_FAILURE;
    }

    if(listen(socketFD, 100) == -1)
    {
        perror("Unable to start listening on TCP socket");
        return EXIT_FAILURE;
    }

    return 0;
}

void TCPSocket::startListen()
{
    printf("Starting to listen on TCP port %d\n", port);
    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int connectionFD = accept(socketFD, NULL, NULL);

        if(connectionFD < 0)
        {
            perror("Failed to accept incoming TCP connection");
            continue;
        }

        int bytesRead = read(connectionFD, buffer, BUFFER_SIZE);

        printf("Received %d bytes through TCP\n%s", bytesRead, buffer);

        if(shutdown(connectionFD, SHUT_RDWR) == -1)
        {
            perror("Failed to shut down TCP connection");
        }

        // Finally close connection
        close(connectionFD);
    }
}
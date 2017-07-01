#include "udp_socket.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace NTA;

UDPSocket::UDPSocket(int port) : Socket(port)
{}

UDPSocket::~UDPSocket()
{}

int UDPSocket::initialise()
{
    printf("Initialising UDP socket\n");
    socketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(socketFD == -1)
    {
        perror("Failed to create UDP socket");
        return EXIT_FAILURE;
    }

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketFD, (struct sockaddr*) &sa, sizeof sa) == -1)
    {
        perror("Unable to bind UDP socket");
        return EXIT_FAILURE;
    }

    return 0;
}

void UDPSocket::startListen()
{
    printf("Starting to listen on UDP port %d\n", port);
    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = recv(socketFD, buffer, BUFFER_SIZE, 0);
        if(bytesRead < 0)
        {
            perror("Error when receiving data over UDP");
        }

        printf("Received %d bytes through UDP\n%s", bytesRead, buffer);
    }
}
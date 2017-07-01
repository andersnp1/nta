/// This is to be used on the receiver side to receive data and echo it
/// This is mostly for proof of concept
/// Now, why is this C++, not just C? Sure such a simple program doesn't need to
/// be C++.
/// Well, I miss C++; what of it?!
#include "socket.hpp"
#include "tcp_socket.hpp"
#include "udp_socket.hpp"

#include <cstdio>
#include <cstdlib>
#include <thread>

using namespace NTA;
using namespace std;

void threadHandler(Socket* socket)
{
    socket->startListen();
}

int main(int argc, const char * argv[])
{
    if(argc < 2)
    {
        printf("A port number must be passed\n");
        exit(EXIT_SUCCESS);
    }
    
    int port = atoi(argv[1]);

    if(port == 0 || port < 0 || port > 65536)
    {
        printf("Port must be a positive integer\n");
        exit(EXIT_SUCCESS);
    }

    Socket* tcpSock = new TCPSocket(port);
    Socket* udpSock = new UDPSocket(port);
    if(tcpSock->initialise() == EXIT_FAILURE)
    {
        delete tcpSock;
    }
    if(udpSock->initialise() == EXIT_FAILURE)
    {
        delete udpSock;
    }

    thread tcpThread (threadHandler, tcpSock);
    thread udpThread (threadHandler, udpSock);

    tcpThread.join();
    udpThread.join();

    delete tcpSock;
    delete udpSock;
    return EXIT_SUCCESS;
}
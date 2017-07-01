#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "socket.hpp"

namespace NTA
{
    class TCPSocket : public Socket
    {
    public:
        TCPSocket(int port);
        ~TCPSocket();
        int initialise();
        void startListen();
    protected:
        int socketFD;
    private:
    };
}

#endif // !TCP_SOCKET_H
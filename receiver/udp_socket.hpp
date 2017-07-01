#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "socket.hpp"

namespace NTA
{
    class UDPSocket : public Socket
    {
    public:
        UDPSocket(int port);
        ~UDPSocket();
        int initialise();
        void startListen();
    protected:
    private:
    };
}

#endif //! UDP_SOCKET_H
#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

#define BUFFER_SIZE 2000

namespace NTA
{
    class Socket
    {
    public:
        Socket(int port);
        virtual ~Socket();
        virtual int initialise() = 0;
        virtual void startListen() = 0;

    protected:
        int port;
        int socketFD;
        struct sockaddr_in sa;
        void* buffer;
    private:
    };
}

#endif // !SOCKET_H
#include "socket.hpp"

#include <unistd.h>
#include <cstdint>
#include <cstdlib>

using namespace NTA;

Socket::Socket(int port)
{
    this->port = port;
    this->buffer = static_cast<void*>(new uint8_t[BUFFER_SIZE]);
}

Socket::~Socket()
{
    close(this->socketFD);
    free(buffer);
}
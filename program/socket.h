#ifndef __SOCKET_H__
#define __SOCKET_H__

int initialize(void);
int initialise_udp(void);
int initialise_tcp(void);
void closeSocket(void);
int sendPacket(void);
int receivePacket(char *msg);

#endif

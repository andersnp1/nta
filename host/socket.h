#ifndef SOCKET_H
#define SOCKET_H

int socket_initialise(void);
void socket_broadcast(void);
int socket_send_command(int, const char*, int);

#endif // SOCKET_H
#include "socket.h"
#include "parameters.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

//#include <arpa/inet.h>

static struct sockaddr_in sa;
static int sock;

int socket_initialise(void)
{
    int broadcastEnable = 1;
    int res = 0;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1)
    {
        perror("Unable to open socket");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    res = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if(res != 0)
    {
        perror("Unable to set socket as broadcast");
        close(sock);
        exit(EXIT_FAILURE);
    }

    /*res = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, interface, sizeof interface);
    if(res == -1)
    {
        printf("Failed to assiciate with interface %s; errno: %d\n", interface, errno);
        close(sock);
        exit(EXIT_FAILURE);
    }*/

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("10.193.255.255");
    sa.sin_port = htons(5557);

    return 0;
}

void socket_broadcast(void)
{
    char value[15];
    if(protocol[0] != '\0')
    {
        socket_send_command(PROTOCOL, protocol, sizeof protocol);
    }
    if(periodicity > 0)
    {
        sprintf(value, "%d", periodicity);
        socket_send_command(PERIODICITY, value, sizeof value);
    }
    if(packet_size > 0)
    {
        sprintf(value, "%d", packet_size);
        socket_send_command(PACKET_SIZE, value, sizeof value);
    }
}

int socket_send_command(int command, const char* value, int value_length)
{
    int res = 0;
    char buff[value_length+3];
    printf("Sending %s\n", value);

    sprintf(buff, "%00d:%s", command, value);

    res = sendto(sock, buff, value_length+3, 0, (struct sockaddr *)&sa, sizeof sa);
    if(res == -1)
    {
        perror("Unable to send command");
    }

    return 0;
}
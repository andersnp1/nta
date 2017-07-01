#include "socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>          // getenv
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_MESSAGE 1024

static struct sockaddr_in serv_adr;
static int socket_udp, socket_tcp;
int packet_len;
static char container_id[68] = {0};
static size_t len_container_id = 0;
short use_udp;

static void readEnvironment(char *gateway_ip, char *container_id, int *middleware_port)
{
    if(!getenv("RADIO_GW")) printf("Gateway was not found\n");
    if(!getenv("CONTAINER_ID")) printf("Container id was not found\n");
    if(!getenv("PORT")) printf("Middleware port was not found\n");

    strcpy(gateway_ip, getenv("RADIO_GW"));
    strcpy(container_id, getenv("CONTAINER_ID"));
    *middleware_port = atoi(getenv("PORT"));
}

int initialize(void)
{
    printf("Initializing package sender\n");

    char address[16] = {0};
    int port = 0;

    readEnvironment(address, container_id, &port);

    printf("Container strlen: %d, address strlen: %d\n", strlen(container_id), strlen(address));

    printf("\nAddress: %s\nContainer ID: %s\nPort: %d\n\n", address, container_id, port);

    if(initialise_udp()) return 1;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(port);
    serv_adr.sin_addr.s_addr = inet_addr("CHANGE_ME");

    // Set default packet size
    len_container_id = strlen(container_id);
    packet_len = 200;
    use_udp = 1;

    return 0;
}

int initialise_udp(void)
{
    socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(socket_udp < 0) {
        perror("Unable to open UDP socket");
        close(socket_udp);
        return 1;
    }
    return 0;
}

int initialise_tcp(void)
{
    socket_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socket_tcp < 0) {
        perror("Unable to open TCP socket");
        close(socket_tcp);
        return 1;
    }
    return 0;
}

int sendPacket(void)
{
    static char packet[65508];
    int current_len, max_len = packet_len - 3;

    memset(packet, 0, sizeof packet);

    strcat(packet, "<");

    // Copy the entire container ID into packet as many times as possible
    for(current_len = 1; current_len < max_len - (int) len_container_id; current_len += (signed) len_container_id)
    {
        strcat(packet, container_id);
    }

    // From here we assume there is not enough space remaining in the packet
    // to copy the entire container ID again, so we only copy part of it
    // making space for a final ">\n"
    memcpy(&packet[current_len], container_id, 1 + max_len - current_len);

    strcat(packet, ">\n");

    printf("Packet: %sLength: %d\n", packet, strlen(packet));

    //assert(msg && "Message to send was null.");

    int adr_len = sizeof(serv_adr);

    if (use_udp)
    {
        return sendto(socket_udp, packet, strlen(packet), 0,
                (const struct sockaddr *)&serv_adr, adr_len);
    }
    else
    {
        initialise_tcp();
        // Establish TCP connection
        if(connect(socket_tcp, (struct sockaddr*) &serv_adr, sizeof serv_adr))
        {
            perror("Unable to connect to recepient");
            return -1;
        }

        // Send message
        write(socket_tcp, packet, strlen(packet));

        // Close connection
        if(close(socket_tcp))
        {
            perror("Unable to close TCP connection");
            return 1;
        }

        return 0;
    }
}

int receivePacket(char *msg)
{
    socklen_t adr_len = sizeof(serv_adr);

    return recvfrom(socket_udp, msg, MAX_MESSAGE, 0,
            (struct sockaddr*)&serv_adr, &adr_len);
}

void closeSocket(void)
{
    close(socket_udp);
    close(socket_tcp);
}

#include "control.h"
#include "parameters.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>          // getenv
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

static struct sockaddr_in sa;
static int socket_descriptor;

static void readEnvironment(char *control_dev, char *control_subnet, uint16_t* port)
{
    if(!getenv("CONTROL_DEV")) printf("Control interface was not found\n");
    if(!getenv("CONTROL_PREFIX")) printf("Control prefix was not found\n");
    if(!getenv("PORT")) printf("Middleware port was not found\n");

    strcpy(control_dev, getenv("CONTROL_DEV"));
    strcpy(control_subnet, getenv("CONTROL_PREFIX"));
    *port = atoi(getenv("PORT"))+1;
}

int control_initialize_listener(void)
{
    printf("Initialising control listener\n");
    uint16_t port = 0;
    char interface_name[10];
    char control_prefix[16];
    char listen_addr[16];

    readEnvironment(interface_name, control_prefix, &port);
    sprintf(listen_addr, "%s.255.255", control_prefix);
   

    printf("\nInterface name: %s, Listen address: %s, \nPort: %d\n\n", interface_name, listen_addr, port);

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Check for error
    if(socket_descriptor == -1)
    {
        perror("Failed to create socket\n");
        close(socket_descriptor);
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(listen_addr);

    if(setsockopt(socket_descriptor, SOL_SOCKET, SO_BINDTODEVICE, interface_name, strlen(interface_name)) == -1)
    {
        printf("Failed to associate with interface %s\n", interface_name);
    }

    if(bind(socket_descriptor, (struct sockaddr*) &sa, sizeof(sa)) == -1)
    {
        perror("Failed to bind socket\n");
        close(socket_descriptor);
        return -1;
    }

    #ifdef false
    if(listen(socket_descriptor, 10) == -1)
    {
        perror("Failed to set up socket for listening");
        close(socket_descriptor);
        return -1;
    }
    #endif

    return 0;
}

void control_listener(void* args)
{
    (void) args;

    static char msg_buffer[1500];

    while(1)
    {
        char code[4] = "";
        char *value;
        parameter_t param;
        printf("Listening\n");
        memset(msg_buffer, 0, sizeof(msg_buffer));
        int length = recv(socket_descriptor, msg_buffer, sizeof(msg_buffer), 0);

        printf("Received %s with length %d on control network\n", msg_buffer, length);
        
        // Get first occurence of ":"
        char *colon = strchr(msg_buffer, ':');

        //Move code value into code
        memcpy(code, msg_buffer, colon-msg_buffer);
        param = atoi(code);
        value = &msg_buffer[colon-msg_buffer+1];

        if(param == PROTOCOL)
        {
            printf("Swichting to use %s\n", value);
            if(strcmp("tcp", value) == 0)
            {
                use_udp = 0;
            }
            else if(strcmp("udp", value) == 0)
            {
                use_udp = 1;
            }
            else
            {
                printf("Protocol %s is not supported\n", value);
            }
        }
        else if(param == PERIODICITY)
        {
            int period = atoi(value);
            if(period < 100)
            {
                printf("Unable to set period to %d ms\n", period);
                printf("Period must be greater than or equal to 100 ms\n");
            }
            else
            {
                printf("Setting period to %d ms\n", period);
                periodicity = period;
            }
        }
        else if(param == PACKET_SIZE)
        {
            int size = atoi(value);
            if(size < 3 || size > 65507)
            {
                printf("Packet size could not be set to %d bytes\n", size);
                printf("Packet size must be greater than 3 bytes but no more than 65507 bytes\n");
            }
            else
            {
                printf("Setting packet size to %d bytes\n", size);
                packet_len = size;
            }
        }
        else
        {
            printf("Did not understand command.\n");
        }

        printf("Code was %s\n",code);
        printf("Value was %s\n", value);
    }
}

void control_close_socket(void)
{
    close(socket_descriptor);
}
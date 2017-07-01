#include "options.h"
#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, const char* argv[])
{
    void* parameter = 0;
    // Enumerate arguments
	for( int i = 1; i < argc; i++ )
	{
		printf( "arg %d: %s\n", i, argv[i] );
        if(parameter == 0)
        {
            if(strcmp(argv[i], "-h") == 0)
            {
                printf("Welcome to the broadcast tool for NTA\n");
                printf("broadcast [OPTIONS]\n");
                printf("Options\n");
                printf("\t-p [value]\t\tProtocol. Valid options: tcp, udp\n");
                printf("\t-P [value]\t\tPeriodicity. Measured in milliseconds\n");
                printf("\t-s [value]\t\tPacket size. Measured in bytes\n");
                return 0;
            }
            else if(strcmp(argv[i], "-p") == 0)
            {
                parameter = (void*) protocol;
            }
            else if(strcmp(argv[i], "-P") == 0)
            {
                parameter = (void*) &periodicity;
            }
            else if(strcmp(argv[i], "-s") == 0)
            {
                parameter = (void*) &packet_size;
            }
        }
        else
        {
            if(parameter == protocol)
            {
                strcpy(parameter, argv[i]);
            }
            else
            {
                *(int*)(parameter) = atoi(argv[i]);
            }
            parameter = 0;
        }
	}

    /*if(parameter != 0 || argc < 2)
    {
        printf("Missing interface name\n");
        return -1;
    }
    else if(argc > 3)
    {
        strcpy(interface, argv[argc-1]);
    }*/

    printf("Interface: %s\n", interface);
    printf("Protocol: %s\nPeriodicity: %d\nPacket size: %d\n",
        protocol, periodicity, packet_size);

    socket_initialise();

    socket_broadcast();

    return 0;
}
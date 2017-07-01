#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
    int mem_size;
    if(argc > 1)
    {
        // Raise to MB
        mem_size = atoi(argv[1]) << 20;
        if(atoi(argv[1]) > 2047) 
        {
            printf("Maximum allocatable memory is 2047 MB\n");
        }
    }
    else
    {
        // 50 MB
        mem_size = 50 << 20;
    }
    char* buffer = malloc(mem_size);
    if(buffer == NULL)
    {
        perror("Unable to allocate memory");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < mem_size; i++)
    {
        buffer[i] = i;
    }

    getchar();
    free(buffer);
    return EXIT_SUCCESS;
}
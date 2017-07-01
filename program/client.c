#include "shell.h"
#include "thread.h"
#include "socket.h"
#include "control.h"
#include "xtimer.h"
#include "timex.h"

char main_stack[THREAD_STACKSIZE_MAIN];
char listener_stack[THREAD_STACKSIZE_MAIN];
unsigned int periodicity = 1000;

void *thread_handler(void *arg)
{
    (void)arg;
    unsigned long long start, end;

    while(1) {
        start = xtimer_now_usec64();
        sendPacket();
        end = xtimer_now_usec64();
        xtimer_usleep((periodicity * 1000) - (unsigned int)(end - start));
    }
    return NULL;
}

void *control_thread(void *args)
{
    control_listener(args);

    return NULL;
}

int main(void)
{
    xtimer_init();
    initialize();
    control_initialize_listener();

    thread_create(main_stack, sizeof(main_stack),
            THREAD_PRIORITY_MAIN - 2,
            THREAD_CREATE_STACKTEST,
            thread_handler,
            NULL, "radio_thread");

    // Create a thread to listen on the control interface
    thread_create(listener_stack, sizeof(listener_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        control_thread,
        NULL, "control_listener");


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    closeSocket();
    control_close_socket();

    return 0;
}

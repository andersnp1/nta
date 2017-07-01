#ifndef CONTROL_H
#define CONTROL_H

extern unsigned int periodicity;
extern int packet_len;
extern int use_udp;

int control_initialize_listener(void);
void control_listener(void*);
void control_close_socket(void);

#endif // !CONTROL_H
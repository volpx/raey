#ifndef COMM_H
#define COMM_H

#define BUFSIZE 10

#include <stdint.h>
//#include <avr/io.h>

uint8_t commands_in_queue=0;

void get_available_commands();
void process_command();

#endif //COMM_H

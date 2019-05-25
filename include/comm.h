#ifndef COMM_H
#define COMM_H

#include <stdint.h>
//#include <avr/io.h>

extern uint8_t commands_in_queue;

void get_available_commands();
void process_command();

#endif //COMM_H

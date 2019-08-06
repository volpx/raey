#ifndef COMM_H
#define COMM_H

#include "../raey.h"

extern uint8_t commands_in_queue;
extern uint8_t state;

void get_available_commands();
void process_input();

//subroutines
void g(uint8_t);
void a(uint8_t);

#endif //COMM_H

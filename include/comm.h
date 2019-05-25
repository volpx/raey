#ifndef COMM_H
#define COMM_H

#include "../raey.h"

extern uint8_t commands_in_queue;

void get_available_commands();
void process_input();

#endif //COMM_H

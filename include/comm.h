#ifndef COMM_H
#define COMM_H

#include "../raey.h"
#include "util.h"

#include <string.h>

extern uint8_t commands_in_queue;
extern uint8_t state;

void get_available_commands();
void process_input();
uint8_t loadarrayfromstring_hex(uint8_t a[],char s[]);
uint8_t char_to_hex(char c);

#endif //COMM_H

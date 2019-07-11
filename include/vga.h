#ifndef VGA_H
#define VGA_H

#include "../raey.h"

#define VGA_DENB 2
#define VGA_CLK  3
#define VGA_DATA 4

void vga_init();
extern uint8_t vga_gain;
void vga_set_gain(const uint8_t);

#endif //VGA

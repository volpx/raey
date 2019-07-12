#ifndef UTIL_H
#define UTIL_H

#include "../raey.h"

#include <avr/wdt.h>

//port D
#define LASER 0x20
//port B
#define LED   0x20

extern uint8_t util_reg;
enum util_reg_bits{
  CON_PUL_EN
};

void pin_init();
void pulse_laser_n(uint8_t nops);
void pulse_laser();
inline void toggle_laser(){PORTD^=LASER;}
void idle();


void watchdog_init();

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }

#endif //UTIL_H

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <avr/wdt.h>
#include "macros.h"
//#include <avr/io.h>

#define LASER 0x20

void pin_init();
void pulse_laser_n(uint8_t nops);
void pulse_laser();
void toggle_laser();
void idle();

// Initialize watchdog with a 4 second interrupt time
void watchdog_init();

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }

#endif //UTIL_H

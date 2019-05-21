#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <avr/wdt.h>
//#include <avr/io.h>

void pulse_laser_n(uint8_t nops);
void pulse_laser(void);
void idle(void);

// Initialize watchdog with a 4 second interrupt time
void watchdog_init(void);

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset(void) { wdt_reset(); }

#endif //UTIL_H

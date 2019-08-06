#ifndef UTIL_H
#define UTIL_H

#include "../raey.h"

#include <avr/wdt.h>

//port D
#define LASER 5
//pin B5 is also the SPI clock, don't use it!
#define LED_BUILTIN   5
//port C
#define LED 3

extern uint8_t util_reg;
enum util_reg_bits{
  CON_PUL_EN
};

void pin_init();
void pulse_laser_n(uint8_t nops);
void pulse_laser();
inline void toggle_laser(){PORTD^=(1<<LASER);}
#define LED_ON() PORTC|=(1<<LED)
#define LED_OFF() PORTC&= ~(1<<LED)
#define LED_TOG() PORTC^= (1<<LED)
void idle();


void watchdog_init();

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }

#endif //UTIL_H

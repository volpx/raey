#ifndef UTIL_H
#define UTIL_H

#include "../raey.h"

#include <avr/wdt.h>

//port D
#define LASER 5
//pin B5 is also the SPI clock, don't use it!
#define LED_BUILTIN 5
//port C
#define LED 3

extern uint8_t util_reg;
#define UTIL_PULSE_ENABLE 0

void util_init();
void idle();
void laser_pulse_n(uint8_t nops);
void laser_pulse();
#define LED_ON() PORTC|=(1<<LED)
#define LED_OFF() PORTC&= ~(1<<LED)
#define LED_TOG() PORTC^= (1<<LED)
#define LASER_ON() PORTD|=(1<<LASER)
#define LASER_OFF() PORTD&= ~(1<<LASER)
#define LASER_TOG() PORTD^= (1<<LASER)

void overtemp();
void dovertemp();

void measure();

void watchdog_init();

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }

// Utility functions

//take number from string until end or space or letter
uint8_t stringtoint(const char s[]);
// return the index of the first non space after
// the space that comes after the index given
uint8_t nospace_after(const char s[],uint8_t start);

#endif //UTIL_H

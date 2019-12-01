#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#include "../raey.h"

// F_OVF=F_MCU/PRESCALER/TIMERCS/TOP=976.5 Hz
#define NOP_TIME        1/16e6
#define PWM_BOOST_PIN   1

extern volatile uint32_t ms;
void time_init();
void time_base();
void delay_ms(const uint16_t delay);

#endif //TIMER_H

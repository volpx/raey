#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#include "../raey.h"

// F_OVF=F_MCU/PRESCALER/TIMERCS/TOP=976.5 Hz
// NOP_TIME=1/(F_MCU/PRESCALER)
#define NOP_TIME        1/16e6
#define PWM_BOOST_PIN   1

//extern volatile uint16_t time_ovf_count;
extern uint32_t ms;
void time_init();
void time_base();

#endif //TIMER_H

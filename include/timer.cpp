#include "timer.h"

void delay_ms(uint8_t ms){
  while(ms)
    idle();
}
void timer_init(void){
  // Set port pin to output to abilitate pin driving
  DDRB|=0x02;
  // Set PWM mode and the wanted pin
  TCCR1A=0xC2;
  // Set prescaler
  TCCR1B=0x09;
  // Set half dutycycle
  OCR1A=0x0100;
  // Enable interrupts flags
  TIMSK1=0x023;
}

ISR(TIMER1_COMPA_vect){
  //PORTB^=0x20;
}
ISR(TIMER1_OVF_vect){
  //TODO this is callled at the overflow
}

#include "time.h"

uint8_t time=0;
void delay_ms(uint8_t ms){
  while(ms)
    idle();
}
void timer_init(void){
  // The timer module used in this configuration is TIMER1/16 bit PWM
  // Disable power saving
  //TODO:check if this was the problem of the dog dying (also decomment last row)
  //PRR&=~(1<<PRTIM1);
  // Set port pin to output to abilitate pin driving for PWM
  DDRB|=0x02;
  // Set PWM mode and the wanted pin
  TCCR1A=0xC2;
  // Set prescaler
  TCCR1B=0x09;
  // Set half dutycycle
  OCR1A=0x0100;
  // Enable interrupts flags
  //TODO: it causes the dog to die
  //TIMSK1=0x023;
}

ISR(TIMER1_COMPA_vect){
  // for pwm for example:
  //PORTB^=0x20;
}
ISR(TIMER1_OVF_vect){
  // incrase time variable
  // TODO: check how much is a unity of this variable
  time++;
}

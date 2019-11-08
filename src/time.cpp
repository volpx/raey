#include "time.h"

//uint16_t time_ovf_count=0;
uint32_t ms=0; // over 1000 hours before overflow
void delay_ms(uint32_t dms){
  dms+=ms;
  while(dms!=ms)
    idle();
}
void time_base(){
  
}
void time_init(){
  // Timer TIMER0
  // Disable power saving
  PRR&=~(1<<PRTIM0);
  // Enable overflow interrupt
  TIMSK0|=(1<<TOIE0);
  // Mode selection: normal
  TCCR0A|=0;
  // Scale clock
  TCCR0B|=(1<<CS01)|(1<<CS00);


  /*
  // Timer TIMER1
  // Set port pin to output to abilitate pin driving for PWM
  DDRB|=(1<<PWM_BOOST_PIN);
  // Set PWM mode and the wanted pin
  TCCR1A=0xC2;
  // Set prescaler
  TCCR1B=0x09;
  // Set half dutycycle
  OCR1A=0x0100;
  // Enable interrupts flags
  //TODO: it causes the dog to die
  //TIMSK1=0x023;
  */

  //  Set prescalers
  // Sync mode on
  GTCCR|=(1<<TSM);
  // Reset prescalers
  GTCCR|=(1<<PSRSYNC);
  // Sync mode off
  GTCCR&=~(1<<TSM);
}
ISR(TIMER1_COMPA_vect){
  // for pwm for example:
  //PORTB^=0x20;
}
ISR(TIMER0_OVF_vect){
  // incrase time variable
  //time_ovf_count++;
  ms++;
}

#include "time.h"

volatile uint32_t ms=0; // over 1000 hours before overflow
volatile uint16_t dms=0;
void delay_ms(const uint16_t delay){
  dms=delay+1;
  while(dms)
    idle();
}
void time_base(){

}
void time_init(){
  // Timer TIMER0
  // This timer is setted that it counts from 0 to 0xFF
  // and then generate the interrupt.
  // prescaler /64 ==>> 16MHz/64/256~=1kHz
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

  // Sync timers after prescaler selection
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
  ++ms;
  dms=(dms)?dms-1:0;
}

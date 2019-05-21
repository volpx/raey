#include "util.h"

void idle(void){
  //TODO
}
void pulse_laser_n(uint8_t nops){
  //turn laser on
  PORTB|=0x10;
  //wait some nops
  while(--nops);
  //turn laser off
  PORTB&=~0x10;
}
void pulse_laser(void){
  //turn laser on
  PORTB|=0x10;
  //nowait, as-fast-as-possible
  //turn laser off
  PORTB&=~0x10;
}


// Initialize watchdog with 8s timeout, if possible. Otherwise, make it 4s.
void watchdog_init(void) {
  #if ENABLED(WATCHDOG_DURATION_8S) && defined(WDTO_8S)
    #define WDTO_NS WDTO_8S
  #else
    #define WDTO_NS WDTO_4S
  #endif
  #if ENABLED(WATCHDOG_RESET_MANUAL)
    // Enable the watchdog timer, but only for the interrupt.
    // Take care, as this requires the correct order of operation, with interrupts disabled.
    // See the datasheet of any AVR chip for details.
    wdt_reset();
    cli();
    _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
    _WD_CONTROL_REG = _BV(WDIE) | (WDTO_NS & 0x07) | ((WDTO_NS & 0x08) << 2); // WDTO_NS directly does not work. bit 0-2 are consecutive in the register but the highest value bit is at bit 5
                                                                              // So worked for up to WDTO_2S
    sei();
    wdt_reset();
  #else
    wdt_enable(WDTO_NS); // The function handles the upper bit correct.
  #endif
  //delay(10000); // test it!
}

#include "util.h"

uint8_t util_reg=0;

void util_init(){
    // laser pin init
    // Port D13 (Pin 19 in the ATmega) made output
    DDRD |= (1<<LASER);

    // My led
    DDRC |= (1<<LED);
    // turn off the debug led
    // can be turned on with PORTC|=LED;
    LED_OFF();
}
void laser_pulse_n(uint8_t nops){
    //turn laser on
    LASER_ON();
    //wait some time
    while(--nops);
    //turn laser off
    LASER_OFF();
}
void laser_pulse(){
    //turn laser on
    LASER_ON();
    //nowait, as-fast-as-possible
    //turn laser off
    LASER_OFF();
}

void idle(){
    // watchdog_reset();
    time_base();
}

// Initialize watchdog with 8s timeout, if possible. Otherwise, make it 4s.
void watchdog_init(){
    // #define WDTO_NS WDTO_2S
    // #undef WATCHDOG_RESET_MANUAL
    // #if ENABLED(WATCHDOG_RESET_MANUAL)
    //   // Enable the watchdog timer, but only for the interrupt.
    //   // Take care, as this requires the correct order of operation, with interrupts disabled.
    //   // See the datasheet of any AVR chip for details.
    //   wdt_reset();
    //   cli();
    //   _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
    //   // WDTO_NS directly does not work. bit 0-2 are consecutive in the register but the highest value bit is at bit 5
    //   // So worked for up to WDTO_2S
    //   _WD_CONTROL_REG = _BV(WDIE) | (WDTO_NS & 0x07) | ((WDTO_NS & 0x08) << 2);
    //   sei();
    //   wdt_reset();
    // #else
    //   wdt_enable(WDTO_NS); // The function handles the upper bit correct.
    // #endif
    // //delay(10000); // test it!
    wdt_enable(WDTO_2S);
}

void overtemp(){
    // turn off the laser
    LASER_OFF();
    util_reg&=~(1<<UTIL_PULSE_ENABLE);
}

void dovertemp(){

}

void measure(){

}

uint8_t stringtoint(const char s[]){
    //take number from string until end or space or letter
    uint8_t res=0;
    char c=1;
    while (c){
        c=(*(s++));
        if (c>='0' && c<='9') {
            res=res*10+(c-'0');
        }
        else {
            c=0;
        }
    }
    return res;
}

uint8_t nospace_after(const char s[],uint8_t start){
    // return the index of the first non space after
    // the space that comes after the index given

    //find space
    while(s[start]!=' ' && s[start]!=0){
        ++start;
    }
    //find nospace
    while(s[start]==' ' && s[start]!=0){
        ++start;
    }
    return start;
}

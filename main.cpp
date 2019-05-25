#define F_CPU 16000000
#include "include/uart.h"
#include "include/timer.h"
#include "include/util.h"
#include "include/comm.h"
//https://appelsiini.net/2011/simple-usart-with-avr-libc

int main(void){

  watchdog_init();
  pin_init();
  uart_init();
  timer_init();
  sei();
  //PORTB |= 0x10;

  uint32_t foo=0;
  uart_print("Ready");

  while(1){
    while (COMMAND_AVAILABLE){
      //manage command
    }
    // if (!(foo++)){
    //   PORTB ^= LED;
    //
    // }
    idle();
    wdt_reset();
  }
  return 0;
}

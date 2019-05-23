#define F_CPU 16000000
#include "include/uart.h"
#include "include/timer.h"
#include "include/util.h"
#include "include/comm.h"
//https://appelsiini.net/2011/simple-usart-with-avr-libc

int main(void){

  pin_init();
  uart_init();
  timer_init();
  sei();
  //PORTB |= 0x10;

  while(1){
    while (COMMAND_AVAILABLE){
      //manage command
    }
    idle();
    watchdog_reset();
  }
  return 0;
}

#define F_CPU 16000000
#include "include/uart.h"
#include "include/timer.h"
#include "include/util.h"
#include "include/comm.h"
//https://appelsiini.net/2011/simple-usart-with-avr-libc

int main(void){
  // laser pin init
  // Port D13 (Pin 19 in the ATmega) made output
  DDRB = 0b00110000;

  uart_init();
  timer_init();
  sei();

  while(1){
    if (commands_in_queue < BUFSIZE)
      get_available_commands();
    idle();
  }
  return 0;
}

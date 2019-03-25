#define F_CPU 16000000
#include <avr/io.h> // This contains the definitions of the terms used
#include <util/delay.h> // This contains the definition of delay function
#include "include/uart.h"
#include "include/timer.h"
//https://appelsiini.net/2011/simple-usart-with-avr-libc

int main(void){
  DDRB = 0b00100000; // Port D2 (Pin 4 in the ATmega) made output
  PORTB |= 0b00100000; // Turn LED on

  uart_init();
  timer_init();
  sei();

  while(1){
    uart_tx('c');
    _delay_ms(100);
    OCR1A=(OCR1A+10)%0x0200;

  }
  return 0;
}

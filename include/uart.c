// https://github.com/crapp/uartavr/blob/master/src/uart.h
#include "uart.h"

void uart_init(void){
  // Disable module power reduction
  PRR&=~(0x02);
  // Set 9600 boudrate given F_CPU=16000000
  UBRR0=0x67;
  // Enable RxTx parts and the interrupts
  UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
  // Async mode, no parity check, 1 stop bit, 8 bit frame
  UCSR0C=0x06;
}
void uart_tx(uint8_t data){
  // Wait tx buffer flush
  while(!(UCSR0A & (1<<UDRE0)));
  // send data
  UDR0=data;
}
uint8_t uart_rx(void){
  while(!(UCSR0A & (1<<RXC0)));

  return UDR0;
}
void uart_rx_flush(void){

}
ISR(USART_TX_vect)
{
  //PORTB^=0x20;
}

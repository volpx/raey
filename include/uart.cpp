// https://github.com/crapp/uartavr/blob/master/src/uart.h
#include "uart.h"

void uart_init(){
  // Disable module power reduction
  PRR&=~(0x02);
  // Set 9600 boudrate given F_CPU=16000000
  UBRR0=0x89;
  // Enable RxTx parts and the interrupts
  UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
  // Async mode, no parity check, 1 stop bit, 8 bit frame
  UCSR0C=0x06;
}
void uart_print(const uint8_t *s){
  while (const uint8_t c = *(s++))
    uart_tx(c);
}
void uart_tx(const uint8_t c){
  // Wait tx buffer flush
  while(!(UCSR0A & (1<<UDRE0)));
  // send data
  UDR0=c;
}
uint8_t uart_rx(){
  while(!(UCSR0A & (1<<RXC0)));

  return UDR0;
}
void uart_rx_flush(){
  //TODO
}
ISR(USART_TX_vect){
  //TODO
  //PORTB^=0x20;
}
ISR(USART_RX_vect){
  //TODO
}

// https://github.com/crapp/uartavr/blob/master/src/uart.h
#include "uart.h"

//Variable declaration
uint8_t uart_reg=0;
char buf_rx[BUFSIZE];
uint8_t buf_rx_head=0;
uint8_t buf_rx_tail=0;
char buf_tx[BUFSIZE];
uint8_t buf_tx_head=0;
uint8_t buf_tx_tail=0;

void uart_init(){
  // Disable module power reduction
  PRR&=~(0x02);
  // Set 9600 baudrate given F_CPU=16000000
  // UBRR0=F_CPU/16/BAUD-1
  UBRR0=0x67;
  // Enable RxTx parts and the interrupts
  UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
  // Async mode, no parity check, 1 stop bit, 8 bit frame
  UCSR0C=0x06;
}
void uart_print(const char *s){
  // wait last transmission to end,
  // when multi commands buffer will be implemented
  // it will wait only if it's the last command in buffer, unlikely
  //while (uart_reg&TX_PEN);

  // load the tx buffer
  while (const char c=(*(s++))){
    // wait to not overwrite data
    while ((buf_tx_head+1)%BUFSIZE==buf_tx_tail);
    // increment head
    buf_tx[buf_tx_head=(buf_tx_head+1)%BUFSIZE]=c;
    if (!(uart_reg&TX_PEN)){
      // start first byte, will continue in the interrupt
      enable_tx();
      UDR0=buf_tx[buf_tx_tail=(buf_tx_tail+1)%BUFSIZE];
    }
  }
}
void uart_byte(const char c){
  // wait to not overwrite data
  while ((buf_tx_head+1)%BUFSIZE==buf_tx_tail);
  // increment head
  buf_tx[buf_tx_head=(buf_tx_head+1)%BUFSIZE]=c;
  if (!(uart_reg&TX_PEN)){
    // start first byte, will continue in the interrupt
    enable_tx();
    UDR0=buf_tx[buf_tx_tail=(buf_tx_tail+1)%BUFSIZE];
  }
}
ISR(USART_TX_vect){
  // transmitted, ready to go on
  if (buf_tx_head!=buf_tx_tail){
    // transmit next
    UDR0=buf_tx[buf_tx_tail=(buf_tx_tail+1)%BUFSIZE];
  }
  else {
    // unset transmission pending
    disable_tx();
  }
}
ISR(USART_RX_vect){
  // read the data
  const char data=UDR0;
  // manage the data
  uart_reg|=NEW_DATA;
  if ((buf_rx_head+1)%BUFSIZE==buf_rx_tail){
    // full buffer, raise the error flag
    uart_reg|=RX_FULL;
  } else {
    // save data in buffer
    buf_rx[buf_rx_head=(buf_rx_head+1)%BUFSIZE]=data;
  }
}

// dangling functions below
void _uart_tx(const char c){
  // Wait tx buffer flush
  while(!(UCSR0A & (1<<UDRE0)));
  // send data
  UDR0=c;
}
uint8_t _uart_rx(){
  //maybe not relevant anymore
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

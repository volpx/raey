// https://github.com/crapp/uartavr/blob/master/src/uart.h
#include "uart.h"

//Variable declaration
uint8_t uart_reg=0;
uint8_t buf_rx[MAX_CMD_SIZE];
uint8_t buf_rx_ind=0;
uint8_t buf_rx[MAX_CMD_SIZE];
uint8_t buf_tx_ind=0;
uint8_t buf_tx_out=0;


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
  while (uart_reg&TX_PEN);
  while (const uint8_t c=(*(s++)))
    uart_tx(c);
  //start first byte
  UDR0=buf_tx[buf_tx_out++ % BUFSIZE];
}
void uart_tx(const uint8_t c){
  buf_tx[buf_tx_ind++ % BUFSIZE]=c;
}
void _uart_tx(const uint8_t c){
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
ISR(USART_TX_vect){
  //transmitted, ready to go on
  if (buf_tx_ind!=buf_tx_out){
    //transmit next
    UDR0=buf_tx[buf_tx_out++ % BUFSIZE];
  }
  else{
    //unset transmission pending
    uart_reg&=~TX_PEN;
  }
}
ISR(USART_RX_vect){
  //read the data
  const uint8_t data=UDR0;
  //manage the data
  switch(data){
    case '\n':
      uart_reg |= NEW_COMMAND;
      buf_rx_ind=0;
      break;
    default:
      buf_rx[buf_rx_ind++]=data;
  }
  //buf_tx[buf_tx_ind++ % BUFSIZE]=data;
  UDR0=data;
}

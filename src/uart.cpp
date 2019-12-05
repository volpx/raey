#include "uart.h"

Uart uart=Uart();

Uart::Uart():
  rx_buf_(rx_buf_raw_,UART_BUFSIZE),
  tx_buf_(tx_buf_raw_,UART_BUFSIZE)
  {
    reg_=0;
    commands_in_=0;
}

void Uart::init(){
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

void Uart::tx_byte(const uint8_t data){
  // wait if full to not overwrite data
  while (tx_buf_.full());
  // increment head
  tx_buf_.put(data);
  if (!(reg_&(1<<UART_TX_PEN))){
    // start first byte, will continue in the interrupt
    tx_enable();
    UDR0=tx_buf_.get();
  }
}
void Uart::print(const char s[]){
  // wait last transmission to end,
  // when multi commands buffer will be implemented
  // it will wait only if it's the last command in buffer, unlikely
  //while (uart_reg&TX_PEN);

  // load the tx buffer
  while (const char c=(*(s++))){
    // wait to not overwrite data, comment to overwrite from bottom
    while (tx_buf_.full());
    // increment head
    tx_buf_.put(c);
    if (c=='\n'){
      // Also automatically add carriage return
      while (tx_buf_.full());
      tx_buf_.put('\r');
    }
    if (!(reg_&(1<<UART_TX_PEN))){
      // start first byte, will continue in the interrupt
      tx_enable();
      UDR0=tx_buf_.get();
    }
  }
}
void Uart::println(const char s[]){
  print(s);
  tx_byte('\n');
}

void Uart::tx_uint(const uint32_t n){
  //Print a uint from 0 to 2**32
  uint8_t first_gone=false;
  uint8_t c=0;
  uint32_t d=1000000000;
  do{
    c=(n/d)%10;
    if (first_gone || c!=0 || d==10){
      tx_byte('0'+c);
      first_gone=true;
    }
    d/=10;
  }while(d!=0);
}
void Uart::tx_float(const float f,const uint8_t dig){
  //Print a float with dig digits after point
  //print integer part
  uart.tx_uint((uint32_t)f);
  if (dig!=0){
    uart.tx_byte('.');
    float dec=f-(uint32_t)f;
    for(uint8_t i =0; i<dig;++i){
      dec*=10;
      uart.tx_byte('0'+((uint8_t)dec%10));
    }
  }
}
void Uart::tx_hex(const uint8_t d){
  if ((d>>4) < 10){
    tx_byte((d>>4)+'0');
  }
  else {
    tx_byte((d>>4)+'A'-10);
  }
  if ((d&0x0F) < 10){
    tx_byte((d&0x0F)+'0');
  }
  else {
    tx_byte((d&0x0F)+'A'-10);
  }
}
void Uart::tx_enable(){
  reg_|=(1<<UART_TX_PEN);
}
void Uart::tx_disable(){
  // TODO: can also disable module
  reg_&=~(1<<UART_TX_PEN);
}
bool Uart::rx_available() const{
  return reg_&(1<<UART_NEW_DATA);
}
bool Uart::rx_available_command() const{
  return commands_in_;
}
uint8_t Uart::rx_byte() {
  uint8_t data=rx_buf_.get();
  // send back
  tx_byte(data);

  if (rx_buf_.empty())
    reg_&=~(1<<UART_NEW_DATA);

  return data;
}
void Uart::rx_command(char s[]){
  uint8_t c=rx_byte();
  uint8_t i=0;
  while(c){
    if (c=='\n' || c==0){
      c=0;
    }
    else if (c=='\r'){
      // skip that one little shit character
      c=rx_byte();
    }
    else {
      // save it
      s[i++]=c;
      // get next one
      c=rx_byte();
    }
  }
  // terminate
  s[++i]=0;
  commands_in_ = (commands_in_ ==0 ? 0 : --commands_in_);
  if (commands_in_==0){
    reg_&=~(1<<UART_NEW_COMMAND);
  }
}

ISR(USART_TX_vect){
  // transmitted, ready to go on
  if (!uart.tx_buf_.empty()){
    // transmit next
    UDR0=uart.tx_buf_.get();
  }
  else {
    // unset transmission pending
    uart.tx_disable();
  }
}
ISR(USART_RX_vect){
  // read the data
  const char data=UDR0;
  // manage the data
  uart.reg_|=(1<<UART_NEW_DATA);
  if (uart.rx_buf_.full()){
    // full buffer, raise the error flag
    uart.reg_|=(1<<UART_RX_FULL);
  }
  else if (data=='\n'){
    // save data in buffer
    uart.rx_buf_.put(data);
    uart.reg_|=UART_NEW_COMMAND;
    uart.commands_in_++;
  } else {
    uart.rx_buf_.put(data);
  }
}

#include "comm.h"

uint8_t commands_in_queue=0;
void get_available_commands(void){
  //TODO
}
void process_input(){
  while(buf_rx_tail!=buf_rx_head){
    // thereis another byte to read in tail+1
    uint8_t data=buf_rx[buf_rx_tail=(buf_rx_tail+1)%BUFSIZE];
    uart_byte(data);
    // TODO: state machine!!!!!
  }
  uart_reg&=~NEW_DATA;
}

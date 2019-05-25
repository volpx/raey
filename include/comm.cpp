#include "comm.h"

uint8_t commands_in_queue=0;
void get_available_commands(void){
  //TODO
}
void process_input(){
  while((buf_rx_tail+1)%BUFSIZE==buf_rx_head){
    uart_byte(buf_rx[buf_rx_tail=(buf_rx_tail+1)%BUFSIZE]);
    // TODO: state machine!!!!!
  }
}

#include "comm.h"

uint8_t commands_in_queue=0;
uint8_t state=0;
uint8_t ind=0;
uint8_t tmp_var=0;

void get_available_commands(void){
  //TODO
}
void process_input(){
  while(buf_rx_tail!=buf_rx_head){
    // thereis another byte to read in tail+1
    uint8_t data=buf_rx[buf_rx_tail=(buf_rx_tail+1)%BUFSIZE];
    uart_byte(data);

    if (ind==0){
      ind++;
      switch (data) {
        case 't':
          adc_start(ADCWhich::UTEMP);
          break;
        case 'l':
          adc_start(ADCWhich::DTEMP);
          break;
        case 'g':
          state='g';
          break;
        default:
          ind--;
      }
    }
    else {
      switch(state){
        case 'g':
          g(data);
          break;
      }
      if (ind==255){
        //there has been an error in input, abort
        uart_print("ERR\n");
        tmp_var=0;
        ind=0;
      }
    }
  }
  uart_reg&=~NEW_DATA;
}

void g(uint8_t data){
  if (data>'9' || data<'0'){
    if (data==0x0A){
      // end line
      vga_set_gain(tmp_var);
      tmp_var=0;
      ind=0;
    }
    else{
      // transmission error
      ind=255;
    }
    return;
  }
  // build value
  tmp_var=tmp_var*10+(data-'0');
}

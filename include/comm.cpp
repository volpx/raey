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
        case 'p':
          uart_print("\n");
          pulse_laser();
          ind=0;
          break;
        case 's':
          uart_print("\n");
          toggle_laser();
          ind=0;
          break;
        case 'q':
          uart_print("\n");
          util_reg^= (1<<CON_PUL_EN);
          ind=0;
          break;
        case 'a':
          state='a';
          break;
        case 'b':
          for(uint8_t i=0;i<spi_pack_size;i++)
            uart_byte(spi_pack[i]);
          ind=0;
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
        case 'a':
          a(data);
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
void a(uint8_t data){
  if ((ind-1)==spi_pack_size-1){
    spi_pack[ind-1]=data;
    uart_print("OK\n");
    spi_tx(SPIWhich::TDC);
    ind=0;
    return;
  }
  spi_pack[ind-1]=data;
  ind++;
}
void g(uint8_t data){
  if (data>'9' || data<'0'){
    if (data==0x0D){
      // end line
      uart_uint(tmp_var);
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

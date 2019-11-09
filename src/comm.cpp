#include "comm.h"

uint8_t state=0;
uint8_t ind=0;
uint8_t tmp_var=0;
char comms[32];

void get_available_commands(void){
  //TODO
}
void process_input(){
  if(uart.rx_available_command()){
    // there is another command to read
    uart.rx_command(comms);
    state=strcmp(comms,"ciao");

    uint8_t data=uart.rx_byte();
    uart.tx_byte(data);
    uart.print("\n");
    if (ind==0){
      ind++;
      switch (data) {
        case 't':
          uart.print("Start uC temp meas\n");
          adc_onetime(ADCWhich::UTEMP);
          ind=0;
          break;
        case 'l':
          uart.print("Start pd temp meas\n");
          adc_onetime(ADCWhich::DTEMP);
          break;
        case 'g':
          // gain setter
          state='g';
          break;
        case 'p':
          uart.print("Pulse laser\n");
          uart.print("\n");
          laser_pulse();
          ind=0;
          break;
        case 's':
          uart.print("Laser toggle\n");
          LASER_TOG();
          ind=0;
          break;
        case 'q':
          uart.print("Toggle laser pulsing\n");
          util_reg^= (1<<UTIL_PULSE_ENABLE);
          ind=0;
          break;
        case 'a':
          // communicate to TDC
          state='a';
          break;
        case 'b':
          uart.print("Read spi\n");
          {
            int ii=0;
            while(spi.available()){
              uart.tx_byte(spi.buf[++ii]);
            }
          }
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
        uart.print("ERR\n");
        tmp_var=0;
        ind=0;
      }
    }
  }
}
void a(uint8_t data){
  if ((ind-1)==8-1){
    spi.buf[ind-1]=data;
    uart.print("OK\n");
    spi.tx(SPIWhich::TDC,8);
    ind=0;
    return;
  }
  spi.buf[ind-1]=data;
  ind++;
}
void g(uint8_t data){
  if (data>'9' || data<'0'){
    if (data==0x0D){
      // end line
      uart.tx_uint(tmp_var);
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

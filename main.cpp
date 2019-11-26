#include "raey.h"

int main(void){

  watchdog_init();
  util_init();
  uart.init();
  adc_init();
  time_init();
  vga_init();
  spi.init();
  sei();


  // uint16_t foo=0;

  uart.print("\nReady!\n");
  LED_OFF();

  while(1){
    if (adc_available()){
      // manage new adcdata
      adc_process();
    }
    else if (spi.available()){
      for (uint8_t i=0;i<spi.pack_size();i++){
        uart.tx_byte(spi.buf[i]);
      }
      spi.reset();
    }
    else if (uart.rx_available_command()){
      // manage data
      process_input();
    }
    else if (util_reg&(1<<UTIL_PULSE_ENABLE)){
      laser_pulse();
    }
    idle();
  }
  return 0;
}

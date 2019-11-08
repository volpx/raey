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
  LED_ON();

  while(1){
    if (uart.rx_available()){
      // manage data
      process_input();
    }
    if (adc_available()){
      // manage new adcdata
      adc_process();
    }
    if (util_reg&(1<<UTIL_PULSE_ENABLE)){
      laser_pulse();
    }
    if (spi.available()){
      for (uint8_t i=0;i<spi.pack_size();i++){
        uart.tx_byte(spi.buf[i]);
      }
      spi.reset();
    }
    idle();
    wdt_reset();
  }
  return 0;
}

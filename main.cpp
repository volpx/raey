#include "raey.h"

int main(void){

  watchdog_init();
  pin_init();
  uart_init();
  //adc_init();
  //timer_init();
  //vga_init();
  spi_master_init();
  sei();


  // uint16_t foo=0;

  uart_print("\nReady!\n");
  LED_ON();
  while(1){
    if (uart_rx_available()){
      // manage data
      process_input();
    }
    if (adc_available()){
      // manage new adcdata
      adc_process();
    }
    if (util_reg&(1<<CON_PUL_EN)){
      pulse_laser();
    }
    if (spi_available()){
      for (uint8_t i=0;i<spi_pack_size;i++){
        uart_byte(spi_pack[i]);
      }
      spi_reset();
    }
    idle();
    wdt_reset();
  }
  return 0;
}

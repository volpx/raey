#include "raey.h"

int main(void){

  watchdog_init();
  pin_init();
  uart_init();
  adc_init();
  //timer_init();
  vga_init();
  sei();


  // uint16_t foo=0;

  uart_print("Ready!\n");

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
    idle();
    wdt_reset();
  }
  return 0;
}

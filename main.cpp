#define F_CPU 16000000
#include "include/uart.h"
#include "include/timer.h"
#include "include/util.h"
#include "include/comm.h"

int main(void){

  watchdog_init();
  pin_init();
  uart_init();
  //timer_init();
  sei();

  uint16_t foo=0;

  uart_print("Ready!");

  while(1){
    while (uart_rx_available()){
      //manage data
      process_input();
    }
    idle();
    wdt_reset();
  }
  return 0;
}

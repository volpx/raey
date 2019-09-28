#include "vga.h"

//Variable declaration
uint8_t vga_gain=0x0F;

void vga_init(){
  // init communication pins
  DDRD|= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) ;
  PORTD&= ~(0x1C) ;

  vga_set_gain(0);
}
void vga_set_gain(const uint8_t gain){
  // take down the latch and clock
  PORTD&= ~((1<<VGA_DENB)|(1<<VGA_CLK));

  // set gain
  for (uint8_t i=0 ; i<4 ; i++){
    PORTD= (PORTD&(~(1<<VGA_DATA))) | (((gain>>(3-i))&0x01)<<VGA_DATA) ;
    PORTD|= (1<<VGA_CLK) ;
    PORTD&= ~(1<<VGA_CLK);
  }

  PORTD|=(1<<VGA_DENB);
  vga_gain=gain;
}

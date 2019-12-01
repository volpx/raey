#include "vga.h"

//Variable declaration
uint8_t vga_gain=0x00;

void vga_init(){
  vga_set_gain(15);
}
void vga_set_gain(const uint8_t gain){
  spi.wait_available();
  spi.buf[0]=gain;
  spi.tx(SPIWhich::VGA,1);
  vga_gain=gain;
}

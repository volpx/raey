#include "vga.h"

//Variable declaration
uint8_t vga_gain=0x0F;

void vga_init(){
  vga_set_gain(0);
}
void vga_set_gain(const uint8_t gain){
  spi.wait_available();
  spi.buf[0]=(gain<<4);
  spi.tx(SPIWhich::VGA,1);
  vga_gain=gain;
}

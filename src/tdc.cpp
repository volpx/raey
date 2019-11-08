#include "tdc.h"

//Variable declaration
uint8_t tdc_reg=0;
uint8_t calib1=0;
uint8_t calib2=0;

void tdc_init(){

}
void calibration(){
  // First calibration

  // Set second calibration in CONFIG2

  // Store the values of calibration 1,2

}
void get_measure(){
  // get last measure
  spi.tx(SPIWhich::TDC,1);
  // unset the new flag
  tdc_reg&=~(1<<TDC_NEW);
}

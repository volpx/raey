#include "tdc.h"

//Variable declaration
uint8_t tdc_reg=0;

void tdc_clear_int_status();

void tdc_init(){
  // Mode selected is 1

  // interrupt port
  DDRD&=~(1<<TDC_INT_PIN);
  PORTD|=(1<<TDC_INT_PIN);

  // CONFIG 1
  spi.wait_available();
  spi.buf[0]=TDC7200_WRITE_BIT | TDC7200_CONFIG1_REG;
  spi.buf[1]=TDC7200_STOP_EDGE_POLARITY;
  spi.tx(SPIWhich::TDC,2);

  // CONFIG 2
  spi.wait_available();
  spi.buf[0]=TDC7200_WRITE_BIT | TDC7200_CONFIG2_REG;
  //10 cal periods
  spi.buf[1]=TDC7200_CAL2_PERIODS0;
  spi.tx(SPIWhich::TDC,2);

  // INT MASK
  spi.wait_available();
  spi.buf[0]=TDC7200_WRITE_BIT | TDC7200_INTRPT_MASK_REG;
  spi.buf[1]=TDC7200_NEW_MEAS_MASK
              |TDC7200_COARSE_CNT_OF_MASK
              |TDC7200_CLOCK_CNT_OF_MASK;
  spi.tx(SPIWhich::TDC,2);

  // clear INT STATUS
  spi.wait_available();
  spi.buf[0]=TDC7200_WRITE_BIT | TDC7200_INTRPT_MASK_REG;
  spi.buf[1]=TDC7200_NEW_MEAS_MASK
              |TDC7200_COARSE_CNT_OF_MASK
              |TDC7200_CLOCK_CNT_OF_MASK;
  spi.tx(SPIWhich::TDC,2);

  tdc_clear_int_status();

  tdc_reg=0;
}
void tdc_clear_int_status(){
  // clear INT STATUS
  spi.wait_available();
  spi.buf[0]=TDC7200_WRITE_BIT | TDC7200_INTRPT_STATUS_REG;
  spi.buf[1]=0xFF;
  spi.tx(SPIWhich::TDC,2);
}
void tdc_prepare_measure(){
  tdc_reg|=(1<<TDC_BUSY);
  tdc_reg&=~(1<<TDC_NEW);

  // start measure
  spi.wait_available();
  spi.buf[0]=TDC7200_CONFIG1_REG|TDC7200_WRITE_BIT;
  spi.buf[1]=TDC7200_START_NEW_MEASUREMENT
      |TDC7200_STOP_EDGE_POLARITY
      |TDC7200_FORCE_CALIBRATION;
  spi.tx(SPIWhich::TDC,2);
  spi.wait_available();
}

void tdc_wait_finished(){
  while(PIND&(1<<TDC_INT_PIN)){
    idle();
  }
}

float tdc_get_measure(){
  // read status register
  spi.wait_available();
  spi.buf[0]=TDC7200_INTRPT_STATUS_REG;
  spi.tx(SPIWhich::TDC,2);

  spi.wait_available();
  uart.tx_hex(spi.buf[1]);
  uart.tx_byte('\n');

  // get time 1
  spi.wait_available();
  spi.buf[0]=TDC7200_TIME1_REG|TDC7200_AUTOINC_ON_BIT;
  spi.tx(SPIWhich::TDC,1+3+3);
  spi.wait_available();
  uint32_t time1=((uint32_t)spi.buf[3]<<16)
      +((uint32_t)spi.buf[2]<<8)+(uint32_t)spi.buf[1];
  uint32_t clock1=((uint32_t)spi.buf[6]<<16)
      +((uint32_t)spi.buf[5]<<8)+(uint32_t)spi.buf[4];

  spi.wait_available();
  uart.tx_uint(time1);
  uart.tx_byte('\n');
  uart.tx_uint(clock1);
  uart.tx_byte('\n');

  // get calib
  spi.buf[0]=TDC7200_CALIBRATION1_REG|TDC7200_AUTOINC_ON_BIT;
  spi.tx(SPIWhich::TDC,1+3+3);
  spi.wait_available();
  uint32_t calib1=((uint32_t)spi.buf[3]<<16)
      +((uint32_t)spi.buf[2]<<8)+(uint32_t)spi.buf[1];
  uint32_t calib2=((uint32_t)spi.buf[6]<<16)
      +((uint32_t)spi.buf[5]<<8)+(uint32_t)spi.buf[4];

  spi.wait_available();
  uart.tx_uint(calib1);
  uart.tx_byte('\n');
  uart.tx_uint(calib2);
  uart.tx_byte('\n');

  // calculate time
  float normLSB=TDC_CLOCK_PERIOD/
      ((float)(calib2-calib1)/(TDC_CAL_PERIODS-1));
  float tof1=time1*normLSB;

  tdc_reg|=(1<<TDC_NEW);
  tdc_reg&=~(1<<TDC_BUSY);

  return tof1;
}

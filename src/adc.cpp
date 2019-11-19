#include "adc.h"

uint8_t adc_data=0;
uint8_t adc_reg=0;
uint8_t adc_which=ADCWhich::NONE;

void adc_init(){
  ADMUX=(1<<ADLAR)|(1<<REFS0) | (0x0E);
  // 0x07 is the 128 prescaler
  ADCSRA=(1<<ADEN) | (1<<ADIE) | (0x07);
  // disable driving on pins
  DIDR0=(1<<ADC0D);

  // start freerunning on temperature
  ADCSRA|=(1<<ADATE);
  adc_reg&=~ADC_ONETIME;
  adc_start(ADCWhich::DTEMP);
}
void adc_onetime(const uint8_t which){
  // Stop the freerunning on temp
  ADCSRA&=~(1<<ADATE);
  adc_reg|=ADC_ONETIME;
  // Start this one time
  adc_start(which);
}
void adc_start(const uint8_t which){
  adc_which=which;
  adc_reg|=ADC_CONV_PEN;
  adc_reg&=~ADC_DONE_FIRST;
  ADMUX=(0xF0&ADMUX)|adc_which;
  ADCSRA|=(1<<ADSC);
}
void adc_stop(){
  ADCSRA&=~(1<<ADSC);
  adc_reg&=~ADC_DONE_FIRST;
  adc_which=ADCWhich::NONE;
}
ISR(ADC_vect){
  adc_reg&=~ADC_CONV_PEN;
  if (!(adc_reg&ADC_DONE_FIRST)){
    adc_start(adc_which);
    adc_reg|=ADC_DONE_FIRST;
  }
  else{
    if (adc_reg&ADC_NEW_DATA){
      // I have not read the last data
      adc_reg|=ADC_FULL_ERR;
    }
    else{
      adc_data=ADCH;
      adc_reg|=ADC_NEW_DATA;
    }
  }
}
void adc_process(){
  // unset new data
  adc_reg&=~ADC_NEW_DATA;
  // Do whatever
  switch(adc_which){
    case ADCWhich::DTEMP:
      if (adc_data>TMAX){
        overtemp();
        adc_reg|=OVERTEMP;
        uart.print("OverTemp!\n");
      }
      else if (adc_reg&OVERTEMP){
        adc_reg&= ~OVERTEMP;
        dovertemp();
      }
      break;
    case ADCWhich::UTEMP:
      uart.print("Temp uC:");
      uart.tx_uint(adc_data);
      uart.print("\n");
      break;
  }
  // then continue freerunning on DTEMP
  ADCSRA|=(1<<ADATE);
  adc_start(ADCWhich::DTEMP);

}

void temp_adc(){
  // TODO: make float for example
  // ADCx if the data register
  // TS_OFFSET os the correction term
  // TS_GAIN from the signature row
  // see datasheet
  //return (((ADCH<<8)+ADCL)-(273+100-TSOFFSET))*128/TSGAIN+25;
}

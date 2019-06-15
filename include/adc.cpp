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
  adc_start(ADCWhich::DTEMP);
}
void adc_onetime(const uint8_t which){
  // Stop the freerunning on temp
  ADCSRA&=~(1<<ADATE);
  // Start this one time
  adc_start(which);
}
void adc_start(const uint8_t which){
  adc_which=which;
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
  if (adc_reg&ADC_NEW_DATA){
    // I have not read the last data
    adc_reg|=ADC_FULL_ERR;
  }
  else{
    adc_data=ADCH;
    adc_reg|=ADC_NEW_DATA;
  }
}
void adc_process(){
  // unset new data
  adc_reg&=~ADC_NEW_DATA;
  if (adc_reg&ADC_DONE_FIRST){
    // Do whatever
    switch(adc_which){
      case ADCWhich::DTEMP:
        if (adc_data>TMAX){
          // overtemp();
          uart_print("OverTemp!\n");
        }
        else{
          // deovertemp();
        }
        break;
      case ADCWhich::UTEMP:
        break;
    }

    // then continue freerunning on DTEMP
    ADCSRA|=(1<<ADATE);
    adc_start(ADCWhich::DTEMP);
  }
  else{
    // Skip the first
    adc_reg|=ADC_DONE_FIRST;
    adc_start(adc_which);
  }
}

void temp_adc(){
  // TODO: make float for example
  // ADCx if the data register
  // TS_OFFSET os the correction term
  // TS_GAIN from the signature row
  // see datasheet
  //return (((ADCH<<8)+ADCL)-(273+100-TSOFFSET))*128/TSGAIN+25;
}

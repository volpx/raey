#ifndef ADC_H
#define ADC_H

#include "../raey.h"

/*
* The adc is in free running on monitoring the temperature of the
* photodiode/laser on pin ADC0D aka pin PC0
*
* TMAX=(m*tmax+q)/Vdd*256
*/
#define TMAX  0x55

extern uint8_t adc_reg;
#define ADC_NEW_DATA   1
#define ADC_CONV_PEN   2
#define ADC_FULL_ERR   4
#define ADC_DONE_FIRST 8
#define OVERTEMP       16

extern uint8_t adc_data;
extern uint8_t adc_which;

// For adc_start
#define DTEMP      0x00
#define UTEMP     0x08
void adc_start(const uint8_t which);
void adc_onetime(const uint8_t which);

void adc_stop();
void adc_init();

inline uint8_t adc_available(){
  return adc_reg&ADC_NEW_DATA;
}

#endif //ADC_H

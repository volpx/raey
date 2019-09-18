#ifndef TDC_H
#define TDC_H

#include "../raey.h"

//status
extern uint8_t tdc_reg;
#define TDC_READY 0
#define TDC_NEW   1

// spi flags
#define TDC_AINC       7
#define TDC_WRITE      6
#define TDC_ADDR_MASK 0x3F

//registers definitions
#define TDC_CONFIG1    0x00
#define TDC_CONFIG2    0x01


#endif //TDC_H

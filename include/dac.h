#ifndef DAC_H
#define DAC_H

#include "../raey.h"
#include <stdint.h>

#define DAC_THR 1
#define DAC_OVD 2

void dac_setout(const uint8_t which, const uint8_t value);



#endif //DAC_H
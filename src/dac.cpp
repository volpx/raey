#include "dac.h"

void dac_setout(const uint8_t which, const uint8_t value){
    spi.wait_available();
    switch(which){
        case DAC_THR:
            spi.buf[0]=0xF0|(value>>4);
            spi.buf[1]=(value<<4);
            break;
        case DAC_OVD:
            spi.buf[0]=0x70|(value>>4);
            spi.buf[1]=(value<<4);
            break;
    }
    spi.tx(SPIWhich::DAC,2);
}
#ifndef SPI_H
#define SPI_H

#include "../raey.h"

// packet size
extern uint8_t spi_pack_size;
#define SPI_MAXBUFSIZE 8

// which select
enum class SPIWhich{
  TDC,
  VGA
};
extern SPIWhich spi_which;

// buffer, only one
extern uint8_t spi_pack[SPI_MAXBUFSIZE];
// is the index of the last byte transmited as
// well as where goes next byte received
// it can be used to determine when the transmission has ended
extern volatile uint8_t spi_point;

void spi_master_init();
void spi_tx(const SPIWhich which);
void spi_tx(const SPIWhich which,const uint8_t size);
inline uint8_t spi_available(){
  return spi_point==spi_pack_size;
}
inline void spi_reset(){ spi_point=0;}

#endif //SPI_H

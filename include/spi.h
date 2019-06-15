#ifndef SPI_H
#define SPI_H

#include "../raey.h"

// packet size
extern uint8_t spi_pack_size;
#define SPI_BUFSIZE 8

//status
extern uint8_t spi_reg;
namespace spi{
enum spi_reg_e{
  NEW_COMMAND=1,
  TX_PEN     =2,
  RX_PEN     =4,
  NEW_DATA   =8,
  NEW_PACK  =16
};
}
enum spi_which_e{
  TDC,
  VGA
};

// buffer, only one
extern char spi_pack[SPI_BUFSIZE];
// is the index of the last byte transmited as
// well as where goes next byte received
// it can be used to determine when the transmission has ended
extern uint8_t spi_point;

void spi_master_init();
void spi_tx(const spi_which_e which);
inline uint8_t spi_available(){
  return spi_reg&spi_reg_e::NEW_PACK;
}
inline uint8_t spi_canwrite(){
  return spi_point==SPI_BUFSIZE;
}

#endif //UART_H

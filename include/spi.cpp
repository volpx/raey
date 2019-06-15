#include "spi.h"

//Variable declaration
uint8_t spi_pack_size=8;
SPIWhich spi_which;
uint8_t spi_point=0;
uint8_t spi_pack[SPI_MAXBUFSIZE];

void spi_master_init(){
  // Disable module power reduction
  PRR&=~(1<<PRSPI);
  // set MOSI and SSN0 as output
  DDRB|=(1<<DDB5)|(1<<DDB3)|(1<<DDB2);
  // Enable SPI as a master and set clock
  SPCR = (1<<SPIE)|(0<<DORD)|(1<<SPE)|
    (1<<MSTR)|(1<<SPR0)|(0<<CPOL)|(0<<CPHA)|
    (0x02);
  SPSR = (1<<SPI2X);
}
void spi_tx(const SPIWhich which){
  // Before entering here the pack must be filled with the data
  // Pull down the slave correct select
  spi_which=which;
  switch(spi_which){
    case SPIWhich::VGA:
      //TODO: see if the SS pin is pulled low anyway
      //PORTB&=~PB2;
      break;
    case SPIWhich::TDC:
      //TODO: define a port
      //DDRB&=~(1<<which);
      break;
  }
  // Start transmission
  spi_point=0;
  SPDR = spi_pack[spi_point];
}
ISR(SPI_STC_vect){
  // // check if the MSTR bit has been cleared by someone pulling SS low
  // if (!(SPCR & MSTR)){
  //   // am I a slave now?
  //   // no because the ss is an output
  // }
  // read the data
  spi_pack[spi_point]=SPDR;
  // manage the data
  // check if the last byte in pack has been transmitted
  spi_point++;
  if (spi_point<spi_pack_size){
    // continue transmission with next byte
    SPDR=spi_pack[spi_point];
  } else {
    // end of pack
    // pull up the ss
    switch(spi_which){
      case SPIWhich::VGA:
        PORTB|=PB2;
        break;
      case SPIWhich::TDC:
        //TODO: define a port
        //DDRB|=(1<<which);
        break;
    }
  }
}

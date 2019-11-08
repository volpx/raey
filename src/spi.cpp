#include "spi.h"

Spi spi=Spi();

Spi::Spi():
  max_size_(SPI_BUFSIZE)
  {
  size_= 0;
  which_=SPIWhich::NONE;
  point_=0;

}

void Spi::init(){
  // Disable module power reduction
  PRR&=~(1<<PRSPI);
  // set MOSI and clock as output
  DDRB|=(1<<DDB5)|(1<<DDB3);
  // set pullup on my cs
  PORTB|=(1<<PB2);
  // Enable SPI as a master and set clock
  SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|
    (1<<MSTR)|(0<<CPOL)|(0<<CPHA)|
    (0x02);
  //SPSR = (1<<SPI2X);

  // set the cs outputs
  DDRD|=(1<<TDC_CS)|(1<<VGA_CS)|(1<<DAC_CS);
  // set high the cs s
  PORTD|=(1<<TDC_CS)|(1<<VGA_CS)|(1<<DAC_CS);
}

void Spi::tx(const SPIWhich which){
  tx(which,SPI_BUFSIZE);
}
void Spi::tx(const SPIWhich which,const uint8_t size){
  // set pack size
  if (size>max_size_)
    // exceed the maximum size
    return;
  size_=size;
  // Before entering here the pack must be filled with the data
  // Pull down the slave correct select
  which_=which;
  switch(which_){
    case SPIWhich::VGA:
      PORTD&=~(1<<VGA_CS);
      break;
    case SPIWhich::TDC:
      PORTD&=~(1<<TDC_CS);
      break;
    case SPIWhich::DAC:
      PORTD&=~(1<<DAC_CS);
      break;
    case SPIWhich::NONE: default:
      return;
  }
  // Reenable master operations
  SPCR|=(1<<MSTR);
  // Start transmission
  point_=0;
  SPDR = buf[point_];
}
bool Spi::available() const {
  return point_==size_;
}
void Spi::wait_available() const {
  while(!available())
    idle();
}
void Spi::reset(){
  point_=0;
}
uint8_t Spi::pack_size() const {
  return size_;
}

ISR(SPI_STC_vect){
  // // check if the MSTR bit has been cleared by someone pulling SS low
  // if (!(SPCR & MSTR)){
  //   // am I a slave now?
  // }
  // read the data
  spi.buf[spi.point_]=SPDR;
  // manage the data
  // check if the last byte in pack has been transmitted
  ++spi.point_;
  if (spi.point_<spi.size_){
    // continue transmission with next byte
    SPDR=spi.buf[spi.point_];
  }
  else {
    // end of pack
    // pull up the ss
    switch(spi.which_){
      case SPIWhich::VGA:
        PORTD|=(1<<VGA_CS);
        break;
      case SPIWhich::TDC:
        PORTD|=(1<<TDC_CS);
        break;
      case SPIWhich::DAC:
        PORTD|=(1<<DAC_CS);
        break;
      default:
        break;
    }
  }
}

#ifndef SPI_H
#define SPI_H

#include <avr/interrupt.h>
#include <stdint.h>

// packet size
#define SPI_BUFSIZE 8

extern "C" void SPI_STC_vect(void) __attribute__((signal));

// which select
enum class SPIWhich{
  NONE,
  TDC,
  VGA
};

class Spi {
public:
  explicit Spi();

  void tx(const SPIWhich which);
  void tx(const SPIWhich which, const uint8_t size);
  bool available() const;
  void reset();

private:
  uint8_t buf_[SPI_BUFSIZE];
  const uint8_t max_size_;
  volatile uint8_t point_;
  SPIWhich which_;
  uint8_t size_;
  friend void SPI_STC_vect();

};

#endif //SPI_H

#ifndef SPI_H
#define SPI_H

#include <avr/interrupt.h>
#include <stdint.h>

extern void idle();

// chip selects PORTD
#define TDC_CS      3
#define VGA_CS      6
#define DAC_CS      4

// packet size
#define SPI_BUFSIZE 8

extern "C" void SPI_STC_vect(void) __attribute__((signal));

// which select
enum class SPIWhich{
  NONE,
  TDC,
  VGA,
  DAC
};

class Spi {
public:
  explicit Spi();
  void init();

  void tx(const SPIWhich which);
  void tx(const SPIWhich which, const uint8_t size);
  bool available() const;
  void wait_available() const;
  void reset();
  uint8_t pack_size() const;

  uint8_t buf[SPI_BUFSIZE];

private:
  const uint8_t max_size_;
  volatile uint8_t point_;
  volatile SPIWhich which_;
  uint8_t size_;
  friend void SPI_STC_vect();

};

extern Spi spi;

#endif //SPI_H

#ifndef UART_H
#define UART_H

#include "circular_buffer.h"

#include <avr/interrupt.h>
#include <stdint.h>

// char per command
#define UART_BUFSIZE 32
// total commands in buffer
// #define CMDN    10
// #define COMMAND_AVAILABLE (uart_reg & NEW_COMMAND)

//status bits
#define UART_NEW_COMMAND   0
#define UART_TX_PEN        1
#define UART_RX_PEN        2
#define UART_NEW_DATA      3
#define UART_RX_FULL       4

extern "C" void USART_TX_vect(void) __attribute__((signal));
extern "C" void USART_RX_vect(void) __attribute__((signal));

class Uart {
public:
  explicit Uart();

  void tx_enable();
  void tx_disable();
  bool rx_available() const;

  void tx_byte(const uint8_t data);
  void print(const char s[]);
  void tx_hex(const uint8_t data);
  void tx_uint(const uint32_t data);

  uint8_t rx();
private:
  uint8_t rx_buf_raw_[UART_BUFSIZE];
  uint8_t tx_buf_raw_[UART_BUFSIZE];
  CircularBufferV rx_buf_;
  CircularBufferV tx_buf_;
  volatile uint8_t reg_;
  friend void USART_TX_vect();
  friend void USART_RX_vect();
};

extern Uart uart;

// https://github.com/crapp/uartavr/blob/master/src/uart.h
#endif //UART_H

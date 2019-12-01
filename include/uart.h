#ifndef UART_H
#define UART_H

#include "circular_buffer.h"

#include <avr/interrupt.h>
#include <stdint.h>

#include "../raey.h"

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

  void init();

  void tx_enable();
  void tx_disable();

  void tx_byte(const uint8_t data);
  void print(const char s[]);
  void tx_hex(const uint8_t data);
  void tx_uint(const uint32_t data);
  void tx_float(const float f,const uint8_t dig=0);

  bool rx_available() const;
  bool rx_available_command() const;
  void rx_command(char s[]);
  uint8_t rx_byte();

private:
  uint8_t rx_buf_raw_[UART_BUFSIZE];
  uint8_t tx_buf_raw_[UART_BUFSIZE];
  CircularBuffer rx_buf_;
  CircularBuffer tx_buf_;
  volatile uint8_t reg_;
  volatile uint8_t commands_in_;
  friend void USART_TX_vect();
  friend void USART_RX_vect();
};

extern Uart uart;

// https://github.com/crapp/uartavr/blob/master/src/uart.h
#endif //UART_H

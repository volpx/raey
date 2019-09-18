#ifndef UART_H
#define UART_H

#include "../raey.h"

// char per command
#define BUFSIZE 32
// total commands in buffer
// #define CMDN    10
// #define COMMAND_AVAILABLE (uart_reg & NEW_COMMAND)

//status
extern volatile uint8_t uart_reg;
#define NEW_COMMAND   1
#define TX_PEN        2
#define RX_PEN        4
#define NEW_DATA      8
#define RX_FULL       16

//single command buffer
extern char buf_rx[BUFSIZE];
extern volatile uint8_t buf_rx_head;
extern volatile uint8_t buf_rx_tail;
extern char buf_tx[BUFSIZE];
extern volatile uint8_t buf_tx_head;
extern volatile uint8_t buf_tx_tail;


void uart_init();
void uart_print(const char *s);
void uart_byte(const char c);
uint8_t uart_rx();
void uart_hex(const uint8_t n);
//void uart_read();
void uart_uint(const uint8_t n);

inline bool uart_rx_available(){
  return uart_reg&NEW_DATA;
}
inline void enable_tx(){
  uart_reg|=TX_PEN;
}
inline void disable_tx(){
  uart_reg&=~TX_PEN;
}

#endif //UART_H

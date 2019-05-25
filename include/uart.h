#ifndef UART_H
#define UART_H

#include <stdint.h>
//#include <avr/io.h>
#include <avr/interrupt.h>

#include "util.h"

// char per command
#define BUFSIZE 16
// total commands in buffer
#define CMDN    10
// #define COMMAND_AVAILABLE (uart_reg & NEW_COMMAND)

//status
extern uint8_t uart_reg;
#define NEW_COMMAND   1
#define TX_PEN        2
#define RX_PEN        4
#define NEW_DATA      8
#define RX_FULL       16

//single command buffer
extern char buf_rx[BUFSIZE];
extern uint8_t buf_rx_head;
extern uint8_t buf_rx_tail;
extern char buf_tx[BUFSIZE];
extern uint8_t buf_tx_head;
extern uint8_t buf_tx_tail;


void uart_init();
void uart_print(const char *s);
uint8_t uart_rx();
void uart_byte(const char c);
void uart_read();

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

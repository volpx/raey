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
#define COMMAND_AVAILABLE (uart_reg & NEW_COMMAND)

//status
extern uint8_t uart_reg;
#define NEW_COMMAND   0
#define TX_PEN        1
#define RX_PEN        2
#define NEW_DATA      3
#define RX_FULL       4

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
void uart_tx(char c);
void uart_read();

#endif //UART_H

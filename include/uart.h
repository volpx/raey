#ifndef UART_H
#define UART_H

#include <stdint.h>
//#include <avr/io.h>
#include <avr/interrupt.h>

#define MAX_CMD_SIZE 10
#define BUFSIZE 10
#define COMMAND_AVAILABLE (uart_reg & NEW_COMMAND)

//status
extern uint8_t uart_reg;
#define NEW_COMMAND   0
#define TX_PEN        1
#define RX_PEN        2

//single command buffer
extern uint8_t buf_rx[MAX_CMD_SIZE];
extern uint8_t buf_rx_ind;
extern uint8_t buf_tx[MAX_CMD_SIZE];
extern uint8_t buf_tx_ind;
extern uint8_t buf_tx_out;


void uart_init();
void uart_print(const uint8_t *s);
uint8_t uart_rx();
void uart_tx(uint8_t c);
void uart_read();

#endif //UART_H

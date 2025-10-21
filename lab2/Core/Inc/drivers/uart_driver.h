#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "main.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define UART_RX_BUF_SIZE 128
#define UART_TX_BUF_SIZE 128
#define UART_MAX_LINE 128
#define UART_TIMEOUT 100

void uart_set_interrupts(uint8_t enabled);
uint8_t uart_get_interrupts(void);

void uart_print_char(char ch);
void uart_print_string(char *str);
void uart_println_string(char *str);

void uart_receive_line_task(void);
void process_command_line(const char *line);

#endif



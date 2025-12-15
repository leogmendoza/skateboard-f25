#pragma once

#include <stdint.h>

#define UART_BAUD 9600UL
#define UART_UBRR_VALUE ((F_CPU / (16UL * UART_BAUD)) - 1)

void uart_init(void);
void uart_write_byte(uint8_t data);
void uart_write_str(const char *s);

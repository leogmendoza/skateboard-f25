#include "uart.h"

#include <avr/io.h>
#include <stdint.h>

void uart_init(void) {
    // Set baud rate
    UBRR0H = (uint8_t)(UART_UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(UART_UBRR_VALUE & 0xFF);

    UCSR0B = (1 << TXEN0);  // Enable transmitter only (TX on PD1)

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Frame format: 8 data bits, no parity, 1 stop bit (8N1)
}

void uart_write_byte(uint8_t data) {
    // Wait until transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0))) {
        // spin
    }

    // Put data into buffer, sending the byte
    UDR0 = data;
}

void uart_write_str(const char *string) {
    while (*string) {
        uart_write_byte((uint8_t)*string++);
    }
}

#include "utils.h"

#include <avr/interrupt.h>

/* GPIO */

void utils_gpio_set_output(volatile uint8_t *ddr, uint8_t bit) {
    *ddr |= (1 << bit);
}

void utils_gpio_set_input(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, bool pullup_enable) {
    *ddr &= ~(1 << bit);  // Clear bit to set as input

    if (pullup_enable) {
        *port |= (1 << bit);
    } else {
        *port &= ~(1 << bit);  // Clear bit to disable pullup
    }
}

void utils_gpio_write(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, bool logic_level) {
    if (*ddr & (1 << bit)) {  // Check if set as input
        if (logic_level) {
            *port |= (1 << bit);
        } else {
            *port &= ~(1 << bit);  // Clear bit to write a LOW signal
        }
    }
}

bool utils_gpio_read(volatile uint8_t *pin, uint8_t bit) {
    return (*pin & (1 << bit)); 
}

/* CLOCK & TIMING */

void utils_timer1_init(void) {
    TCCR1A = (0 << COM1A1) | (0 << COM1A0) |  // Normal port operation, OC1A/OC1B disconnected
             (0 << WGM11) | (0 << WGM10);  // CTC (Mode 4)

    TCCR1B = (0 << WGM13) | (1 << WGM12) |  // CTC, TOP in OCR1A (Mode 4)
             (0 << CS12) | (1 << CS11) | (0 << CS10);  // clk_IO / 8 (from prescaler)

    OCR1A = TIMER1_TOP_COUNT;  // 999

    TIMSK1 = (1 << OCIE1A);  // TIMER1_COMPA_vect executed 
}
#include "utils.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

static volatile uint32_t uptime_ms = 0;

// Lookup table for output compare registers to change PWM duty cycle of TAIL LIGHTS
static volatile uint8_t* const OCR_REG[] = {
    &OCR0A,
    &OCR0B,
    &OCR2A,
    &OCR2B,
};

/* Configured by utils_timer1_init() to trigger every 1 ms */
ISR(TIMER1_COMPA_vect) {
    uptime_ms++;
}

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
    if (*ddr & (1 << bit)) {  // Check if set as output
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

    TIMSK1 |= (1 << OCIE1A);  // TIMER1_COMPA_vect executed 
}

uint32_t utils_uptime_ms(void) {
    uint32_t value;

    cli();  // CRITICAL SECTION BEGINS
    value = uptime_ms;  // Prevent interrupts from manipulating the 32-bit uptime value while reading it
    sei();  // CRITICAL SECTION ENDS

    return value;
}

// bool utils_has_time_elapsed(uint32_t prev_time_ms, uint16_t wait_period_ms) {
//     if ( (utils_uptime_ms() - prev_time_ms) >= wait_period_ms ) {
//         return true;
//     }

//     return false;
// }

void utils_timer0_init(void) {
    // Check if OC0A at PD6 is set as an output
    if ((DDRD & (1 << PD6)) == 0) {
        printf("PD6 is not set as output. Cannot initialize PWM through OC0A.");

        return;
    }

    // Check if OC0B at PD5 is set as an output
    if ((DDRD & (1 << PD5)) == 0) {
        printf("PD5 is not set as output. Cannot initialize PWM through OC0B.");

        return;
    }

    TCCR0A = (1 << COM0A1) | (0 << COM0A0) |  // Clear OC0A on compare match, set OC0A at BOTTOM, (non-inverting mode)
             (1 << COM0B1) | (0 << COM0B0) |  // Clear OC0B on compare match, set OC0B at BOTTOM, (non-inverting mode)
             (1 << WGM01) | (1 << WGM00);  // Fast PWM, TOP = 0xFF = 255 (mode 3)

    TCCR0B = (0 << FOC0A) | (0 << FOC0B) |  // Forcibly set to 0 to ensure compatibility with PWM mode
             (0 << WGM02) |  // Fast PWM (mode 3)
             (0 << CS02) | (1 << CS01) | (1 << CS00);  // Prescaler = 64

    // Explicitly clear to ensure PWM starts at LOW (these will be manipulated during runtime for duty cycle)
    OCR0A = 0;
    OCR0B = 0;
}

void utils_timer2_init(void) {
    // Check if OC2A at PB3 is set as an output
    if ((DDRB & (1 << PB3)) == 0) {
        printf("PB3 is not set as output. Cannot initialize PWM through OC2A.");

        return;
    }

    // Check if OC2B at PD3 is set as an output
    if ((DDRD & (1 << PD3)) == 0) {
        printf("PD3 is not set as output. Cannot initialize PWM through OC2B.");

        return;
    }

    TCCR2A = (1 << COM2A1) | (0 << COM2A0) |  // Clear OC2A on compare match, set OC0A at BOTTOM, (non-inverting mode)
             (1 << COM2B1) | (0 << COM2B0) |  // Clear OC2B on compare match, set OC0B at BOTTOM, (non-inverting mode)
             (1 << WGM21) | (1 << WGM20);  // Fast PWM, TOP = 0xFF = 255 (mode 3)

    TCCR2B = (0 << FOC2A) | (0 << FOC2B) |  // Forcibly set to 0 to ensure compatibility with PWM mode
             (0 << WGM22) |  // Fast PWM (mode 3)
             (0 << CS22) | (1 << CS21) | (1 << CS20);  // Prescaler = 64

    // Explicitly clear to ensure PWM starts at LOW (these will be manipulated during runtime for duty cycle)
    OCR2A = 0;
    OCR2B = 0;
}

void utils_pwm_set_duty_cycle(PwmChannel channel, uint8_t duty_cycle) {
    if (channel >= NUM_PWM_CH) {
        printf("PWM channel out of range.");
        return;
    }
    *OCR_REG[channel] = duty_cycle;  // Call lookup table instead of switch-case block to reduce code duplication
}

void utils_pwm_disable(PwmChannel channel) {
    switch(channel) {
        // Clear the Compare Output Mode bits to disconnect port from PWM
        case PWM_CH_OC0A:
            TCCR0A &= ~((1 << COM0A0) | (1 << COM0A1));  // Normal port operation, OC0A disconnected
            break;
        case PWM_CH_OC0B:
            TCCR0A &= ~((1 << COM0B0) | (1 << COM0B1));  // Normal port operation, OC0B disconnected
            break;
        case PWM_CH_OC2A:
            TCCR2A &= ~((1 << COM2A0) | (1 << COM2A1));  // Normal port operation, OC2A disconnected
            break;
        case PWM_CH_OC2B:
            TCCR2A &= ~((1 << COM2B0) | (1 << COM2B1));  // Normal port operation, OC2B disconnected
            break;
    }
}
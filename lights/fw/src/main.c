#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "utils.h"
#include "i2c.h"
#include "head.h"
#include "tail.h"

#if ENABLE_UART_DEBUG
    #include "uart.h"   
#endif

int main(void) {
    // Note: pins are reverse from main 1x8 to BOTH light boards' 1x8 cuz of crimped wires
    #if ENABLE_PRODUCTION_CODE
        // Initialize GPIO pins of Tail and Head LEDs
        for (uint8_t i = 0; i < NUM_TAIL_LEDS; i++) {
            utils_gpio_set_output(tail_ddr_sequence[i], tail_port_sequence[i], tail_bit_sequence[i]);
        }
        for (uint8_t i = 0; i < NUM_HEAD_LEDS; i++) {
            utils_gpio_set_output(HEAD_LEDS_DDR, HEAD_LEDS_PORT, head_bit_sequence[i]);
        }

        utils_pwm_reset_timers();

        utils_timer1_init();  // Enable timer for system timing

        // Enable timers for PWM
        utils_timer0_init();
        utils_timer2_init();

        i2c_init();  // Enable I2C for accelerometer

        #if ENABLE_UART_DEBUG
            uart_init();
        #endif

        sei();  // Enable global interrupts 

        uint32_t curr_time_ms;
        uint32_t prev_time_ms;

        HeadLight head_light;
        TailLight tail_light;

        head_light_init(&head_light);
        tail_light_init(&tail_light);

        prev_time_ms = utils_uptime_ms();

        while (1) {
            curr_time_ms = utils_uptime_ms();

            if ( (curr_time_ms - prev_time_ms) >= TASK_PERIOD_1000_HZ_IN_MS ) {
                prev_time_ms += TASK_PERIOD_1000_HZ_IN_MS;  // Robust timing

                head_light_update(&head_light);
                tail_light_update(&tail_light);
            }
        }
    #else
        /* ---------- FORCE ALL GATES LOW FIRST ---------- */

        // Tail: clear output latches

        PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD3));
        PORTB &= ~(1 << PB3);

        // Head: clear output latches
        PORTC &= ~((1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0));

        // Now configure as outputs
        DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
        DDRB |= (1 << PB3);
        DDRC |= (1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0);

        while (1) {

            /* ---------- ALL ON ---------- */
            PORTD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
            PORTB |= (1 << PB3);

            PORTC |= (1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0);

            _delay_ms(500);

            /* ---------- ALL OFF ---------- */
            PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD3));
            PORTB &= ~(1 << PB3);

            PORTC &= ~((1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0));

            _delay_ms(500);
        }
    #endif
}
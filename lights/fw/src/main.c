#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"
#include "uart.h"
#include "i2c.h"
#include "head.h"
#include "tail.h"

int main(void) {
    utils_timer1_init();
    utils_timer0_init();
    utils_timer2_init();
    uart_init();
    i2c_init();

    sei();  // Enable global interrupts 

    uint32_t curr_time_ms;
    uint32_t prev_time_ms;

    HeadLight head_light;
    TailLight tail_light;

    head_light_init(&head_light);
    tail_light_init(&tail_light);

    prev_time_ms = utils_uptime_ms();

    uart_write_str("hello\r\n");

    while (1) {
        curr_time_ms = utils_uptime_ms();

        if ( (curr_time_ms - prev_time_ms) >= TASK_PERIOD_1000_HZ_IN_MS ) {
            prev_time_ms = curr_time_ms;

            head_light_update(&head_light);
            tail_light_update(&tail_light);
        }
    }

    // DDRB = (1 << PB5);

    // while(1) {
    //     PORTB = (1 << PB5);
    //     _delay_ms(500);

    //     PORTB = (0 << PB5);
    //     _delay_ms(500);
    // } 
}
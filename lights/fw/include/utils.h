#pragma once

#include <stdint.h>

/* GPIO */
void utils_gpio_set_output(volatile uint8_t *ddr, uint8_t bit);  // Note: DDR, PORT, PIN are declared "volatile" since they point to hardware registers, NOT typical variables in memory
void utils_gpio_set_input(volatile uint8_t *ddr, uint8_t bit);
void utils_gpio_write_high(volatile uint8_t *port, uint8_t bit);  // Note: "PORT" is for reading inputs
void utils_gpio_write_low(volatile uint8_t *port, uint8_t bit);
void utils_gpio_read(volatile uint8_t *pin, uint8_t bit);  // Note: "PIN" is for reading outputs (counterintuitively, a PIN does not refer to the individual bits of a PORT)

/* PWM */
void utils_pwm_init(void);  // Configure Timer1, Timer2 for PWM
void utils_pwm_set_duty_cycle(uint8_t channel, uint8_t duty);

/* Clock & Timing */
void utils_timer0_init(void);  // Configure Timer0 for 1 ms tick
uint32_t utils_uptime_ms(void);  // Custom implementation of millis() from Arduino framework, which counts time since the system booted up
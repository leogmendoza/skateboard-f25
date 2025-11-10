#pragma once

#include <stdint.h>

/* GPIO */

/**
 * @brief Set pin as output
 * @note DDR, PORT, PIN are declared "volatile" since they point to hardware registers, NOT typical variables in memory
 */
void utils_gpio_set_output(volatile uint8_t *ddr, uint8_t bit);

/**
 * @brief Set pin as input
 */
void utils_gpio_set_input(volatile uint8_t *ddr, uint8_t bit);

/**
 * @brief Drive output pin high
 * @note "PORT" is for reading inputs
 */
void utils_gpio_write_high(volatile uint8_t *port, uint8_t bit);

/**
 * @brief Drive output pin low
 */
void utils_gpio_write_low(volatile uint8_t *port, uint8_t bit);

/**
 * @brief Read logical level from input pin
 * @note "PIN" is for reading outputs (counterintuitively, a PIN does not refer to the individual bits of a PORT)

 */
void utils_gpio_read(volatile uint8_t *pin, uint8_t bit);
/* PWM */

/**
 * @brief Initialize PWM timers (Timer1, Timer2) for LED control
 */
void utils_pwm_init(void);

/**
 * @brief Set PWM duty cycle (0–255) for a channel
 */
void utils_pwm_set_duty_cycle(uint8_t channel, uint8_t duty);

/* Clock & Timing */

/**
 * @brief Configure Timer0 for 1 ms system tick
 *
void utils_timer0_init(void);

/**
 * @brief Return system uptime in milliseconds since reset
 * @note This is a custom implementation of millis() from Arduino framework
 */
uint32_t utils_uptime_ms(void);
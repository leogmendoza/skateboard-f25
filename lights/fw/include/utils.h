#pragma once

#include <stdint.h>
#include <stdbool.h>

#define F_CPU_HZ F_CPU
#define SYSTEM_TICK_INTERVAL_S 0.001
#define TOTAL_CYCLES (F_CPU_HZ * SYSTEM_TICK_INTERVAL_S) // Ticks to count for 1 ms interval
#define PRESCALER 8
// Calculate the exact value for the Output Compare Register (OCR1A/TOP)
#define TIMER1_TOP_COUNT ((TOTAL_CYCLES / PRESCALER) - 1UL) // 999 => 125 us Timer1 tick period

/* GPIO */

/**
 * 13.2.1 Configuring the Pin
 * Each port pin consists of three register bits: DDxn, PORTxn, and PINxn. As shown in Section 13.4 “Register Description” on
 * page 72, the DDxn bits are accessed at the DDRx I/O address, the PORTxn bits at the PORTx I/O address, and the PINxn
 * bits at the PINx I/O address.
 * The DDxn bit in the DDRx register selects the direction of this pin. If DDxn is written logic one, Pxn is configured as an output
 * pin. If DDxn is written logic zero, Pxn is configured as an input pin.
 * If PORTxn is written logic one when the pin is configured as an input pin, the pull-up resistor is activated. To switch the
 * pull-up resistor off, PORTxn has to be written logic zero or the pin has to be configured as an output pin. The port pins are
 * tri-stated when reset condition becomes active, even if no clocks are running.
 * If PORTxn is written logic one when the pin is configured as an output pin, the port pin is driven high (one). If PORTxn is
 * written logic zero when the pin is configured as an output pin, the port pin is driven low (zero)
 */

/**
 * @brief Set pin as output
 * @note DDR, PORT, PIN are declared "volatile" since they point to hardware registers, NOT typical variables in memory
 */
void utils_gpio_set_output(volatile uint8_t *ddr, uint8_t bit);

/**
 * @brief Set pin as input and configure internal pull-up resistor, if needed
 */
void utils_gpio_set_input(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, bool pullup_enable);

/**
 * @brief Drive output pin high or low
 * @note "PORT" is for reading inputs
 */
void utils_gpio_write(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, bool logic_level);

/**
 * @brief Read logical level from input pin
 * @note "PIN" is for reading outputs (counterintuitively, a PIN does not refer to the individual bits of a PORT)

 */
bool utils_gpio_read(volatile uint8_t *pin, uint8_t bit);

/* CLOCK & TIMING */

/**
 * @brief Configure Timer1 to generate a 1 ms system tick interrupt
 *
 * Timer1 runs in CTC mode with OCR1A set for 1 ms interrupts.
 * Used for uptime tracking in utils_uptime_ms().
 * 
 * Uses pin 13 on-chip/PB1.
 */
void utils_timer1_init(void);

/**
 * @brief Return system uptime in milliseconds since reset
 */
uint32_t utils_uptime_ms(void);

/* PWM */

/**
 * @brief Configure Timer0 for PWM output (OC0A/OC0B)
 *
 * Sets Timer0 in Fast PWM, non-inverting mode, ~1 kHz frequency.
 * Typical pins: PD6 (OC0A) and PD5 (OC0B).
 */
void utils_timer0_init(void);

/**
 * @brief Configure Timer2 for PWM output (OC2A/OC2B)
 *
 * Sets Timer2 in Fast PWM, non-inverting mode, ~1 kHz frequency.
 * Typical pins: PB3 (OC2A) and PD3 (OC2B).
 */
void utils_timer2_init(void);

/**
 * @brief Set PWM duty cycle (0–255) for a specific hardware channel
 *
 * @param channel  PWM channel index (0=OC0A, 1=OC0B, 2=OC2A, 3=OC2B)
 * @param duty     Duty cycle (0–255)
 */
void utils_pwm_set_duty_cycle(uint8_t channel, uint8_t duty);

/**
 * @brief Stop PWM output on a specific channel
 *
 * @param channel  PWM channel index (0–3)
 */
void utils_pwm_disable(uint8_t channel);

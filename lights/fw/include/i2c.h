#pragma once

#include <stdint.h>
#include <stdbool.h>

#define I2C_ADDR_LEN 7
#define I2C_READ_BIT 1
#define I2C_WRITE_BIT 0

/**
 * @brief Initialize AVR TWI (I2C) hardware.
 * 
 * Read ATMega datasheet Section 21.5 for TWBR/TWSR setup.
 */
void i2c_init(void);

/**
 * @brief Send START condition and wait for TWINT. 
 * 
 * See Section 21.6.3 (Master Transmitter Mode) for TWCR flags.
 */
void i2c_start(void);

/**
 * @brief Send STOP condition (TWSTO = 1).
 * 
 * See Section 21.6.5.
 */
void i2c_stop(void);

/**
 * @brief Send slave address byte (7-bit << 1 | R/W bit).
 * 
 * Use TWI Status Codes in Table 21-2 to verify ACK.
 */
void i2c_send_address(uint8_t address_byte);

/**
 * @brief Write a single data byte; wait for ACK.
 * 
 * See Section 21.6.3 (load TWDR, clear TWINT).
 */
void i2c_write(uint8_t data);

/**
 * @brief Read a byte and ACK it (for multi-byte reads).
 * 
 * See Section 21.6.4 (Master Receiver Mode).
 */
uint8_t i2c_read_ack(void);

/**
 * @brief Read a byte and NACK it (for final byte).
 * 
 * See Section 21.6.4 (send NACK by clearing TWEA).
 */
uint8_t i2c_read_nack(void);

/**
 * @brief Convenience: write 1 byte to a device register.
 * 
 * Sequence: START -> addr(W) -> reg -> value -> STOP.
 */
void i2c_write_register(uint8_t slave_addr, uint8_t slave_reg, uint8_t value);

/**
 * @brief Convenience: read 1 byte from a device register.
 * 
 * Sequence: START -> addr(W) -> reg -> RESTART -> addr(R) -> byte -> NACK+STOP.
 */
uint8_t i2c_read_register(uint8_t slave_addr, uint8_t slave_reg);

/**
 * @brief Convenience: read multiple sequential registers.
 * 
 * Use repeated START and ACK all but final byte.
 */
void i2c_read_multi(uint8_t dev_addr, uint8_t start_reg, uint8_t *buf, uint8_t len);

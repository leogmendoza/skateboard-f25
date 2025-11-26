#pragma once

#include <stdint.h>
#include <stdbool.h>

#define ADXL343_I2C_SLAVE_ADDR_ALT 0x53  // ALT_ADDRESS is pulled down to GND, so using alternate address rather than default
#define ADXL343_I2C_WRITE_BYTE ((ADXL343_I2C_SLAVE_ADDR_ALT << 1) | 0)  // 0x3A
#define ADXL343_I2C_READ_BYTE ((ADXL343_I2C_SLAVE_ADDR+_ALT << 1) | 1)  // 0x3B

// Note: I2C will be abstracted away in these functions

/**
 * @brief Initialize ADXL343 over I2C and verify device identity
 */
bool adxl343_init(void);

/**
 * @brief Read latest acceleration data from the sensor's 6 data registers (DATAX0–DATAZ1) and update cached axis values
 */
void adxl343_update(void);

/**
 * @brief Retrieve most recent raw (unclear about this rn) acceleration values
 * 
 * @param ax Pointer to receive X-axis raw acceleration.
 * @param ay Pointer to receive Y-axis raw acceleration.
 * @param az Pointer to receive Z-axis raw acceleration.
 */
void adxl343_get_acceleration(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Determine whether the current acceleration indicates deceleration beyond the configured threshold
 */
bool adxl343_is_braking(void);

/**
 * @brief Adjust braking detection threshold in milli-g units
 * 
 * @param threshold_mg Deceleration magnitude (in mg) required to report braking
 */
void adxl343_set_brake_threshold(int16_t threshold_mg);
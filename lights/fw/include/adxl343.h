#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "i2c.h"

// I2C
#define ADXL343_I2C_SLAVE_ADDR_ALT 0x53  // ALT_ADDRESS is pulled down to GND, so using alternate address rather than default

// Registers
#define ADXL343_REG_DEVID 0x00
#define ADXL343_REG_BW_RATE 0x2C
    #define ADXL343_REG_BW_RATE_RATE_3 3
    #define ADXL343_REG_BW_RATE_RATE_2 2
    #define ADXL343_REG_BW_RATE_RATE_1 1
    #define ADXL343_REG_BW_RATE_RATE_0 0
#define ADXL343_REG_POWER_CTL 0x2D
    #define ADXL343_REG_POWER_CTL_MEASURE 3
#define ADXL343_REG_DATA_FORMAT 0x31
    #define ADXL343_REG_DATA_FORMAT_FULL_RES 3
    #define ADXL343_REG_DATA_FORMAT_RANGE_1 1
    #define ADXL343_REG_DATA_FORMAT_RANGE_0 0

// Register Values
#define ADXL343_DEVICE_ID 0xE5

// Note: I2C will be abstracted away in these functions

/**
 * @brief Initialize ADXL343 over I2C and verify device identity
 * 
 * Device ID register = 0x00, must read 0xE5 (Table 20, page 23).
 * Output data rate via BW_RATE (0x2C) (Table 25, page 24).
 * Measurement enable via POWER_CTL (0x2D), Measure bit D3 (page 25).
 * Full-resolution, ±2g via DATA_FORMAT (0x31) (page 26).
 */
bool adxl343_init(void);

/**
 * @brief Read latest acceleration data from the sensor's 6 data registers (DATAX0–DATAZ1) and update cached axis values
 * 
 * Data registers described on page 26 (“Register 0x32–0x37—DATAX0 to DATAZ1”).
 * Must burst-read all 6 bytes (page 26).
 */
void adxl343_update(void);

/**
 * @brief Retrieve most recent raw (unclear about this rn) acceleration values
 * 
 * @param ax Pointer to receive X-axis raw acceleration.
 * @param ay Pointer to receive Y-axis raw acceleration.
 * @param az Pointer to receive Z-axis raw acceleration.
 * 
 * Data is 16-bit, two’s complement, little-endian (page 26).
 * Full-resolution scale = 256 LSB/g → 3.9 mg/LSB (Table 1, page 3).
 */
void adxl343_get_acceleration(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Determine whether the current acceleration indicates deceleration beyond the configured threshold
 * 
 * Activity thresholds use scale 62.5 mg/LSB (Registers THRESH_ACT, page 23).
 * Raw acceleration signs & axes orientation shown in Figure 47 (page 34) and Figure 48 (page 34).
 */
bool adxl343_is_braking(void);

/**
 * @brief Adjust braking detection threshold in milli-g units
 * 
 * @param threshold_mg Deceleration magnitude (in mg) required to report braking
 * 
 * 1 LSB = 3.9 mg in full-resolution mode (Table 1, page 3).
 * You are NOT writing this into THRESH_ACT; this is a firmware-side threshold.
 */
void adxl343_set_brake_threshold(int16_t threshold_mg);
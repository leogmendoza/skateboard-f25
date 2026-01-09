#include "adxl343.h"

#include <stdlib.h>

#include "i2c.h"

static int16_t ax_raw = 0;
static int16_t ay_raw = 0;
static int16_t az_raw = 0;

static int16_t brake_threshold_lsb = 0;  
static int16_t stationary_threshold_lsb = 0;

bool adxl343_init(void) {
    // Verify Device ID
    uint8_t device_id = i2c_read_register_single(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_DEVID);
    if (device_id != ADXL343_DEVICE_ID) {
        return false;
    }

    // Power Control: Ensure starting mode is correct
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_POWER_CTL, 
        (0 << ADXL343_REG_POWER_CTL_MEASURE)  // Standby Mode
    );

    // Data Format
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_DATA_FORMAT, (
        (1 << ADXL343_REG_DATA_FORMAT_FULL_RES) |  // Full Resolution for 256 LSB/g no matter what range is chosen
        (0 << ADXL343_REG_DATA_FORMAT_RANGE1) | (0 << ADXL343_REG_DATA_FORMAT_RANGE0)  // +/- 2g to eliminate as much noise as possible (and acceleration isn't too high)
    ));

    // Bandwidth-Rate: Just the default (0x0A)
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_BW_RATE, (
        (1 << ADXL343_REG_BW_RATE_RATE3) | (0 << ADXL343_REG_BW_RATE_RATE2) | (1 << ADXL343_REG_BW_RATE_RATE1) | (0 << ADXL343_REG_BW_RATE_RATE0)  // Output Data Rate = 100 Hz => Bandwidth = 50 Hz
        // I_DD = 140 uA
    ));

    // Power Control, again: Begin measuring!
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_POWER_CTL, 
        (1 << ADXL343_REG_POWER_CTL_MEASURE)  // Measurement Mode
    );

    return true;
}

void adxl343_update(void) {
    uint8_t buffer[6];  // Six bytes to be read (2 bytes for each axis)
    i2c_read_register_multiple(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_DATAX0, buffer, 6);

    // DATAx1 holds the MSB
    ax_raw = (int16_t)( (buffer[1] << 8) | buffer[0] );
    ay_raw = (int16_t)( (buffer[3] << 8) | buffer[2] );
    az_raw = (int16_t)( (buffer[5] << 8) | buffer[4] );
}

void adxl343_get_acceleration(int16_t *ax, int16_t *ay, int16_t *az) {
    *ax = ax_raw;
    *ay = ay_raw;
    *az = az_raw;
}

void adxl343_set_thresholds(int16_t brake_threshold_mg, int16_t stationary_threshold_mg) {
    // Clamp negative inputs
    if (brake_threshold_mg < 0) {
        brake_threshold_mg = -brake_threshold_mg;
    }
    if (stationary_threshold_mg < 0) {
        stationary_threshold_mg = -stationary_threshold_mg;
    }

    // Convert mg to LSB (allowing truncation)
    // Note: LSB = (mg * 256) / 1000
    int32_t temp1 = (int32_t)(brake_threshold_mg * 256);
    brake_threshold_lsb = (int16_t)(temp1 / 1000);
    int32_t temp2 = (int32_t)(stationary_threshold_mg * 256);
    stationary_threshold_lsb = (int16_t)(temp2 / 1000);

    // Clamp to magnitude-only outputs
    if (brake_threshold_lsb < 0) {
        brake_threshold_lsb = -brake_threshold_lsb;  
    }
    if (stationary_threshold_mg < 0) {
        stationary_threshold_lsb = -stationary_threshold_mg;
    }
}

bool adxl343_is_braking(void) {
    // Note: We only care about the X-direction. Also, ax_raw is in LSBs, 2's complement.
    return (ax_raw <= -(brake_threshold_lsb));
}

bool adxl343_is_stationary(void) {
    return(abs(ax_raw) < stationary_threshold_lsb);
}   

int16_t adxl343_get_brake_threshold_lsb(void) {
    return brake_threshold_lsb;
}

int16_t adxl343_get_stationary_threshold_lsb(void) {
    return stationary_threshold_lsb;
}

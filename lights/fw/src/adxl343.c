#include "adxl343.h"

static int16_t ax_raw = 0;
static int16_t ay_raw = 0;
static int16_t az_raw = 0;

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
        (0 << ADXL343_REG_DATA_FORMAT_RANGE_1) | (0 << ADXL343_REG_DATA_FORMAT_RANGE_0)  // +/- 2g to eliminate as much noise as possible (and acceleration isn't too high)
    ));

    // Bandwidth-Rate: Just the default (0x0A)
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_BW_RATE, (
        (1 << ADXL343_REG_BW_RATE_RATE_3) | (0 << ADXL343_REG_BW_RATE_RATE_2) | (1 << ADXL343_REG_BW_RATE_RATE_1) | (0 << ADXL343_REG_BW_RATE_RATE_0)  // Output Data Rate = 100 Hz => Bandwidth = 50 Hz
        // I_DD = 140 uA
    ));

    // Power Control, again: Begin measuring!
    i2c_write_register(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_POWER_CTL, 
        (1 << ADXL343_REG_POWER_CTL_MEASURE)  // Standby Mode
    );
}

void adxl343_update(void) {
    int8_t buffer[6];  // Six bytes to be read (2 bytes for each axis)
    i2c_read_register_multiple(ADXL343_I2C_SLAVE_ADDR_ALT, ADXL343_REG_DATAX0, buffer, 6);

    // DATAx1 holds the MSB
    ax_raw = (int16_t)( (buffer[1] << 8) | buffer[0] );
    ax_raw = (int16_t)( (buffer[3] << 8) | buffer[2] );
    ax_raw = (int16_t)( (buffer[5] << 8) | buffer[4] );
}


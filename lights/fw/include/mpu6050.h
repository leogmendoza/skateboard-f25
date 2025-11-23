#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize MPU6050 over I²C and verify connection
 */
bool mpu6050_init(void);

/**
 * @brief Read latest acceleration data from the sensor
 */
void mpu6050_update(void);

/**
 * @brief Retrieve most recent raw acceleration values
 */
void mpu6050_get_acceleration(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Return true if measured deceleration exceeds threshold
 */
bool mpu6050_is_braking(void);

/**
 * @brief Adjust braking detection threshold in milli-g units
 */
void mpu6050_set_brake_threshold(int16_t threshold_mg);
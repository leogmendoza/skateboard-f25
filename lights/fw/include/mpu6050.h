#pragma once

#include <stdint.h>
#include <stdbool.h>

bool mpu6050_init(void);
void mpu6050_update(void);
void mpu6050_get_acceleration(int16_t *ax, int16_t *ay, int16_t *az);
bool mpu6050_is_braking(void);
void mpu6050_set_brake_threshold(int16_t threshold_mg);
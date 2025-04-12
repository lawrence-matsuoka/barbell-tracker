// mpu6050.c
#include <msp430.h>
#include "mpu6050.h"
#include "i2c.h"

#define MPU6050_ADDR 0x68

void mpu6050_init() {
    i2c_init(); // Use UCB1 (P4.1 SDA, P4.2 SCL)
    i2c_write_byte(MPU6050_ADDR, 0x6B, 0); // Wake up from sleep
}

void mpu6050_read_accel(float *ax, float *ay, float *az) {
    uint8_t data[6];
    i2c_read_bytes(MPU6050_ADDR, 0x3B, data, 6);

    int16_t ax_raw = (data[0] << 8) | data[1];
    int16_t ay_raw = (data[2] << 8) | data[3];
    int16_t az_raw = (data[4] << 8) | data[5];

    *ax = ax_raw / 16384.0;
    *ay = ay_raw / 16384.0;
    *az = az_raw / 16384.0;
}

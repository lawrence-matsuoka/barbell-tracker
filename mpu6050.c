#include "mpu6050.h"
#include "i2c.h"
#include "uart.h"
#include <msp430.h>

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define WHO_AM_I 0x75

void mpu6050_init(void) {
  unsigned char id = i2c_read_byte(MPU6050_ADDR, WHO_AM_I);

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "MPU ID: 0x%X\n", id);
  uart_send_string(buffer);

  if (id == 0x68) {
    uart_send_string("MPU6050 OK\n");

    // Wake it up from sleep
    i2c_write_byte(MPU6050_ADDR, PWR_MGMT_1, 0x00); // Clear sleep bit
    __delay_cycles(50000);                          // delay to stabilize
  } else {
    uart_send_string("MPU6050 NOT FOUND\n");
  }
}

int mpu6050_read_word(unsigned char reg_high) {
  unsigned char high = i2c_read_byte(MPU6050_ADDR, reg_high);
  unsigned char low = i2c_read_byte(MPU6050_ADDR, reg_high + 1);
  return (int)((high << 8) | low);
}

void mpu6050_read_accel(float *ax, float *ay, float *az) {
  int raw_ax = mpu6050_read_word(0x3B);
  int raw_ay = mpu6050_read_word(0x3D);
  int raw_az = mpu6050_read_word(0x3F);

  *ax = raw_ax / 16384.0;
  *ay = raw_ay / 16384.0;
  *az = raw_az / 16384.0;
}

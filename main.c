#include "i2c.h"
#include "lcd.h"
#include "mpu6050.h"
#include "uart.h"
#include <math.h>
#include <msp430.h>
#include <stdio.h>

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;

  lcd_init();
  lcd_clear();
  // lcd_print("LCD\n");

  uart_init();
  // uart_send_string("UART\n");

  i2c_init();
  // lcd_print("I2C");
  // uart_send_string("I2C\n");

  i2c_write_byte(0x68, 0x6B, 0x00); // Wake up MPU
  uart_send_string("MPU Wake Sent\n");

  mpu6050_init();
  //    lcd_print("MPU");
  //    uart_send_string("MPU\n");

  float ax, ay, az;
  float vz = 0, max_vz = 0;
  float dt = 0.1; // seconds between reads
  int in_rep = 0;
  float last_vz = 0;
  unsigned int reps = 0;
  char buffer[16];

  while (1) {
    // Test for main loop
    // uart_send_string("Loop");

    // Get accelerometer data
    mpu6050_read_accel(&ax, &ay, &az);
    float net_az = az - 1.0;
    if (fabs(net_az) < 0.05)
      net_az = 0;
    vz += net_az * 9.81 * dt;

    // Rep detection
    if (last_vz < 0 && vz >= 0 && fabs(vz) > 0.1) {
      reps++;
    }
    last_vz = vz;

    // Track max velocity
    if (vz > max_vz) {
      max_vz = vz;
    }

    // Display metrics
    lcd_set_cursor(0, 0);
    snprintf(buffer, 16, "Reps:%2d  V:%.1f", reps, vz);
    lcd_print(buffer);

    lcd_set_cursor(1, 0);
    snprintf(buffer, 16, "Max:%.1f", max_vz);
    lcd_print(buffer);

    __delay_cycles(100000);
  }
}

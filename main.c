#include "lcd.h"
#include "mpu6050.h"
#include "uart.h" // import the UART library
#include <msp430.h>
#include <stdio.h>

unsigned int ax, ay, az;
char buffer[17];

int main(void) {

  WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

  uart_init();
  uart_send_string("uart works\n");

  lcd_init();
  mpu6050_init();

  while (1) {
    mpu6050_read_accel(&ax, &ay, &az);



    //lcd_clear();
    lcd_goto_xy(0, 0);
    snprintf(buffer, 16, "X:%d", ax);
    lcd_print(buffer);

    lcd_goto_xy(1, 0);
    snprintf(buffer, 16, "Y:%d Z:%d", ay, az);
    lcd_print(buffer);

    // Debugging
    snprintf(buffer, 64, "AX:%d AY:%d AZ:%d\n", ax, ay, az);
    uart_send_string(buffer);

    __delay_cycles(500000); // Delay ~0.5 sec
  }

  return 0;
}

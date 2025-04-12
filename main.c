#include "lcd.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"
#include <msp430.h>
#include <stdio.h>


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    lcd_init();
    lcd_clear();
    //lcd_print("Init LCD");

    uart_init();
    uart_send_string("UART ready\n");

    i2c_init(); // This should NOT break LCD
    //lcd_print("I2C ready");
    uart_send_string("I2C ready\n");

    mpu6050_init();
    //lcd_print("init mpu");

    lcd_set_cursor(1, 0);
    lcd_print("Done!");
    while (1);
}


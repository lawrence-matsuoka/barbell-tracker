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
    //lcd_print("LCD\n");

    uart_init();
    //uart_send_string("UART\n");

    i2c_init();
    //lcd_print("I2C");
    //uart_send_string("I2C\n");

    i2c_write_byte(0x68, 0x6B, 0x00);  // Wake up MPU
    uart_send_string("MPU Wake Sent\n");

/*    unsigned char id = i2c_read_byte(0x68, 0x75); // WHO_AM_I
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "MPU ID = 0x%X\n", id);
    uart_send_string(buffer);
*/
    mpu6050_init();
//    lcd_print("MPU");
//    uart_send_string("MPU\n");

    //lcd_set_cursor(1, 0);
    //lcd_print("Ready!");

    float ax, ay, az;
    char buffer[16];

    while (1) {
        // test
        uart_send_string("Loop");

        mpu6050_read_accel(&ax, &ay, &az);

        lcd_set_cursor(0, 0);
        snprintf(buffer, 16, "Ax:%.2f", ax);
        lcd_print(buffer);

        lcd_set_cursor(1, 0);
        snprintf(buffer, 16, "Az:%.2f", az);
        lcd_print(buffer);

        uart_send_string("Accel read\n");

        __delay_cycles(1000000); // 1 second
    }
}


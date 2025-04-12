#include "lcd.h"
#include "mpu6050.h"
#include "uart.h" // import the UART library
#include <msp430.h>
#include <stdio.h>


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    uart_init();
    uart_send_string("uart works\n");

    // Initialize modules
    lcd_init();
//    mpu6050_init();

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Hello World");

    while (1);
//    __delay_cycles(1000000);

/*    while (1) {
        float ax, ay, az;
        char buffer[16];

        mpu6050_read_accel(&ax, &ay, &az);

        // Calculate velocity and displacement (basic)
        // v = a * dt; s += v * dt (not shown here, for simplicity)
        // Just show acceleration for now

        lcd_set_cursor(0, 0);
        snprintf(buffer, 16, "Ax:%.2f", ax);
        lcd_print(buffer);

        lcd_set_cursor(1, 0);
        snprintf(buffer, 16, "Az:%.2f", az);
        lcd_print(buffer);

        __delay_cycles(1000000); // 1 sec delay (adjust as needed)
    }*/
}


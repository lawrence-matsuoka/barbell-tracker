#include <msp430.h>
#include "lcd.h"
#include <string.h>

#define RS BIT3  // P2.3
#define EN BIT1  // P2.1

void delay_ms(unsigned int ms) {
    __delay_cycles(500000); // Assuming 1 MHz clock
}

void lcd_pulse_enable() {
    P2OUT |= EN;
    delay_ms(1);
    P2OUT &= ~EN;
    delay_ms(1);
}

void lcd_cmd(unsigned char cmd) {
    P2OUT &= ~RS;
    P3OUT = cmd;              // Send lower 7 bits to P3
    P4OUT = (cmd & 0x80) ? BIT0 : 0; // Bit 7 to P4.0
    lcd_pulse_enable();
}

void lcd_data(unsigned char data) {
    P2OUT |= RS;
    P3OUT = data;
    P4OUT = (data & 0x80) ? BIT0 : 0;
    lcd_pulse_enable();
}

void lcd_init() {
    P2DIR |= RS | EN;
    P3DIR = 0xFF;
    P4DIR |= BIT0;

    delay_ms(20);

    lcd_cmd(0x38); // 8-bit, 2 line
    lcd_cmd(0x0C); // Display on, cursor off
    lcd_cmd(0x06); // Entry mode
    lcd_cmd(0x01); // Clear
    delay_ms(2);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_clear() {
    lcd_cmd(0x01);
    delay_ms(2);
}

void lcd_goto_xy(unsigned char row, unsigned char col) {
    unsigned char addr = (row == 0) ? 0x00 + col : 0x40 + col;
    lcd_cmd(0x80 | addr);
}


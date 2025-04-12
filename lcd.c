#include "lcd.h"
#include <msp430.h>
#include <string.h>

#define LCD_RS BIT0 // P2.0
#define LCD_EN BIT6 // P1.6
#define LCD_D4 BIT2 // P2.2
#define LCD_D5 BIT3 // P2.3
#define LCD_D6 BIT4 // P2.4
#define LCD_D7 BIT5 // P2.5

void pulse_enable() {
  P1OUT |= LCD_EN;
  __delay_cycles(200);
  P1OUT &= ~LCD_EN;
  __delay_cycles(200);
}

void lcd_send_nibble(unsigned char nibble) {
  P2OUT &= ~(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

  if (nibble & BIT4)
    P2OUT |= LCD_D4;
  if (nibble & BIT5)
    P2OUT |= LCD_D5;
  if (nibble & BIT6)
    P2OUT |= LCD_D6;
  if (nibble & BIT7)
    P2OUT |= LCD_D7;

  pulse_enable();
}

void lcd_command(unsigned char cmd) {
  P2OUT &= ~LCD_RS; // RS = 0 for command

  lcd_send_nibble(cmd & 0xF0);
  lcd_send_nibble((cmd << 4) & 0xF0);

  __delay_cycles(2000);
}

void lcd_data(unsigned char data) {
  P2OUT |= LCD_RS; // RS = 1 for data

  lcd_send_nibble(data & 0xF0);
  lcd_send_nibble((data << 4) & 0xF0);

  __delay_cycles(2000);
}

void lcd_print(char *str) {
  while (*str) {
    lcd_data(*str++);
  }
}

void lcd_set_cursor(unsigned char row, unsigned char col) {
  unsigned char address = (row == 0) ? 0x80 + col : 0xC0 + col;
  lcd_command(address);
}

void lcd_clear() {
  lcd_command(0x01); // Clear display
  __delay_cycles(50000);
}

void lcd_init() {
  P1SEL &= ~BIT6;
  P2SEL &= ~(BIT0 | BIT2 | BIT3 | BIT4 | BIT5);

  P2DIR |= LCD_RS | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  P2OUT &= ~(LCD_RS | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

  P1DIR |= LCD_EN;
  P1OUT &= ~LCD_EN;

  __delay_cycles(100000);

  lcd_send_nibble(0x03); // Initialize
  __delay_cycles(5000);
  lcd_send_nibble(0x03);
  __delay_cycles(5000);
  lcd_send_nibble(0x03);
  __delay_cycles(5000);
  lcd_send_nibble(0x02); // 4-bit mode

  lcd_command(0x28); // 4-bit, 2 line, 5x8 font
  lcd_command(0x0C); // Display on
  lcd_command(0x06); // Entry mode
  lcd_command(0x01); // Clear display
  __delay_cycles(2000);
}

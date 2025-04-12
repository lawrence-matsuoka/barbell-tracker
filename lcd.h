#ifndef LCD_H_
#define LCD_H_

void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_print(char *str);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_clear(void);

#endif

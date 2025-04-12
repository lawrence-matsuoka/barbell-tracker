#ifndef LCD_H_
#define LCD_H_

void lcd_init();
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_print(char *str);
void lcd_clear();
void lcd_goto_xy(unsigned char row, unsigned char col);

#endif


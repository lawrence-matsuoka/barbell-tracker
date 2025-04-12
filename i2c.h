#ifndef I2C_H_
#define I2C_H_

void i2c_init();
void i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char data);
unsigned char i2c_read_byte(unsigned char addr, unsigned char reg);

#endif

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void i2c_init(void);
void i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
void i2c_read_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);

#endif

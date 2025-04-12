#ifndef MPU6050_H_
#define MPU6050_H_

void mpu6050_init();
unsigned int mpu6050_read_word(unsigned int reg);
void mpu6050_read_accel(unsigned int *ax, unsigned int *ay, unsigned int *az);

#endif


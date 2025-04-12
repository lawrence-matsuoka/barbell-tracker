#ifndef MPU6050_H_
#define MPU6050_H_

void mpu6050_init();
void mpu6050_read_accel(float *ax, float *ay, float *az);

#endif

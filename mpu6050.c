#include <msp430.h>
#include <stdint.h>
#include "mpu6050.h"

#define MPU_ADDR 0x68

void i2c_init_B1() {
    UCB1CTL1 |= UCSWRST;
    UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB1CTL1 = UCSSEL_2 | UCSWRST;
    UCB1BR0 = 10;
    UCB1BR1 = 0;
    P4SEL |= BIT1 | BIT2;
    UCB1CTL1 &= ~UCSWRST;
}

void i2c_write_byte(unsigned int addr, unsigned int reg, unsigned int data) {
    while (UCB1CTL1 & UCTXSTP); // Wait if STOP is sent
    UCB1I2CSA = addr;
    UCB1CTL1 |= UCTR | UCTXSTT;

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg;

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = data;

    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 |= UCTXSTP;
}

unsigned int i2c_read_byte(unsigned int addr, unsigned int reg) {
    while (UCB1CTL1 & UCTXSTP);
    UCB1I2CSA = addr;
    UCB1CTL1 |= UCTR | UCTXSTT;

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg;

    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 &= ~UCTR;
    UCB1CTL1 |= UCTXSTT;

    while (UCB1CTL1 & UCTXSTT);
    unsigned int data = UCB1RXBUF;
    UCB1CTL1 |= UCTXSTP;

    return data;
}

unsigned int mpu6050_read_word(unsigned int reg) {
    unsigned int high = i2c_read_byte(MPU_ADDR, reg);
    unsigned int low  = i2c_read_byte(MPU_ADDR, reg + 1);
    return (unsigned int)((high << 8) | low);
}

void mpu6050_init() {
    i2c_init_B1();
    i2c_write_byte(MPU_ADDR, 0x6B, 0x00); // Wake up from sleep
}

void mpu6050_read_accel(unsigned int *ax, unsigned int *ay, unsigned int *az) {
    *ax = mpu6050_read_word(0x3B);
    *ay = mpu6050_read_word(0x3D);
    *az = mpu6050_read_word(0x3F);
}


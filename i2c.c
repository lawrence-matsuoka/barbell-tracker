#include <msp430.h>
#include "i2c.h"

#include <msp430.h>

void i2c_init(void) {
    // Set P4.1 (SDA), P4.2 (SCL) to I2C mode
    P4SEL |= BIT1 | BIT2;

    UCB1CTL1 |= UCSWRST;                     // Put eUSCI_B in reset
    UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC;    // I2C Master, synchronous
    UCB1CTL1 = UCSSEL_2 | UCSWRST;           // Use SMCLK, still in reset
    UCB1BR0 = 10;                            // 100kHz @ 1MHz SMCLK
    UCB1BR1 = 0;
    UCB1I2CSA = 0x68;                        // MPU6050 default address

    UCB1CTL1 &= ~UCSWRST;                    // Release from reset
    UCB1IE = 0;                              // Disable interrupts (for now)
}


void i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char data) {
    while (UCB1CTL1 & UCTXSTP); // Wait for stop

    UCB1I2CSA = addr;
    UCB1CTL1 |= UCTR + UCTXSTT; // Transmitter + Start
    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg;
    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = data;
    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 |= UCTXSTP; // Stop
    while (UCB1CTL1 & UCTXSTP);
}

unsigned char i2c_read_byte(unsigned char addr, unsigned char reg) {
    unsigned char value;

    while (UCB1CTL1 & UCTXSTP);
    UCB1I2CSA = addr;
    UCB1CTL1 |= UCTR + UCTXSTT;
    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg;
    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 &= ~UCTR; // Receiver mode
    UCB1CTL1 |= UCTXSTT;
    while (UCB1CTL1 & UCTXSTT);
    while (!(UCB1IFG & UCRXIFG));
    value = UCB1RXBUF;
    UCB1CTL1 |= UCTXSTP;

    return value;
}


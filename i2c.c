#include <msp430.h>
#include "i2c.h"

void i2c_init(void) {
    P4SEL |= BIT1 | BIT2; // P4.1 = SDA, P4.2 = SCL
    UCB1CTL1 |= UCSWRST; // Put USCI in reset
    UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC; // Master, I2C mode, synchronous
    UCB1CTL1 = UCSSEL_2 | UCSWRST; // SMCLK
    UCB1BR0 = 10; // fSCL = SMCLK / 10
    UCB1BR1 = 0;
    UCB1CTL1 &= ~UCSWRST; // Clear reset
}

void i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    while (UCB1CTL1 & UCTXSTP); // Wait if stop is sent

    UCB1I2CSA = dev_addr;
    UCB1CTL1 |= UCTR + UCTXSTT; // Transmit mode and start

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg_addr; // Register address

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = data; // Data

    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 |= UCTXSTP; // Send stop
    while (UCB1CTL1 & UCTXSTP);
}

void i2c_read_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length) {
    while (UCB1CTL1 & UCTXSTP); // Wait if stop is sent

    UCB1I2CSA = dev_addr;
    UCB1CTL1 |= UCTR + UCTXSTT; // Write mode to send register

    while (!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = reg_addr;

    while (!(UCB1IFG & UCTXIFG));
    UCB1CTL1 &= ~UCTR;           // Read mode
    UCB1CTL1 |= UCTXSTT;         // Restart

    while (UCB1CTL1 & UCTXSTT);  // Wait for start to be sent

    uint8_t i;
    for (i = 0; i < length; i++) {
        if (i == length - 1) {
            UCB1CTL1 |= UCTXSTP; // Send stop after last byte
        }
        while (!(UCB1IFG & UCRXIFG));
        data[i] = UCB1RXBUF;
    }

    while (UCB1CTL1 & UCTXSTP);
}

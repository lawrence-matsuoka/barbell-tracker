#include "i2c.h"
#include <msp430.h>

#define MPU6050_ADDR 0x68

void i2c_init(void) {
  // Set P4.1, P4.2 to I2C mode
  P4SEL |= BIT1 | BIT2;

  UCB1CTL1 |= UCSWRST;                  // Put eUSCI_B in reset
  UCB1CTL0 = UCMST | UCMODE_3 | UCSYNC; // I2C Master, synchronous
  UCB1CTL1 = UCSSEL_2 | UCSWRST;        // Use SMCLK
  UCB1BR0 = 20;
  UCB1BR1 = 0;
  UCB1I2CSA = MPU6050_ADDR; // default addr of MPU6050

  UCB1CTL1 &= ~UCSWRST; // Release from reset
  UCB1IE = 0;           // Disable interrupts
}

void i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char data) {

  while (UCB1CTL1 & UCTXSTP)
    ;
  UCB1CTL1 |= UCTR + UCTXSTT; // Transmitter + START
  UCB1I2CSA = addr;

  while (!(UCB1IFG & UCTXIFG))
    ;              // Wait for TX buffer
  UCB1TXBUF = reg; // Send register

  while (!(UCB1IFG & UCTXIFG))
    ;
  UCB1TXBUF = data;

  while (!(UCB1IFG & UCTXIFG))
    ;
  UCB1CTL1 |= UCTXSTP; // Send stop
  while (UCB1CTL1 & UCTXSTP)
    ; // Wait
}

unsigned char i2c_read_byte(unsigned char addr, unsigned char reg) {
  while (UCB1CTL1 & UCTXSTP)
    ;               // Wait for stop condition
  UCB1I2CSA = addr; // Set slave address

  UCB1CTL1 |= UCTR + UCTXSTT; // Transmitter mode
  while (!(UCB1IFG & UCTXIFG))
    ;              // Wait for TX buffer
  UCB1TXBUF = reg; // Send register

  while (!(UCB1IFG & UCTXIFG))
    ;                  // Wait for transmission
  UCB1CTL1 &= ~UCTR;   // Receiver mode
  UCB1CTL1 |= UCTXSTT; // Repeated START

  while (UCB1CTL1 & UCTXSTT)
    ;                  // Wait for START to complete
  UCB1CTL1 |= UCTXSTP; // Send stop

  while (!(UCB1IFG & UCRXIFG))
    ; // Wait for receive complete
  return UCB1RXBUF;
}

void i2c_scan() {
  uart_send_string("Scanning I2C...\n");

  char buffer[32];

  unsigned char addr = 1;
  for (addr; addr < 127; addr++) {
    UCB1I2CSA = addr;
    UCB1CTL1 |= UCTR + UCTXSTT;
    while (UCB1CTL1 & UCTXSTT)
      ; // Wait for START to complete

    if (!(UCB1IFG & UCNACKIFG)) {
      snprintf(buffer, sizeof(buffer), "Found at 0x%02X\n", addr);
      uart_send_string(buffer);
    }

    UCB1CTL1 |= UCTXSTP;
    while (UCB1CTL1 & UCTXSTP)
      ; // Wait for STOP to complete
  }

  uart_send_string("Scan done.\n");
}

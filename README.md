# barbell-tracker

A proof-of-concept barbell device to track velocity, acceleration, and power using an MSP430F5529LP development board and an MPU6050 module.

[Trusted resource](https://www.strongerbyscience.com/complete-guide-to-bar-speed-trackers/)

## Learning Outcomes
- Communication over I2C
- Debugging with UART

## Hardware
- MSP430F5529LP
- MPU6050
- Logic analyzer (if you're me, for debugging)
- Breadboard (for prototype)
- Jumper wire, resistors, LEDs
- AA battery holder

### MSP430F5529 Launchpad
![MSP430F5529LP pin map](docs/MSP430F5529LP-Launchpad-Pin-Map.png)

## Breakdown
- MPU6050 takes in signal over I2C
- Converts to units we care about
- Display the units on the LCD

## Debugging over UART
Getting everything working in one go is impossible for anyone at my level of study. I am debugging with a logic analyzer using PulseView to view the sent characters/strings/ints over UART.

### PulseView settings
![PulseView settings for UART](docs/pulseview-uart-example.png)
- Add protocol decoder (yellow and green waves icon) and select UART
- 8 data bits, no parity, 1 stop bit


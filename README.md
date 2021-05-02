# PSoC Servo Control Library
This repository contains a simple library to be used with PSoC 5LP microcontrollers for the control of servo motors.

## Hardware Requirements
- PSoC 5LP MCU. You can start with a kit, such as the [CY8CKIT-059](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-059-psoc-5lp-prototyping-kit-onboard-programmer-and)
- A servo motor. 
## Included Projects
- **Servo Library**: simple project showing basic functionality of the Servo library. To be used as a starting point for integrating the servo library into your own projects.
- **Servo Library I2C**: this PSoC Creator project allows to control a servo motor directly from a I2C interface, exploiting the EZI2C component from PSoC Creator. Just connect the servo motor to the MCU, connect the USB via a I2C Bridge and send a write request to register `0x00` with the angle to be set. 
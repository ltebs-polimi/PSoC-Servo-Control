# PSoC Servo Control Library
This repository contains a simple library to be used with PSoC 5LP microcontrollers for the control of servo motors. 

## Hardware Requirements
- PSoC 5LP MCU. You can start with a kit, such as the [CY8CKIT-059](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-059-psoc-5lp-prototyping-kit-onboard-programmer-and)
- A servo motor. You can use standard servo motors (MG996R, ...)
## Included Projects
- **Servo Library**: simple project showing basic functionality of the Servo library. To be used as a starting point for integrating the servo library into your own projects.
- **Servo Library Interface**: this PSoC Creator project allows to control a servo motor directly from a I2C or UART interface. Just connect the servo motor to the MCU, connect the USB port of the KitProg section to your laptop and you're ready to go. 
    - *I2C MODE*: connect the KitProg to the Bridge Control Panel (or any I2C Bridge, another MCU would work as well).To set the new servo angle, send a write request to register `0x00` with the angle to be set. 
    - *UART MODE*: connect the KitProg to any serial monitor (e.g., CoolTerm) and send out the command `h` to get the list of available commands.
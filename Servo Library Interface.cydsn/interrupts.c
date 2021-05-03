/**
  *  \file           interrupts.c
  *  \brief          Source file for servo motor interrupts.
  *
  *  \author         Davide Marzorati
  */
 
/*******************************************************************************
 * Copyright (c) 2021 Marzorati Davide
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include "cytypes.h"
#include "servo.h"

/**
 *  \brief          Buffer with data received from I2C.
 */
extern uint8_t slave_buffer[];

/**
 *  \brief          Flag signaling new data to be processed.
 */
extern uint8_t ezi2c_flag;

/**
 *  \brief          Variable holding the angle of the servo motor.
 */
extern uint8_t servo_angle;


/**
 *  \brief          Callback called on EZI2C exit.
 *  
 *  This callback is called when exiting from the EZI2C ISR. It
 *  processes the buffer and checks whether it is required to
 *  update the servo angle.
 */
void EZI2C_ISR_ExitCallback(void)
{
    // Get the new servo angle from the I2C buffer
    uint8_t new_servo_angle = slave_buffer[0];
    
    // Check if the value changed and if we have a valid angle
    if ( (new_servo_angle >= 0) && (new_servo_angle <= 180) && (new_servo_angle != servo_angle) )
    {
        // Update angle
        servo_angle = new_servo_angle;
        Servo_SetPosition(servo_angle);
        // Get position and update buffer
        uint16_t pwm_compare = Servo_GetPosition();
        slave_buffer[1] = pwm_compare >> 8;
        slave_buffer[2] = pwm_compare & 0xFF;
        // Set flag
        ezi2c_flag = 1;
    }
    else
    {
        // We either had a read transaction or an invalid angle
        slave_buffer[0] = servo_angle; // Make sure we set the angle in the buffer
        if ( new_servo_angle > 180)
        {
            // Invalid angle --> set flag
            ezi2c_flag = 2;
        }
    }
}

/* [] END OF FILE */

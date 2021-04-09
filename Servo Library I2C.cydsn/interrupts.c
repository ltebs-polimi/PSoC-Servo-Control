/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "cytypes.h"
#include "servo.h"

extern uint8_t slave_buffer[];
extern uint8_t ezi2c_flag;
extern uint8_t servo_angle;

void EZI2C_ISR_ExitCallback(void)
{
    // Get the new servo angle from the I2C buffer
    uint8_t new_servo_angle = slave_buffer[0];
    
    // Check if the value changed and if we have a valid angle
    if ( (new_servo_angle >= 0) && (new_servo_angle <= 180) && (new_servo_angle != servo_angle) )
    {
        // Update angle
        servo_angle = new_servo_angle;
        servo_setPosition(servo_angle);
        // Get position and update buffer
        uint16_t pwm_compare = servo_getPosition();
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

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

#ifndef __SERVO_H
    #define __SERVO_H
    
    #include "PWM_1.h"
    #include <cytypes.h>
    #include <math.h>

    /// Physical limits [degrees] of the servo
    #define SERVO_LIMIT_L   0
    #define SERVO_LIMIT_H   180
    
    /// PWM on-time limits [digit] of the servo; compare values for PWM_1
    #define SERVO_PWM_LIMIT_L   1547
    #define SERVO_PWM_LIMIT_H   1399
    
    /// Map function implementation: used in servo_setPosition(deg)
    uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
    
    /// Set the servo to the desired motion
    uint16_t servo_setPosition(uint16_t deg);
    
    /// Get the current position of the servo
    uint16_t servo_getPosition(void);
    
    /// Define motion profile from xi to xf with duration t
    void servo_motionProfile(uint16_t xi, uint16_t xf, uint8_t t);
    
    /// Define motion profile from xi to xf with duration t : BLOCKING FUNCTION
    void servo_motionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t);
    
    
#endif

/* [] END OF FILE */

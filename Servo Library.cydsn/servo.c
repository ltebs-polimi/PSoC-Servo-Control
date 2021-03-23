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

#include "servo.h"
#include "UART_1.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    
}

uint16_t servo_setPosition(uint16_t deg) {
 
    // Get the compare value for the desired position
    float tmp = map(deg, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    
    // Update the PWM compare value
    uint16_t compareValue = round(tmp);
    PWM_1_WriteCompare(compareValue);
    
    return compareValue;
    
}

uint16_t servo_getPosition(void) {
 
    // Get the actual compare value
    uint16_t comparePWM = PWM_1_ReadCompare();
    
    // Map the value to degrees
    return map(comparePWM, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H, SERVO_LIMIT_L, SERVO_LIMIT_H);
    
}

void servo_motionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t) {
    
    // Check : current position = xi ?
    if ( servo_getPosition() != xi ) {
        servo_setPosition(xi);
    }
    
    char b[50];
 
    // Sleep time [ms]
    uint16_t dt = abs(xf-xi)/t;
    
    // Direction
    int8_t dx = (xf > xi) ? 1 : -1;
    
    sprintf(b, "dt = %d\r\n", dt);
    UART_1_PutString(b);
    
    while( xi != xf ) {
    
        xi += dx;
        servo_setPosition(xi);
        CyDelay(dt);
        
    }
    
    // Force final position --> ?
    servo_setPosition(xf);
    
}

/* [] END OF FILE */

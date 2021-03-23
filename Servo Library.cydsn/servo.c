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
 
    // Split the duration in 100 ms windows (10 = 1000ms/s / 100ms)
    uint16_t nw = 40 * t;
    
    // Split the xi-xf range in segments of width dx
    int16_t dx = (xf-xi)/nw;
    // Minimum resolution: 1 degree (test with float value for setPosition())
    dx = (dx == 0) ? 1 : dx;
    
    // Direction
    int8_t dir = (xf > xi) ? 1 : -1;
    //dx *= dir;
    
    sprintf(b, "nw = %d | dir = %d | dx = %d\r\n", nw, dir, dx);
    UART_1_PutString(b);
    
    for( uint16_t i = 0; i < nw; i++ ) {
    
        xi += dx;
        servo_setPosition(xi);
        CyDelay(25);
        
    }
    
    // Force final position --> ?
    servo_setPosition(xf);
    
}

/* [] END OF FILE */

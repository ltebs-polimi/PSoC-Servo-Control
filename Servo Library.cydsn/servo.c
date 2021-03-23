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

float map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    
    float tmp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    char b[30];
    sprintf(b, "map(x) = %f\r\n", tmp);
    UART_1_PutString(b);
    return tmp;
    
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

/* [] END OF FILE */

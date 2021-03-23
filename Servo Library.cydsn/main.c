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
#include "project.h"
#include <stdio.h>
#include "servo.h"

char buf[100];
#define UART_1_PutBuf UART_1_PutString(buf);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_1_Start();
    UART_1_Start();
    
    CyDelay(2000);
    
    /* Set position 1: 45° */
    servo_setPosition(45);
    /* Check */
    uint16_t pwmc = map(45, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    uint16_t check = servo_getPosition();
    sprintf(buf, "Position 1: 45 [deg]\t PWM compare: %d servo_getPosition(): %d\r\n", pwmc, check);
    UART_1_PutBuf;
    
    CyDelay(2000);
    
    /* Set position 2: 135° */
    servo_setPosition(180);
    /* Check */
    pwmc = map(180, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    check = servo_getPosition();
    sprintf(buf, "Position 1: 135 [deg]\t PWM compare: %d servo_getPosition(): %d\r\n", pwmc, check);
    UART_1_PutBuf;
    pwmc = map(752, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H, SERVO_LIMIT_L, SERVO_LIMIT_H);
    sprintf(buf, "Inverse map: %d\r\n", pwmc);
    UART_1_PutBuf;
    
    CyDelay(2000);
    
    // Test the motion profile
    servo_motionProfileBlocking(180, 0, 5);
    
    return 0;
    
}

/* [] END OF FILE */

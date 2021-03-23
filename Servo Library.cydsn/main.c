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

#define POS_0_ms    650
#define POS_90_ms   1550
#define POS_180_ms  2500
// PWM compare values
#define POS_0       769
#define POS_90      734
#define POS_180     699

char buf[100];
#define UART_1_PutBuf UART_1_PutString(buf);

// long map(long x, long in_min, long in_max, long out_min, long out_max);

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
    servo_setPosition(135);
    /* Check */
    pwmc = map(135, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    check = servo_getPosition();
    sprintf(buf, "Position 1: 135 [deg]\t PWM compare: %d servo_getPosition(): %d\r\n", pwmc, check);
    UART_1_PutBuf;
    pwmc = map(752, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H, SERVO_LIMIT_L, SERVO_LIMIT_H);
    sprintf(buf, "Inverse map: %d\r\n", pwmc);
    UART_1_PutBuf;
    
    return 0;
    
}

/* [] END OF FILE */

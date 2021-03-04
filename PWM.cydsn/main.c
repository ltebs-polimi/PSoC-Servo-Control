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

void servoStop(void);
void servoPos(void);
void servoNeg(void);
void wait(uint8_t sec);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_1_Start();
    UART_1_Start();
    
    /*UART_1_PutString("Starting");
    wait(3);
    
    UART_1_PutString("servoStop(): 1.5 ms PWM");
    servoStop();
    wait(5);
    
    UART_1_PutString("servoPos(): 2 ms PWM");
    servoPos();
    wait(5);
    
    UART_1_PutString("servoStop(): 1.5 ms PWM");
    servoStop();
    wait(5);
    
    
    UART_1_PutString("servoNeg(): 1 ms PWM");
    servoNeg();
    wait(5);
    
    UART_1_PutString("Finished.\r\n");
    servoStop();*/
    
}

void servoStop(void) {
 
    // 1.5 ms ON
    PWM_1_WriteCompare(36);
    
}

void servoPos(void) {
    
    // 2.0 ms ON
    PWM_1_WriteCompare(35);
    
}

void servoNeg(void) {
    
    // 1.0 ms ON
    PWM_1_WriteCompare(37);
    
}

void wait(uint8_t sec) {
    
    for ( uint8_t i = 0; i < sec; i++ ) {
        CyDelay(1000);
        UART_1_PutChar('.');
    }
    UART_1_PutString("\r\n");
    
}

/* [] END OF FILE */

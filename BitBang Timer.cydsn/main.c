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

CY_ISR_PROTO(Set_1_ISR);
CY_ISR_PROTO(Set_0_ISR);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    Timer_20ms_Start();
    Timer_Var_Start();
    
    isr_20ms_StartEx(Set_1_ISR);
    isr_control_StartEx(Set_0_ISR);
    
    UART_1_PutString("Servo STOP.\r\n");  
    
}

CY_ISR(Set_1_ISR) {
 
    Servo_Write(1);
    
}

CY_ISR(Set_0_ISR) {
    
    Servo_Write(0);
    
}

/* [] END OF FILE */

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

#define POS_0   1500
#define POS_90  2000
#define POS_180 1000

void servo_setPosition(uint16_t pos);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    
    while(1) {
    
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_0);
        
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_90);
            
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_180);
      
    }
    
}

void servo_setPosition(uint16_t pos) {
 
    Servo_Write(1);
    CyDelayUs(pos);
    Servo_Write(0);
    CyDelayUs(20000-pos);
    
}


/* [] END OF FILE */

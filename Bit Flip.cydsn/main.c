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

#define POS_0   1000
#define POS_45  1500
#define POS_90  2000
#define POS_135 2500
#define POS_180 1500

char buffer[50];

void servo_setPosition(uint16_t pos);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    
    uint8_t count = 0;
    
    while(count<2) {
        
        sprintf(buffer, "Position 0:   %d us\r\n", POS_0);
        UART_1_PutString(buffer);
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_0);
        
        sprintf(buffer, "Position 45:  %d us\r\n", POS_45);
        UART_1_PutString(buffer);
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_45);
        
        sprintf(buffer, "Position 90:  %d us\r\n", POS_90);
        UART_1_PutString(buffer);
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_90);
            
        sprintf(buffer, "Position 135: %d us\r\n", POS_135);
        UART_1_PutString(buffer);
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_135);
            
        sprintf(buffer, "Position 180: %d us\r\n", POS_180);
        UART_1_PutString(buffer);
        for ( uint8_t i = 0; i < 200; i++ ) 
            servo_setPosition(POS_180);
      
        count++;
    
    }
    
//    for(uint8_t i = 0; i < sizeof(POSITIONS); i++) {
//        sprintf(buffer, "Position %d : %d us\r\n", i, POSITIONS[i]);
//        UART_1_PutString(buffer);
//        for ( uint8_t i = 0; i < 200; i++ ) {
//            servo_setPosition(POSITIONS[i]);
//        }
//    }
    
}

void servo_setPosition(uint16_t pos) {
 
    Servo_Write(1);
    CyDelayUs(pos);
    Servo_Write(0);
    CyDelayUs(20000-pos);
    
}


/* [] END OF FILE */

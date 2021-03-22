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

#define POS_0_ms    650
#define POS_90_ms   1550
#define POS_180_ms  2500
// PWM compare values
#define POS_0       769
#define POS_90      734
#define POS_180     699

char buf[50];

long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_1_Start();
    UART_1_Start();
    
    // Test the map() function
    int pos90 = map(0, -90, +90, POS_0_ms, POS_180_ms); // SHOULD RETURN 1550
    sprintf(buf, "pos90 = %d (expected: %d)\r\n", pos90, POS_0_ms);
    UART_1_PutString(buf);
    
    uint8_t count = 0;
    
    while(count < 3) {
     
        // Go to position 0 (left)
        PWM_1_WriteCompare(769);
        // Hold for 2 seconds
        CyDelay(2000);
        
        // Go to position 90 (middle)
        PWM_1_WriteCompare(734);
        // Hold for 2 seconds
        CyDelay(2000);
        
        // Go to position 180 (right)
        PWM_1_WriteCompare(699);
        // Hold for 2 seconds
        CyDelay(2000);
        
        count++;
        
        // 45°
        uint16_t x = map(45, 0, 180, 769, 699);
        PWM_1_WriteCompare(x);
        CyDelay(2000);
        
    }
    
    return 0;
    
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    
}

/* [] END OF FILE */

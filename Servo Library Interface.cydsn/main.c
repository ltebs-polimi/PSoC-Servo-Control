/**
*   \file main.c
*
*   \brief Main source file for the PSoC Servo control project with I2C interface.
*
*   This is the main source file that sets up the main program functionality
*   for PSoC Servo Control with EZI2C interface for angle selection via I2C.
*
*   \author Davide Marzorati, Mattia Pesenti
*   
*/

#include "project.h"
#include <stdio.h>
#include "servo.h"

#define SLAVE_BUFFER_SIZE 3
#define UART_Debug_PutBuf UART_Debug_PutString(buf);

char buf[100];

uint8_t servo_angle;
uint8_t slave_buffer[SLAVE_BUFFER_SIZE];
uint8_t ezi2c_flag;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_Servo_Start();
    UART_Debug_Start();
     
    EZI2C_Start();
    
    slave_buffer[0] = 0;    // Servo angle
    slave_buffer[1] = PWM_Servo_ReadCompare() >> 8;
    slave_buffer[2] = PWM_Servo_ReadCompare() & 0xFF;
    
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 1, slave_buffer);
    uint16_t pwmc, check;
    for (;;)
    {
        if (ezi2c_flag > 0)
        {
            switch(ezi2c_flag)
            {
                case 1:
                    pwmc = map(servo_angle, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
                    check = servo_getPosition();
                    sprintf(buf, "Position: %d [deg]\t PWM compare: %d servo_getPosition(): %d\r\n", servo_angle, pwmc, check);
                    UART_Debug_PutBuf;
                    break;
                case 2:
                    // Invalid angle
                    sprintf(buf, "Invalid angle\r\n");
                    UART_Debug_PutBuf;
                    break;
            }
            ezi2c_flag = 0;
        }
    }
    
    return 0;
    
}

/* [] END OF FILE */

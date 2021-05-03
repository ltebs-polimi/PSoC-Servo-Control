/**
 *  \file           main.c
 *  \brief          Main source file for Servo Library Interface project.
 *
 *  Connect the signal pin of the servo motor to pin 2.6. Select either
 *  communication protocol:
 *      - I2C: connect the PSoC to a I2C bridge, and send the servo angle
 *             to register 0x00.
 *      - UART: send command 'h' to see all available commands on the COM port.
 *
 *  \author         Davide Marzorati
 */
 
/*******************************************************************************
 * Copyright (c) 2021 Marzorati Davide
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include "project.h"
#include <stdio.h>
#include "servo.h"
#include "uart_interface.h"

/************************************
 *          Constants               *
 ************************************/

/**
 *  \brief Size of the slave I2C buffer.
 */
#define SLAVE_BUFFER_SIZE 3

/************************************
 *            Globals               *
 ************************************/
/**
 *  \brief Size of the slave I2C buffer.
 */
char buf[100];

/**
 *  \brief Current servo angle.
 */
uint8_t servo_angle;

/**
 *  \brief Slave I2C buffer.
 */
uint8_t slave_buffer[SLAVE_BUFFER_SIZE];

/**
 *  \brief Flag set in the EZI2C ISR Callback.
 */
uint8_t ezi2c_flag;

/************************************
 *     Function declarations        *
 ************************************/


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Initialize components */
    PWM_Servo_Start();
    UART_Start();
    EZI2C_Start();
    
    // Initialize EZI2C buffer
    slave_buffer[0] = 0;    // Servo angle
    slave_buffer[1] = PWM_Servo_ReadCompare() >> 8;
    slave_buffer[2] = PWM_Servo_ReadCompare() & 0xFF;
    
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 1, slave_buffer);
    
    /* Clear screen on CoolTerm */
    UART_PutChar('\f');
   
    for (;;)
    {
        if (ezi2c_flag > 0)
        {
            /* Flag set in the EZI2C ISR callback */
            switch(ezi2c_flag)
            {
                case 1:
                    /* We have a valid angle, print out info on UART */
                    UART_PrintServoInfo();
                    break;
                case 2:
                    // Invalid angle
                    sprintf(buf, "!! Invalid angle\r\n");
                    UART_PutString(buf);
                    break;
            }
            ezi2c_flag = 0;
        }
        
        if (UART_GetRxBufferSize() > 0)
        {
            UART_ProcessIncomingChar();
        }
        
    }
    
    return 0;
    
}


/* [] END OF FILE */

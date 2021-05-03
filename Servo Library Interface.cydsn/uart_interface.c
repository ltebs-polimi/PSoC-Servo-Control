/**
 *  \file           uart_interface.c
 *  \brief          Source file for servo motor UART interface.
 *
 *  \author         Davide Marzorati
 */

/*******************************************************************************
 * Copyright (c) 2021 Marzorati Davide, Pesenti Mattia
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

#include "uart_interface.h"

#include "servo.h"
#include "stdio.h"
#include "UART.h"

/************************************************
*                 UART Commands                 *
************************************************/

/*
 *  \brief Help command to print out menu guide.
 */
#define HELP_COMMAND            'h'

/*
 *  \brief Command to get current servo information.
 */
#define SERVO_COMMAND           'p'

/*
 *  \brief Multi char command to set servo angle.
 */
#define ANGLE_COMMAND_SET       'd'

/*
 *  \brief Multi char command latch servo angle.
 */
#define ANGLE_COMMAND_LATCH     'D'

/************************************************
*                  Constants                    *
************************************************/

/*
 *  \brief Number of bytes for angle commands.
 */
#define ANGLE_COMMANDS_N_BYTES   5

/*
 *  \brief Help message for UART interface.
 */
#define HELP_MESSAGE "PSoC Servo Motor Control - UART Interface\r\n\
                       - h: print help menu\r\n\
                       - p: print current servo information\r\n\
                       - dXXXD: set angle to XXX degress\r\n"


/************************************************
*                    Globals                    *
************************************************/

/*
 *  \brief Current servo angle (set by I2C of UART).
 */
extern uint8_t servo_angle;

/************************************************
*            Static Variables                   *
************************************************/

/*
 *  \brief Buffer for UART messages.
 */
static char buf[100];

/*
 *  \brief Boolean flag for multi char command.
 */
static uint8_t is_multi_char_cmd = 0;

/*
 *  \brief Current multi char command.
 */
static char multi_char_cmd;

/*
 *  \brief Number of processed bytes for angle multi char command.
 */
static uint8_t number_of_processed_bytes_angle = 0;

/*
 *  \brief Buffer holding current angle settings.
 */
static uint8_t optional_args_buffer_angle[3];

/************************************************
*          Function Declarations                *
************************************************/
static void UART_StartMultiCharCmd(char received);
static void UART_StopMultiCharCmd(void);
static void UART_ProcessAngleCommand(char received);

/*
 *  \brief Print current servo settings.
 */
void UART_PrintServoInfo(void)
{
    uint16_t pwmc = Servo_Map(servo_angle, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    uint16_t check = Servo_GetPosition();
    sprintf(buf, "Position: %d [deg]\t PWM compare: %d servo_getPosition(): %d\r\n", servo_angle, pwmc, check);
    UART_PutString(buf);
}

/*
 *  \brief Process incoming char commands.
 */
void UART_ProcessIncomingChar(void)
{
    /* Get char */
    char received = UART_GetChar();
    
    /* Check if we're in the middle of a multi char command */
    if (is_multi_char_cmd)
    {
        switch (multi_char_cmd)
        {
            case ANGLE_COMMAND_SET:
                UART_ProcessAngleCommand(received);
                break;
        }
    }
    else
    {
        switch (received)
        {
            case HELP_COMMAND:
                UART_PutString(HELP_MESSAGE);
                break;
            case SERVO_COMMAND:
                UART_PrintServoInfo();
                break;
            case ANGLE_COMMAND_SET:
                UART_StartMultiCharCmd(received);
                number_of_processed_bytes_angle += 1;
                break;
            default:
                UART_PutString("!!Unknown command\r\n");
                break;
        }
    }
}

/*
 *  \brief Start multi char command mode.
 */
void UART_StartMultiCharCmd(char received)
{
    multi_char_cmd = received;
    is_multi_char_cmd = 1;
}

/*
 *  \brief Stop multi char command mode.
 */
void UART_StopMultiCharCmd()
{
    is_multi_char_cmd = 0;
}

/*
 *  \brief Process incoming angle commands.
 */
void UART_ProcessAngleCommand(char received)
{
    if ( (number_of_processed_bytes_angle < (ANGLE_COMMANDS_N_BYTES - 1) ) && received == ANGLE_COMMAND_LATCH)
    {
        UART_PutString("Too few bytes!\r\n");
        UART_PutString("Correct usage: dXXXD\r\n");
        UART_StopMultiCharCmd();
        number_of_processed_bytes_angle = 0;
        return;
    }
    
    switch (number_of_processed_bytes_angle)
    {
        case 1:
            optional_args_buffer_angle[0] = received;
            break;
        case 2:
            optional_args_buffer_angle[1] = received;
            break;
        case 3:
            optional_args_buffer_angle[2] = received;
            break;
        case 4:
            if ( received != ANGLE_COMMAND_LATCH )
            {
                number_of_processed_bytes_angle = 0;
                UART_StopMultiCharCmd();
                UART_PutString("4th char not D\r\n");
            }
            break;
        default:
            /* Shouldn't get here..*/
            UART_PutString("Error in multi char command!\r\n");
            UART_PutString("Too many bytes!\r\n");
            UART_PutString("Correct usage: dXXXD\r\n");
            number_of_processed_bytes_angle = 0;
            UART_StopMultiCharCmd();
            break;
    }
    
    number_of_processed_bytes_angle += 1;
    if (number_of_processed_bytes_angle == ANGLE_COMMANDS_N_BYTES)
    {
        uint8_t angle = 0;
        if ( optional_args_buffer_angle[0] == '0' )
        {
            if ( optional_args_buffer_angle[1] == '0')
            {
                angle = optional_args_buffer_angle[2] - '0';
            }
            else
            {
                uint8_t dec = (optional_args_buffer_angle[1] - '0');
                if ( dec > 9 )
                {
                    UART_PutString("Error in multi char command!\r\n");
                    UART_PutString("Invalid value in packet!\r\n");
                    number_of_processed_bytes_angle = 0;
                    UART_StopMultiCharCmd();
                    return;
                }
                uint8_t un = (optional_args_buffer_angle[2] - '0');
                if ( dec > 9 || un > 9 )
                {
                    UART_PutString("Error in multi char command!\r\n");
                    UART_PutString("Invalid value in packet!\r\n");
                    number_of_processed_bytes_angle = 0;
                    UART_StopMultiCharCmd();
                    return;
                }
                dec *= 10;
                angle = dec + un;
            }
        }
        else
        {   
            uint8_t cen = (optional_args_buffer_angle[0] - '0');
            uint8_t dec = (optional_args_buffer_angle[1] - '0');
            uint8_t un = (optional_args_buffer_angle[2] - '0');
            if ( cen > 9 || dec > 9 || un > 9 )
            {
                UART_PutString("Error in multi char command!\r\n");
                UART_PutString("Invalid value in packet!\r\n");
                number_of_processed_bytes_angle = 0;
                UART_StopMultiCharCmd();
                return;
            }
            cen *= 100;
            dec *= 10;
            angle = cen + dec + un;
        }
        
        sprintf(buf, "Received Angle: %d\r\n", angle);
        UART_PutString(buf);
        if ( angle > 180)
        {
            UART_PutString("Invalid angle!\r\n");
        }
        else
        {
            Servo_SetPosition(angle);
            servo_angle = angle; 
        }
        
        UART_StopMultiCharCmd();
        number_of_processed_bytes_angle = 0;
    }
}

/* [] END OF FILE */

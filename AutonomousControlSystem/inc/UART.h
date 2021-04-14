/******************************************************************************
 *
 * File Name:   UART.h
 *
 * Description: UART source file to initialize UART module
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#ifndef UART_H
#define UART_H

#define TERMINATE_STRING 'o'

#include <AutonomousControlSystem/inc/common_includes.h>
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "inc/hw_uart.h"
#include "driverlib/usb.h"


extern SemaphoreHandle_t Sem_UARTReceive;
extern void UART1IntHandler(void);
extern void UART1_Init(int);


#endif

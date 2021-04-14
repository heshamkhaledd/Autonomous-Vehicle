/*
 * UART_TASK.h
 *
 *  Created on: 8 Oct 2019
 *      Author: okasha
 */

#ifndef UART_TASK_H_
#define UART_TASK_H_

#include "common_includes.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "inc/hw_uart.h"
#include "state_decoding.h"

/**********************************************************
 *                    Dynamic Configuration
 ***********************************************************/

/* Rtos_Task Configurations */
#define UART_STACK_DEPTH 200
#define UART_vTASK_PRIO 1

/* decodeing */
#define Numbers_Ascii_Base 0x30
#define Numbers_Ascii_Offset 9


/**********************************************************
 *                    Dynamic Configuration
 ***********************************************************/

extern SemaphoreHandle_t Sem_UARTReceive;

/**********************************************************
 *                    Dynamic Configuration
 ***********************************************************/

extern void vTask_UART(void * param);
extern void vInit_UART();


#endif /* UART_TASK_H_ */

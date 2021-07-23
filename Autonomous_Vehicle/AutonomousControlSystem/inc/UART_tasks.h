 /******************************************************************************
 *
 * File Name:   UART_TASK.h
 *
 * Description: UART Tasks source file to initialize UART module tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/


#ifndef UART_TASK_H_
#define UART_TASK_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <AutonomousControlSystem/inc/state_decoding.h>
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "inc/hw_uart.h"

/**********************************************************
 *                    Dynamic Configuration
 ***********************************************************/

#define BAUD_RATE       9600

/* Rtos_Task Configurations */
#define UART_STACK_DEPTH 200
#define UART_vTASK_PRIO 1

/* Decodeing */
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
extern void UART1IntHandler(void);
extern void UART1_Init(int);


#endif /* UART_TASK_H_ */

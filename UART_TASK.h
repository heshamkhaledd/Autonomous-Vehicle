/*
 * UART_TASK.h
 *
 *  Created on: 8 Oct 2019
 *      Author: okasha
 */

#ifndef UART_TASK_H_
#define UART_TASK_H_

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"
#include "driverlib/usb.h"


#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"

#include "inc/hw_uart.h"

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

/*
extern volatile UBaseType_t UARTTaskHighWaterMark ;*/

extern QueueHandle_t Queue_Current_Orientation;
extern SemaphoreHandle_t Sem_UARTReceive;

/**********************************************************
 *                    Dynamic Configuration
 ***********************************************************/

extern void vTask_UART(void * param);
extern void vInit_UART();


#endif /* UART_TASK_H_ */

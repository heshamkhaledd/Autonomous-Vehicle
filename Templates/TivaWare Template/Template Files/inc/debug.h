/*
 * debug.h
 *
 *  Created on: Oct 22, 2021
 *      Author: Tony
 */

#ifndef TEMPLATE_FILES_INC_DEBUG_H_
#define TEMPLATE_FILES_INC_DEBUG_H_

/*Standard*/
#include <stdint.h>
#include <stdbool.h>

/* Tiva-Ware Library Macros/Defines Includes */
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

/* Tiva-Ware Library Drivers Includes */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_uart.h"

/*******************************************************************************
*                            UART Functions Prototypes                        *
*******************************************************************************/

void UART0_init();
void UART_receiveString (uint32_t ui32Base, uint8_t *Str);
void UART_sendString (uint32_t ui32Base, const uint8_t *Str);
void UART_sendNumber(int32_t decimalValue);

#endif /* TEMPLATE_FILES_INC_DEBUG_H_ */

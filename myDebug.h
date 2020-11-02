/**
 * @file myDebug.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief This lib is used only for debugging purpose via USART 
 * @version 0.1
 * @date 2020-09-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/****************************************** OPTIONAL CONFIGURATION - BEGIN */
#define configHAL_UART
//#define USE_DMA_TX
//#define configLL_UART
/****************************************** OPTIONAL CONFIGURATION - END */

/****************************************** GENERAL-DEFINE-BEGIN */

#define VARIABLE_BUFFER_SIZE (10U)
#define STRING_BUFFER_SIZE (100U)

/* String used to store temporary string data */
char ucGeneralString[VARIABLE_BUFFER_SIZE];

/* Reset cause enumeration */
typedef enum
{
    eRESET_CAUSE_UNKNOWN = 0,
    eRESET_CAUSE_LOW_POWER_RESET,
    eRESET_CAUSE_WINDOW_WATCHDOG_RESET,
    eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET,
    eRESET_CAUSE_SOFTWARE_RESET,
    eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET,
    eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET,
    eRESET_CAUSE_BROWNOUT_RESET,
} reset_cause_t;

/* Check reset flags in RCC_CSR registers to clarify reset cause */
reset_cause_t resetCauseGet(void);

/* Get reset cause name in string */
const char *resetCauseGetName(reset_cause_t reset_cause);


#if defined(configHAL_UART) /* configHAL_UART */
#define DEBUG_USART huart2
/* Print out a string to USART */
void vUARTSend(UART_HandleTypeDef huart, uint8_t *String);

#elif defined(configLL_UART) /* configLL_UART */
#define DEBUG_USART USART2
void vUARTSend(USART_TypeDef *USARTx, uint8_t *String);
#endif

#define PRINTF(str)                             \
    do                                          \
    {                                           \
        vUARTSend(DEBUG_USART, (uint8_t *)str); \
    } while (0)

#define PRINT_VAR(var)                                      \
    do                                                      \
    {                                                       \
        vUARTSend(DEBUG_USART, (uint8_t *)#var);            \
        vUARTSend(DEBUG_USART, (uint8_t *)" = ");           \
        itoa(var, ucGeneralString, 10);                     \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString); \
        newline;                                            \
    } while (0)

/* Print out a desirable number of new line "\r\n" to debug terminal */
#define PRINT_NEWLINE(nb_of_new_line)               \
    do                                              \
    {                                               \
        for (size_t i = 0; i < nb_of_new_line; i++) \
        {                                           \
            newline;                                \
        }                                           \
    } while (0)

#define newline vUARTSend(DEBUG_USART, (uint8_t *)"\r\n");

/****************************************** GENERAL-DEFINE-END */

#endif /* __MYLIB_H */

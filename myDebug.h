/**
 * @file myDebug.h - This is the succeeding version of myLib.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief This lib is used only for debugging purpose via USART 
 * @version 0.1
 * @date 2020-11-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_DEBUG_H /* __MY_DEBUG_H */
#define __MY_DEBUG_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/**
 * @brief Optional configuration
 * 
 */
#define configHAL_UART
//#define USE_DMA_TX
//#define configLL_UART


/********************************************************************************************************/

/**
 * @brief Reset Cause
 * 
 */
/* Reset cause enumeration */
typedef enum reset_cause
{
    eRESET_CAUSE_UNKNOWN = 0,
    eRESET_CAUSE_LOW_POWER_RESET,                   /*  */
    eRESET_CAUSE_WINDOW_WATCHDOG_RESET,             /*  */
    eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET,        /* IWDG Timeout */
    eRESET_CAUSE_SOFTWARE_RESET,                    /* Reset caused by NVIC_SystemReset() */
    eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET,         /*  */
    eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET,          /* Low signal on NRST pin | Reset pin pushed */
    eRESET_CAUSE_BROWNOUT_RESET,                    /*  */
} reset_cause_t;    

/* Check reset flags in RCC_CSR registers to clarify reset cause */
reset_cause_t resetCauseGet(void);

/* Get reset cause name in string */
const char *resetCauseGetName(reset_cause_t reset_cause);
/********************************************************************************************************/


/**
 * @brief IWDG 
 * 
 */
#define PRESCALER_128_UPPER_LIMIT   (13107u)
#define PRESCALER_256_UPPER_LIMIT   (26214u)
#define IWDG_RESOLUTION (4096u)

IWDG_HandleTypeDef _hiwdg;

/********************************************************************************************************/
/* Init Independant watchdog timer */
void vIWDG_Init(float millis);

/**
 * @brief Print debugging log via USART
 * 
 */
/* String used to store temporary string data */

#define VARIABLE_BUFFER_SIZE (10U)
#define STRING_BUFFER_SIZE (100U)

char ucGeneralString[VARIABLE_BUFFER_SIZE];

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
/********************************************************************************************************/

#endif /* !__MY_DEBUG_H */

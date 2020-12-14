/**
 * @file myRTOSaddons.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __MY_RTOS_ADDONS
#define __MY_RTOS_ADDONS

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "task.h"
#include "timers.h"
#include "main.h"
#include "myDebug.h"

extern char ucGeneralString[];


#if (defined(USE_RETARGET_PRINTF)) /* USE_RETARGET_PRINTF */

#define PRINT_IN_TASK(str)          (printf("[%s] - %s", pcTaskGetName(NULL), str))
#define PRINT_VAR_IN_TASK(var)      (printf("[%s] - " #var " = %ld\r\n", pcTaskGetName(NULL), var))

#elif                              /* !USE_RETARGET_PRINTF */
#define PRINT_IN_TASK(str)                                      \
    do                                                          \
    {                                                           \
        vUARTSend(DEBUG_USART, (uint8_t *)"[");                 \
        vUARTSend(DEBUG_USART, (uint8_t *)pcTaskGetName(NULL)); \
        vUARTSend(DEBUG_USART, (uint8_t *)"] - ");              \
        vUARTSend(DEBUG_USART, (uint8_t *)str);                 \
    } while (0)



#define PRINT_VAR_IN_TASK(var)                                              \
    do                                                                      \
    {                                                                       \
            vUARTSend(DEBUG_USART, (uint8_t *)"[");                         \
            vUARTSend(DEBUG_USART, (uint8_t *)pcTaskGetName(NULL));         \
            vUARTSend(DEBUG_USART, (uint8_t *)"] - ");              \
            vUARTSend(DEBUG_USART, (uint8_t *)#var);                \
            vUARTSend(DEBUG_USART, (uint8_t *)" = ");               \
            itoa(var, ucGeneralString, 10);                         \
            vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);     \
            newline;                                                \
    }                                                                        \
while (0)

/* #define PRINT_IN_SWTIMER(timer_id, str)                                      \
    do                                                                       \
    {                                                                        \
        vUARTSend(DEBUG_USART, (uint8_t *)"[");                              \
        vUARTSend(DEBUG_USART, (uint8_t *)osTimerGetName((void *)timer_id)); \
        vUARTSend(DEBUG_USART, (uint8_t *)"] - ");                           \
        vUARTSend(DEBUG_USART, (uint8_t *)str);                              \
    } while (0)

#define PRINT_VAR_INSWTIMER(timer_id, var)                                   \
    do                                                                       \
    {                                                                        \
        vUARTSend(DEBUG_USART, (uint8_t *)"[");                              \
        vUARTSend(DEBUG_USART, (uint8_t *)osTimerGetName((void *)timer_id)); \
        vUARTSend(DEBUG_USART, (uint8_t *)"] - ");                           \
        vUARTSend(DEBUG_USART, (uint8_t *)#var);                             \
        vUARTSend(DEBUG_USART, (uint8_t *)" = ");                            \
        itoa(var, ucGeneralString, 10);                                      \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString);                  \
        newline;                                                             \
    } while (0) */
#endif /* !USE_RETARGET_PRINTF */

#endif /* !__MY_RTOS_ADDONS */

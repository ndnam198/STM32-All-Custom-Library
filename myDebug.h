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

#include <string.h>
#include "main.h"
#include "retarget.h"
#include "myF767.h"
//#include "myF103.h"

/* -------------------------------------------------------------------------- */
/*                            USER-OPTIONAL-DEFINE                            */
/* -------------------------------------------------------------------------- */

#define configHAL_UART
#define USE_RETARGET_PRINTF
//#define configLL_UART
#define PRINT_DEBUG /* Enable print debug information */
#define IWDG_TIME (5000u)

#if defined(STM32_F1)
#define huartX huart2
#define USARTX UART2
#define CLI_TIMER TIM3
#elif defined(STM32_F7)
#define huartX huart7
#define USARTX UART7
#define CLI_TIMER TIM14
#endif /* !STM32_F1*/
/* -------------------------------------------------------------------------- */
/*                         TIME-PROCESSING-MANAGEMENT                         */
/* -------------------------------------------------------------------------- */

typedef struct TimeStamp
{
    uint32_t Hours;
    uint32_t Minutes;
    uint32_t Seconds;
    uint32_t Millis;
} TimeStamp_t;

typedef struct MCUProcessingEvaluate
{
    uint32_t min_process_time;
    uint32_t max_process_time;
    uint32_t current_process_time;
} MCUProcessingEvaluate_t;

#define __PRINT_TIME_STAMP() (vTimeStamp(HAL_GetTick()))
/**
 * @brief Evaluate MCU processing time every superloop elapsed
 * 
 * @param mcu_process_time_handle 
 * @param current_processing_time 
 */
void vMCUProcessTimeUpdate(MCUProcessingEvaluate_t *mcu_process_time_handle,
                           uint32_t current_processing_time);

void vPrintProcessingTime(MCUProcessingEvaluate_t *mcu_process_time_handle);

/**
 * @brief Calculate TimeStamp based on current ticks values
 * 
 * @param now_tick normally passed in HAL_GetTicks(
 */
void vTimeStamp(uint32_t now_tick);

/* -------------------------------------------------------------------------- */
/*           MCU-RESET-CAUSE-MANAGEMENT & INDEPENDENCE-WATCHDOG-INIT          */
/* -------------------------------------------------------------------------- */

typedef enum reset_cause
{
    eRESET_CAUSE_UNKNOWN = 0,
    eRESET_CAUSE_LOW_POWER_RESET,            /*  */
    eRESET_CAUSE_WINDOW_WATCHDOG_RESET,      /*  */
    eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET, /* IWDG Timeout */
    eRESET_CAUSE_SOFTWARE_RESET,             /* Reset caused by NVIC_SystemReset() */
    eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET,  /*  */
    eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET,   /* Low signal on NRST pin | Reset pin pushed */
    eRESET_CAUSE_BROWNOUT_RESET,             /*  */
} reset_cause_t;

#define __PRINT_RESET_CAUSE()                                                                     \
    do                                                                                            \
    {                                                                                             \
        newline;                                                                                  \
        printf("/*----------------------------------------------------------------------*/\r\n"); \
        printf("Reset cause: %s\r\n", resetCauseGetName(resetCauseGet()));                        \
    } while (0)

/**
 * @brief Check reset flags in RCC_CSR registers to clarify reset cause
 * 
 * @return reset_cause_t 
 */
reset_cause_t resetCauseGet(void);

/**
 * @brief Get reset cause name in string
 * 
 * @param reset_cause 
 * @return __weak const* 
 */
__weak const char *resetCauseGetName(reset_cause_t reset_cause);

/**
 * @brief IWDG 
 * 
 */
#if defined(STM32_F1)
#define PRESCALER_128_UPPER_LIMIT (13107u)
#define PRESCALER_256_UPPER_LIMIT (26214u)
#define IWDG_RESOLUTION (4095u)
#endif /* !STM32_F1 */
/**
 * @brief Init Independant watchdog timer
 * 
 * @param hiwdg 
 * @param millis 
 */
void vIWDG_Init(IWDG_HandleTypeDef *hiwdg, uint32_t millis);

/* -------------------------------------------------------------------------- */
/*                            ERROR-HANDLE-FUNCTION                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief  This function is executed in case of error occurrence, defined as __weak
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
__weak void _Error_Handler(char *file, int line);

/* -------------------------------------------------------------------------- */
/*                              USART-DEBUG-PRINT                             */
/* -------------------------------------------------------------------------- */

#define VARIABLE_BUFFER_SIZE (10U)
#define STRING_BUFFER_SIZE (100U)

#if defined(configHAL_UART) /* configHAL_UART */
char ucGeneralString[VARIABLE_BUFFER_SIZE];
extern UART_HandleTypeDef huartX;
#define DEBUG_USART huartX
/* Print out a string to USART */
void vUARTSend(UART_HandleTypeDef huart, uint8_t *String);

#elif defined(configLL_UART) /* configLL_UART */
#define DEBUG_USART USARTX
void vUARTSend(USART_TypeDef *USARTx, uint8_t *String);
#endif

/* Retarget debug USART to use printf */
#define __RETARGET_INIT(__USART_INSTANCE__) (RetargetInit(&(__USART_INSTANCE__)))

#if (defined(USE_RETARGET_PRINTF)) /* USE_RETARGET_PRINTF */
#define PRINTF (printf)
#define PRINT_VAR(var) (printf(#var " = %lu\r\n", var))
#define PRINT_ADDRESS(var) (printf("Address of " #var " : %d\r\n", &var))
#define PRINT_ARRAY(array, offset)                              \
    do                                                          \
    {                                                           \
        for (uint32_t i = offset; i < ARRAY_LENGTH(array); i++) \
        {                                                       \
            printf(#array "[%lu] = %lu\r\n", i, array[i]);      \
        }                                                       \
    } while (0)
#else /* !USE_RETARGET_PRINTF */
#define PRINTF(str) (vUARTSend(DEBUG_USART, (uint8_t *)str))
#define PRINT_VAR(var)                                      \
    do                                                      \
    {                                                       \
        vUARTSend(DEBUG_USART, (uint8_t *)#var);            \
        vUARTSend(DEBUG_USART, (uint8_t *)" = ");           \
        itoa(var, ucGeneralString, 10);                     \
        vUARTSend(DEBUG_USART, (uint8_t *)ucGeneralString); \
        newline;                                            \
    } while (0)
#endif /* !USE_RETARGET_PRINTF */

/* Print out a desirable number of new line "\r\n" to debug terminal */
#define PRINT_NEWLINE(nb_of_new_line)               \
    do                                              \
    {                                               \
        for (size_t i = 0; i < nb_of_new_line; i++) \
        {                                           \
            newline;                                \
        }                                           \
    } while (0)

#define newline (printf("\r\n"))

/********************************************************************************************************/

#endif /* !__MY_DEBUG_H */

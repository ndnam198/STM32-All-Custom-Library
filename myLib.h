/**
 * @file myLib.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief This lib is used only for extra peripheral definitions, such as button, led or buzzer, etc...
 *        Any others could be Print function which's for debug purposes
 * @version 0.1
 * @date 2020-09-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MYLIB_H
#define __MYLIB_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/* GENERAL-DEFINE-BEGIN */
#define VARIABLE_BUFFER_SIZE (10U)
#define STRING_BUFFER_SIZE (100U)
char ucGeneralString[VARIABLE_BUFFER_SIZE];

#define BKFET_BOARD
#define isString(src, des) ((strcmp((char *)src, des)) == 0 ? 1 : 0)
#define NEWLINE(nb_of_new_line)                     \
    do                                              \
    {                                               \
        for (size_t i = 0; i < nb_of_new_line; i++) \
        {                                           \
            newline;                                \
        }                                           \
    } while (0)

/* GENERAL-DEFINE-END */

/**
 * @brief Choose whether to use LL or HAL library for UART -----(OPTIONAL)
 *
 */
//#define configHAL_UART

#if defined(configHAL_UART) /* configHAL_UART */
#define DEBUG_USART huart2
void vUARTSend(UART_HandleTypeDef huart, uint8_t *String);

#elif defined(configLL_UART) /* configLL_UART */
#define DEBUG_USART USART2
void vUARTSend(USART_TypeDef *USARTx, uint8_t *String);
#endif

uint32_t ucRandomNumber(uint32_t min, uint32_t max);

#define toggleLed1 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
#define toggleLed2 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
#define toggleLed3 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
#define toggleLed4 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
#define toggleAllLed \
    do               \
    {                \
        toggleLed1;  \
        toggleLed2;  \
        toggleLed3;  \
        toggleLed4;  \
    } while (0)
#define toggleBuzzer HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
#define onLed1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
#define onLed2 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
#define onLed3 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
#define onLed4 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
#define onAllLed \
    do           \
    {            \
        onLed1;  \
        onLed2;  \
        onLed3;  \
        onLed4;  \
    } while (0)
#define onBuzzer HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
#define offLed1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
#define offLed2 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
#define offLed3 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
#define offLed4 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
#define offAllLed \
    do            \
    {             \
        offLed1;  \
        offLed2;  \
        offLed3;  \
        offLed4;  \
    } while (0)
#define offBuzzer HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

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

#define newline vUARTSend(DEBUG_USART, (uint8_t *)"\r\n");

#define RAND_U32(min, max) (ucRandomNumber(min, max))
#endif /* __MYLIB_H */

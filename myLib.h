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
#include "main.h"

/**
 * @brief General define for all board used
 * 
 */

#define IsString(src, des) ((strcmp((char *)src, des)) == 0 ? 1 : 0)

/**
 * @brief Define BOARD in use -- (OPTIONAL)
 * 
 */
#define BKFET_BOARD
//#define STM32F407G_DISC1
//#define NUCLEO_F767ZI

/**
 * @brief  If using BKFET_BOARD 
 * 
 */
#ifdef BKFET_BOARD

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

#endif

/**
 * @brief If using STM32F407G_DISC1 
 * 
 */
#ifdef STM32F407G_DISC1
#endif

/**
 * @brief  If using NUCLEO_F767ZI
 * 
 */
#ifdef NUCLEO_F767ZI
#endif

/**
 * @brief Choose whether to use LL or HAL library for UART -----(OPTIONAL)
 * 
 */
#define configLL_UART
//#define configHAL_UART
char ucGeneralString[100];

/**
 * @brief #ifdef configHAL_UART
 * 
 */
#ifdef configHAL_UART

#define defineUART_TX_DELAY 1
#define USED_UART huart2

void vUARTSend(UART_HandleTypeDef huart, uint8_t *String);

#endif /* configHAL_UART */

/**
 * @brief #ifdef configLL_UART
 * 
 */
#ifdef configLL_UART

#define USED_UART USART2

void vUARTSend(USART_TypeDef *USARTx, uint8_t *String);
#endif /* configLL_UART */

#define print(str)                                        \
    do                                                    \
    {                                                     \
        sprintf(ucGeneralString, "%s", (char *)str);      \
        vUARTSend(USED_UART, (uint8_t *)ucGeneralString); \
    } while (0)

#define printVar(var)                                            \
    do                                                           \
    {                                                            \
        sprintf(ucGeneralString, "Value of " #var " = %lu", var); \
        vUARTSend(USED_UART, (uint8_t *)ucGeneralString);        \
        print("\r\n");                                           \
    } while (0)

#define endln vUARTSend(USED_UART, (uint8_t *)"\r\n");
/**
 * END of USART --------------------------------------------------------------------------------------------------------------------
 * 
 */

#endif /* __MYLIB_H */

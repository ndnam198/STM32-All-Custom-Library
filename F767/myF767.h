/**
 * @file myF767.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef F767_NUCLEO_H
#define F767_NUCLEO_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/* -------------------------------------------------------------------------- */
/*                                BOARD DEFINE                                */
/* -------------------------------------------------------------------------- */
#define STM32_F7 ((void)0U)

#define LED_PORT GPIOB
#define LED_GREEN GPIO_PIN_0 /* Port B */
#define LED_BLUE GPIO_PIN_7  /* Port B */
#define LED_RED GPIO_PIN_14  /* Port B */

#define LED4_OFF() (GPIOB->BSRR |= 0x00000100)
#define LED4_ON() (GPIOB->BRR |= 0x00000100)
#define LED1_OFF() (GPIOB->BSRR |= 0x00000020)
#define LED1_ON() (GPIOB->BRR |= 0x00000020)

#define OFF (0U)
#define ON (1U)

#define TOGGLE_LED(__LED_NUMBER__) \
    (((__LED_NUMBER__) == LED_GREEN) ? (HAL_GPIO_TogglePin(LED_PORT, LED_GREEN)) : ((__LED_NUMBER__) == LED_BLUE) ? (HAL_GPIO_TogglePin(LED_PORT, LED_BLUE)) : (HAL_GPIO_TogglePin(LED_PORT, LED_RED)))

#define WRITE_LED(__LED_NUMBER__, __STATE__) \
    (((__LED_NUMBER__) == LED_GREEN) ? (HAL_GPIO_WritePin(LED_PORT, LED_GREEN, __STATE__)) : ((__LED_NUMBER__) == LED_BLUE) ? (HAL_GPIO_WritePin(LED_PORT, LED_BLUE, __STATE__)) : (HAL_GPIO_WritePin(LED_PORT, LED_RED, __STATE__)))

#define TOGGLE_ALL_LED()       \
    do                         \
    {                          \
        TOGGLE_LED(LED_GREEN); \
        TOGGLE_LED(LED_BLUE);  \
        TOGGLE_LED(LED_RED);   \
    } while (0)

#define ON_ALL_LED()              \
    do                            \
    {                             \
        WRITE_LED(LED_GREEN, ON); \
        WRITE_LED(LED_BLUE, ON);  \
        WRITE_LED(LED_RED, ON);   \
    } while (0)

#define OFF_ALL_LED()              \
    do                             \
    {                              \
        WRITE_LED(LED_GREEN, OFF); \
        WRITE_LED(LED_BLUE, OFF);  \
        WRITE_LED(LED_RED, OFF);   \
    } while (0)

#endif /* !F767_NUCLEO_H */

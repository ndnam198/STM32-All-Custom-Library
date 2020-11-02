/**
 * @file myF103.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __MY_F103_BOARD_H
#define __MY_F103_BOARD_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "myDebug.h"

#define BKFET_BOARD


/****************************************** BKFET_BOARD DEFINE - BEGIN */
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
/****************************************** BKFET_BOARD DEFINE - END */

#endif /* !__MY_F103_BOARD_H */

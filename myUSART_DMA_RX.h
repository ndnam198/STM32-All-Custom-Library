/**
 * @file myUSART_DMA_RX
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief This lib is used for ultilizing DMA channel to process user input data via USART 
 * @version 0.1
 * @date 2020-09-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_USART_DMA_RX
#define __MY_USART_DMA_RX

#define USART_DMA_RX_BUFFER_SIZE (100UL)

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "main.h"

#ifdef BKFET_BOARD

__weak void DMA1_Channel6_IRQHandler(void);
__weak void USART2_IRQHandler(void);

#endif /* BKFET_BOARD */

#endif /* __MY_USART_DMA_RX */

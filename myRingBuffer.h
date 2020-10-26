/**
 * @file myRingBuffer.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief This file provides method to implement Fifo Buffer (Ring buffer)
 * @version 0.1
 * @date 2020-10-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_RINGBUFFER_H
#define __MY_RINGBUFFER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

#define IS_NULL_POINTER(ptr) ((ptr == NULL)?(1ul):(0ul))

typedef enum
{
    FIFO_SUCCESS = 0,
    FIFO_BUFFER_FULL,
    FIFO_BUFFER_EMPTY,
    FIFO_FAILED
} FIFO_Status_t;

typedef struct
{
    uint32_t Number_of_Element; /* Number of elements defined by user */
    uint32_t Size;              /* Equal to Number_of_Element + 1 */
    uint8_t *Data_Array;        /* Pointer to ring buffer dynamically created by malloc/calloc */
    uint32_t write_index;       /* Write pointer offset */
    uint32_t read_index;        /* Read pointer offset */
} FIFO_StructTypedef;

/**
 * @brief Configure passed-in FIFO_Struct's parameters based on demand size
 * 
 * @param FIFO_Struct   : A FIFO_Struct's pointer 
 * @param number_of_element  : Size of ring buffer will be created
 */
FIFO_Status_t FIFO_Init(FIFO_StructTypedef *FIFO_Struct, uint32_t number_of_element);

/**
 * @brief 
 * 
 * @param FIFO_Struct 
 * @param data 
 * @return FIFO_Status_t 
 */
FIFO_Status_t FIFO_WriteData(FIFO_StructTypedef *FIFO_Struct, uint8_t data);

/**
 * @brief 
 * 
 * @param FIFO_Struct 
 * @param data 
 * @return FIFO_Status_t 
 */
FIFO_Status_t FIFO_ReadData(FIFO_StructTypedef *FIFO_Struct, uint8_t *data);

#endif /* !__MY_RINGBUFFER_H */

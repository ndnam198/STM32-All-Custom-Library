/**
 * @file myI2C.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MYI2C_H /* __MYI2C_H */
#define __MYI2C_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/* I2C1: SDA - PB7; SCL - PB6 */
#define SDA_LOW() (GPIOB->BRR |= 0x00000080)  // set SDA to low
#define SDA_OPEN() (GPIOB->BSRR |= 0x00000080) // set SDA to open-drain
#define SDA_READ (GPIOB->IDR & 0x0080)        // read SDA
#define SCL_LOW() (GPIOB->BRR = 0x00000040)  // set SCL to low
#define SCL_OPEN() (GPIOB->BSRR = 0x00000040) // set SCL to open-drain
#define SCL_READ (GPIOB->IDR & 0x0040)        // read SCL

typedef enum
{
    NO_ERROR = 0x00,       // no error
    ACK_ERROR = 0x01,      // no acknowledgment error
    CHECKSUM_ERROR = 0x02, // checksum mismatch error
    TIMEOUT_ERROR = 0x04,  // timeout error
    PARM_ERROR = 0x80,     // parameter out of range error
} e_Error;

typedef enum
{
    FALSE = 0,
    TRUE = 1
} e_bool;

//-- Enumerations -------------------------------------------------------------
// I2C acknowledge
typedef enum
{
    ACK = 0,
    NACK = 1,
} e_ACK;

void DelayMicroSeconds(uint32_t nbrOfUs);
//=============================================================================
void I2c_Init(void);
//=============================================================================
// Initializes the ports for I2C interface.
//-----------------------------------------------------------------------------
//=============================================================================
void I2c_StartCondition(void);
//=============================================================================
// Writes a start condition on I2C-Bus.
//-----------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
// _____
// SDA: |_____
// _______
// SCL: |___
//=============================================================================
void I2c_StopCondition(void);
//=============================================================================
// Writes a stop condition on I2C-Bus.
//-----------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
// _____
// SDA: _____|
// _______
// SCL: ___|
//=============================================================================
e_Error I2c_WriteByte(uint8_t txByte);
//=============================================================================
// Writes a byte to I2C-Bus and checks acknowledge.
//-----------------------------------------------------------------------------
//
// return: error: ACK_ERROR = no acknowledgment from sensor
// NO_ERROR = no error
//
// remark: Timing (delay) may have to be changed for different microcontroller.
//=============================================================================
e_Error I2c_ReadByte(uint8_t *rxByte, e_ACK ack, uint8_t timeout);
e_Error I2c_GeneralCallReset(void);

#endif /* !__MYI2C_H */

/**
 * @file mySHT3x.h
 * @author your name (you@domain.com)
 * @brief SHT3x is a very high accuracy measurement proficiency Temperature & Humidity sensor   
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __MY_SHT3x_H /* __MY_SHT3x_H */
#define __MY_SHT3x_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "retarget.h"
#include "myDebug.h"

#define SHT3x_I2C_ADDRESS_A (0x44 << 1) /* default value when ADDR pin connected to VSS */
#define SHT3x_I2C_ADDRESS_B (0x45 << 1) /* ADDR pin connected to VDD */

#define SHT3x_DATA_LENGTH (6U)
#define SHT3x_COMMAND_LENGTH (2U)

#define SHT3x_COMMAND_TIMEOUT (10U)

#define CRC_POLYNOMIAL (0x131)

#define MEASUREMENT_DURATION_LOW_REPEATABILITY (4u)    /* 4ms */
#define MEASUREMENT_DURATION_MEDIUM_REPEATABILITY (6u) /* 6ms */
#define MEASUREMENT_DURATION_HIGH_REPEATABILITY (15u)  /* 15ms */

/* Single shot measurement command definition */
#define SINGLE_SHOT_HIGH_REPEATABILITY_CLOCK_STRETCH_ENABLE (0x2C06)
#define SINGLE_SHOT_MEDIUM_REPEATABILITY_CLOCK_STRETCH_ENABLE (0x2C0D)
#define SINGLE_SHOT_LOW_REPEATABILITY_CLOCK_STRETCH_ENABLE (0x2C10)
#define SINGLE_SHOT_HIGH_REPEATABILITY_CLOCK_STRETCH_DISABLE (0x2400)
#define SINGLE_SHOT_MEDIUM_REPEATABILITY_CLOCK_STRETCH_DISABLE (0x240B)
#define SINGLE_SHOT_LOW_REPEATABILITY_CLOCK_STRETCH_DISABLE (0x2416)

/* Periodic data acquisition mode */
#define PERIODIC_HIGH_REPEATABILITY_0_5 (0x2032)
#define PERIODIC_MEDIUM_REPEATABILITY_0_5 (0x2024)
#define PERIODIC_LOW_REPEATABILITY_0_5 (0x202F)
#define PERIODIC_HIGH_REPEATABILITY_1 (0x2130)
#define PERIODIC_MEDIUM_REPEATABILITY_1 (0x2126)
#define PERIODIC_LOW_REPEATABILITY_1 (0x212D)
#define PERIODIC_HIGH_REPEATABILITY_2 (0x2236)
#define PERIODIC_MEDIUM_REPEATABILITY_2 (0x2220)
#define PERIODIC_LOW_REPEATABILITY_2 (0x222B)
#define PERIODIC_HIGH_REPEATABILITY_4 (0x2334)
#define PERIODIC_MEDIUM_REPEATABILITY_4 (0x2322)
#define PERIODIC_LOW_REPEATABILITY_4 (0x2329)
#define PERIODIC_HIGH_REPEATABILITY_10 (0x2737)
#define PERIODIC_MEDIUM_REPEATABILITY_10 (0x2721)
#define PERIODIC_LOW_REPEATABILITY_10 (0x272A)

/* Stop periodic measurement */
#define STOP_PERIODIC_DATA_ACQUISITION_MODE (0x3093)

/* Soft reset for SHT3x */
#define SHT3x_SOFT_RESET (0x30A2)

/* Read out SHT3x status register */
#define READ_OUT_STATUS_REGISTER (0xF3

typedef enum
{
    SHT31_OK = 0x00U,
    SHT31_FAILED,
    SHT31_BUSY
} SHT31_StatusTypeDef;

SHT31_StatusTypeDef SHT3x_sendCommand(I2C_HandleTypeDef *hi2c, uint8_t *command);
SHT31_StatusTypeDef SHT3x_readData(I2C_HandleTypeDef *hi2c, uint8_t *data_buffer);
void SHT3x_calculateTemp(uint16_t temp_data, float *temp_buffer);
void SHT3x_calculateHumid(uint16_t humid_data, float *humid_buffer);
SHT31_StatusTypeDef SHT3x_CRCCheck(uint8_t *data_frame);

#endif /* !__MY_SHT3x_H */
/**
 * @file mySHT31.h
 * @author your name (you@domain.com)
 * @brief SHT31 is a very high accuracy measurement proficiency Temperature & Humidity sensor   
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __MY_SHT31_H /* __MY_SHT31_H */
#define __MY_SHT31_H

#include <stdio.h>
#include "main.h"
#include "retarget.h"
#include "myDebug.h"
#include "myI2C.h"

#define SHT31_I2C_ADDRESS_A (0x44 << 1) /* default value when ADDR pin connected to VSS */
#define SHT31_I2C_ADDRESS_B (0x45 << 1) /* ADDR pin connected to VDD */

#define SHT31_DATA_LENGTH (6U)
#define SHT31_COMMAND_LENGTH (2U)

#define SHT31_COMMAND_TIMEOUT (10U)

#define CRC_POLYNOMIAL (0x131)

typedef enum
{
    eCMD_READ_SERIALNBR = 0x3780,  // read serial number
    eCMD_READ_STATUS = 0xF32D,     // read status register
    eCMD_CLEAR_STATUS = 0x3041,    // clear status register
    eCMD_HEATER_ENABLE = 0x306D,   // enabled heater
    eCMD_HEATER_DISABLE = 0x3066,  // disable heater
    eCMD_SOFT_RESET = 0x30A2,      // soft reset
    eCMD_MEAS_CLOCKSTR_H = 0x2C06, // measurement: clock stretching, high repeatability
    eCMD_MEAS_CLOCKSTR_M = 0x2C0D, // measurement: clock stretching, medium repeatability
    eCMD_MEAS_CLOCKSTR_L = 0x2C10, // measurement: clock stretching, low repeatability
    eCMD_MEAS_POLLING_H = 0x2400,  // measurement: polling, high repeatability
    eCMD_MEAS_POLLING_M = 0x240B,  // measurement: polling, medium repeatability
    eCMD_MEAS_POLLING_L = 0x2416,  // measurement: polling, low repeatability
    eCMD_MEAS_PERI_05_H = 0x2032,  // measurement: periodic 0.5 mps, high repeatability
    eCMD_MEAS_PERI_05_M = 0x2024,  // measurement: periodic 0.5 mps, medium repeatability
    eCMD_MEAS_PERI_05_L = 0x202F,  // measurement: periodic 0.5 mps, low repeatability
    eCMD_MEAS_PERI_1_H = 0x2130,   // measurement: periodic 1 mps, high repeatability
    eCMD_MEAS_PERI_1_M = 0x2126,   // measurement: periodic 1 mps, medium repeatability
    eCMD_MEAS_PERI_1_L = 0x212D,   // measurement: periodic 1 mps, low repeatability
    eCMD_MEAS_PERI_2_H = 0x2236,   // measurement: periodic 2 mps, high repeatability
    eCMD_MEAS_PERI_2_M = 0x2220,   // measurement: periodic 2 mps, medium repeatability
    eCMD_MEAS_PERI_2_L = 0x222B,   // measurement: periodic 2 mps, low repeatability
    eCMD_MEAS_PERI_4_H = 0x2334,   // measurement: periodic 4 mps, high repeatability
    eCMD_MEAS_PERI_4_M = 0x2322,   // measurement: periodic 4 mps, medium repeatability
    eCMD_MEAS_PERI_4_L = 0x2329,   // measurement: periodic 4 mps, low repeatability
    eCMD_MEAS_PERI_10_H = 0x2737,  // measurement: periodic 10 mps, high repeatability
    eCMD_MEAS_PERI_10_M = 0x2721,  // measurement: periodic 10 mps, medium repeatability
    eCMD_MEAS_PERI_10_L = 0x272A,  // measurement: periodic 10 mps, low repeatability
    eCMD_FETCH_DATA = 0xE000,      // readout measurements for periodic mode
    eCMD_R_AL_LIM_LS = 0xE102,     // read alert limits, low set
    eCMD_R_AL_LIM_LC = 0xE109,     // read alert limits, low clear
    eCMD_R_AL_LIM_HS = 0xE11F,     // read alert limits, high set
    eCMD_R_AL_LIM_HC = 0xE114,     // read alert limits, high clear
    eCMD_W_AL_LIM_HS = 0x611D,     // write alert limits, high set
    eCMD_W_AL_LIM_HC = 0x6116,     // write alert limits, high clearCMD_W_AL_LIM_LC = 0x610B, // write alert limits, low clear
    eCMD_W_AL_LIM_LS = 0x6100,     // write alert limits, low set
    eCMD_NO_SLEEP = 0x303E,
} SHT31_Command;

typedef enum
{
    REPEATAB_HIGH,   // high repeatability
    REPEATAB_MEDIUM, // medium repeatability
    REPEATAB_LOW,    // low repeatability
} SHT31_Repeatability;
// Measurement Mode
typedef enum
{
    MODE_CLKSTRETCH, // clock stretching
    MODE_POLLING,    // polling
} SHT31_ClockMode;
typedef enum
{
    FREQUENCY_HZ5,  // 0.5 measurements per seconds
    FREQUENCY_1HZ,  // 1.0 measurements per seconds
    FREQUENCY_2HZ,  // 2.0 measurements per seconds
    FREQUENCY_4HZ,  // 4.0 measurements per seconds
    FREQUENCY_10HZ, // 10.0 measurements per seconds
} SHT31_Freq;

typedef enum
{
    SHT31_OK = 0x00U,
    SHT31_FAILED,
} SHT31_Status;

typedef struct{
    uint8_t raw_data_buffer[6];
    float Temperature;
    float Humidity;
    uint8_t temp_crc;
    uint8_t humid_crc;
    uint8_t crc_flag : 1;
} SHT31_TypeDef_t;

/* Function prototype */
SHT31_Status SHT31_SendCommand(SHT31_Command command);
SHT31_Status SHT31_ReadData(SHT31_TypeDef_t *sht31_data_struct);
void SHT31_calculateTemp(SHT31_TypeDef_t* sht31_data_struct);
void SHT31_calculateHumid(SHT31_TypeDef_t* sht31_data_struct);
void SHT31_CRCCheck(SHT31_TypeDef_t *sht31_data_struct);

#endif /* !__MY_SHT31_H */

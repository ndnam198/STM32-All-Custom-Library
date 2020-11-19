#include "mySHT3x.h"

/**
 * @brief Send 2 byte command to sensor
 * 
 * @param hi2c 
 * @param command 
 * @return SHT31_StatusTypeDef 
 */
SHT31_StatusTypeDef SHT3x_SendCommand(uint8_t *command)
{
    uint8_t msb_cmd, lsb_cmd, i2c_status;
    msb_cmd = *command;
    lsb_cmd = *(command + 1);
    i2c_status = I2C_Write(SHT3x_I2C_ADDRESS_A, msb_cmd, lsb_cmd);
    if (i2c_status == 0)
    {
        return SHT31_OK;
    }
    else
    {
        return SHT31_FAILED;
    }
}

/**
 * @brief Read 6 byte data from sensor
 * 
 * @param hi2c 
 * @param data_buffer 
 * @return SHT31_StatusTypeDef 
 */
SHT31_StatusTypeDef SHT3x_ReadData(uint8_t *data_buffer)
{
    uint8_t i2c_status;
    memset(data_buffer, 0, 6);
    i2c_status = I2C_ReadMulti(SHT3x_I2C_ADDRESS_A, 0x00, SHT3x_DATA_LENGTH, data_buffer);
    if (i2c_status == 0)
    {
        return SHT31_OK;
    }
    else
    {
        return SHT31_FAILED;
    }
}

/**
 * @brief Calculate Temperature to Celcius based on raw data
 * 
 * @param raw_data_buffer 
 * @param temp_buffer 
 */
void SHT3x_calculateTemp(uint8_t *raw_data_buffer, float *temp_buffer)
{
    uint16_t raw_temp = (uint16_t)((*raw_data_buffer << 8) | *(raw_data_buffer + 1));
    *temp_buffer = (-45 + 175 * ((float)raw_temp / 65535));
}

/**
 * @brief Calculate Humidity to Celcius based on raw data
 * 
 * @param raw_data_buffer 
 * @param humid_buffer 
 */
void SHT3x_calculateHumid(uint8_t *raw_data_buffer, float *humid_buffer)
{
    uint16_t raw_humid = (uint16_t)((*(raw_data_buffer + 3) << 8) | *((raw_data_buffer + 4) + 1));
    *humid_buffer = (100 * ((float)raw_humid / 65535));
}

/**
 * @brief Check CRC validity
 * 
 * @param raw_data_buffer 
 * @return SHT31_StatusTypeDef 
 */
SHT31_StatusTypeDef SHT3x_CRCCheck(uint8_t *raw_data_buffer)
{
    uint8_t bit;
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < 2; byteCtr++)
    {
        crc ^= (raw_data_buffer[byteCtr]);
        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }

    if (crc != *(raw_data_buffer + 2))
        return SHT31_FAILED;
    else
        return SHT31_OK;
}

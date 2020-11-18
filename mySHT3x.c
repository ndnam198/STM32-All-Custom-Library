#include "mySHT3x.h"

SHT31_StatusTypeDef SHT3x_sendCommand(I2C_HandleTypeDef *hi2c, uint8_t *command)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(hi2c, SHT3x_I2C_ADDRESS, (uint8_t *)command, SHT3x_COMMAND_LENGTH, SHT3x_COMMAND_TIMEOUT);
    if (status == HAL_OK)
    {
        return SHT31_OK;
    }
    if (status == HAL_BUSY)
    {
        return SHT31_BUSY;
    }
    else
    {
        return SHT31_FAILED;
    }
}

SHT31_StatusTypeDef SHT3x_readData(I2C_HandleTypeDef *hi2c, uint8_t *data_buffer)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Receive(hi2c, SHT3x_I2C_ADDRESS, data_buffer, SHT3x_DATA_LENGTH, SHT3x_COMMAND_TIMEOUT);
    if (status == HAL_OK)
    {
        return SHT31_OK;
    }
    if (status == HAL_BUSY)
    {
        return SHT31_BUSY;
    }
    else
    {
        return SHT31_FAILED;
    }
}

void SHT3x_calculateTemp(uint16_t temp_data, float *temp_buffer)
{
    *temp_buffer = (-45 + 175 * ((float)temp_data / 65535));
}

void SHT3x_calculateHumid(uint16_t humid_data, float *humid_buffer)
{
    *humid_buffer = (100 * ((float)humid_data / 65535));
}

SHT31_StatusTypeDef SHT3x_CRCCheck(uint8_t *data_frame)
{
    uint8_t bit;
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < 2; byteCtr++)
    {
        crc ^= (data_frame[byteCtr]);
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

    if (crc != *(data_frame + 2))
        return SHT31_FAILED;
    else
        return SHT31_OK;
}
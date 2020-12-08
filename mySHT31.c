#include "mySHT31.h"

/**
 * @brief Send two byte command to sensor
 * 
 * @param command 
 * @return SHT31_Status 
 */
SHT31_Status SHT31_SendCommand(SHT31_Command command)
{
    uint8_t msb_cmd, lsb_cmd, i2c_status;
    msb_cmd = (uint8_t)(command >> 8);
    lsb_cmd = (uint8_t)(command & 0xFF);
    i2c_status = I2C_Write(SHT31_I2C_ADDRESS_A, msb_cmd, lsb_cmd);
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
 * @param sht31_data_struct 
 * @return SHT31_Status 
 */
SHT31_Status SHT31_ReadData(SHT31_TypeDef_t *sht31_data_struct)
{
    uint8_t i2c_status;
    i2c_status = I2C_ReadMulti(SHT31_I2C_ADDRESS_A, 0x00, SHT31_DATA_LENGTH, sht31_data_struct->raw_data_buffer);
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
 * @param sht31_data_struct 
 */
void SHT31_calculateTemp(SHT31_TypeDef_t *sht31_data_struct)
{
    uint16_t raw_temp = (uint16_t)(sht31_data_struct->raw_data_buffer[0] << 8 | sht31_data_struct->raw_data_buffer[1]);
    sht31_data_struct->Temperature = (-45 + 175 * (raw_temp / 65535.0));
}

/**
 * @brief Calculate Humidity to Celcius based on raw data
 * 
 * @param sht31_data_struct 
 */
void SHT31_calculateHumid(SHT31_TypeDef_t *sht31_data_struct)
{
    uint16_t raw_humid = (uint16_t)(sht31_data_struct->raw_data_buffer[3] << 8 | sht31_data_struct->raw_data_buffer[4]);
    sht31_data_struct->Humidity = (100 * (raw_humid / 65535.0));
}

/**
 * @brief Check CRC validity
 * 
 * @param sht31_data_struct 
 */
void SHT31_CRCCheck(SHT31_TypeDef_t *sht31_data_struct)
{
    uint8_t bit;
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
    uint8_t *raw_data_buffer = sht31_data_struct->raw_data_buffer;

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
    {
        sht31_data_struct->crc_flag = 0;
    }
    else
    {
        sht31_data_struct->crc_flag = 1;
    }
    return;
}

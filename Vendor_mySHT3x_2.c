#include "Vendor_SHT3x_2.h"

static uint8_t _i2cAddress; // I2C Address
//-- Static function prototypes -----------------------------------------------
static e_Error SHT3x_WriteAlertLimitData(float humidity, float temperature);
static e_Error SHT3x_ReadAlertLimitData(float *humidity, float *temperature);
static e_Error SHT3x_StartWriteAccess(void);
static e_Error SHT3x_StartReadAccess(void);
static void SHT3x_StopAccess(void);
static e_Error SHT3x_WriteCommand(e_SHT3x_Command command);
static e_Error SHT3x_Read2BytesAndCrc(uint16_t *data, e_ACK finaleAckNack,
                                      uint8_t timeout);
static e_Error SHT3x_Write2BytesAndCrc(uint16_t data);
static uint8_t SHT3x_CalcCrc(uint8_t data[], uint8_t nbrOfBytes);
static e_Error SHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum);
static float SHT3x_CalcTemperature(uint16_t rawValue);
static float SHT3x_CalcHumidity(uint16_t rawValue);
static uint16_t SHT3x_CalcRawTemperature(float temperature);
static uint16_t SHT3x_CalcRawHumidity(float humidity);
//-----------------------------------------------------------------------------
void SHT3x_Init(uint8_t i2cAddress) /* -- adapt the init for your uC -- */
{
    // init I/O-pins
    // RCC->APB2ENR |= 0x00000008; // I/O port B clock enabled
    // // Alert on port B, bit 10
    // GPIOB->CRH &= 0xFFFFF0FF; // set floating input for Alert-Pin
    // GPIOB->CRH |= 0x00000400; //
    // // Reset on port B, bit 12
    // GPIOB->CRH &= 0xFFF0FFFF; // set push-pull output for Reset pin
    // GPIOB->CRH |= 0x00010000; //
    // RESET_LOW();
    I2c_Init(); // init I2C
    SHT3x_SetI2cAdr(i2cAddress);
    // release reset
    // RESET_HIGH();
}
//-----------------------------------------------------------------------------
void SHT3x_SetI2cAdr(uint8_t i2cAddress)
{
    _i2cAddress = i2cAddress;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_ReadSerialNumber(uint32_t *serialNumber)
{
    e_Error error; // error code
    uint16_t serialNumWords[2];
    error = SHT3x_StartWriteAccess();
    // write "read serial number" command
    error |= SHT3x_WriteCommand(CMD_READ_SERIALNBR);
    // if no error, start read access
    if (error == NO_ERROR)
        error = SHT3x_StartReadAccess();
    // if no error, read first serial number word
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&serialNumWords[0], ACK,
                                       100);
    // if no error, read second serial number word
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&serialNumWords[1], NACK, 0);
    SHT3x_StopAccess();
    // if no error, calc serial number as 32-bit integer
    if (error == NO_ERROR)
    {
        *serialNumber = (serialNumWords[0] << 16) | serialNumWords[1];
    }
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_ReadStatus(uint16_t *status)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // if no error, write "read status" command
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_READ_STATUS);
    // if no error, start read access
    if (error == NO_ERROR)
        error = SHT3x_StartReadAccess();
    // if no error, read status
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(status, NACK, 0);
    SHT3x_StopAccess();
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_ClearAllAlertFlags(void)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // if no error, write clear status register command
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_CLEAR_STATUS);
    SHT3x_StopAccess();
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_GetTempAndHumi(float *temperature, float *humidity,
                             e_SHT3x_Repeatability repeatability, e_SHT3x_ClockMode mode,
                             uint8_t timeout)
{
    e_Error error;
    switch (mode)
    {
    case MODE_CLKSTRETCH: // get temperature with clock stretching mode
        error = SHT3x_GetTempAndHumiClkStretch(temperature, humidity,
                                               repeatability, timeout);
        break;
    case MODE_POLLING: // get temperature with polling mode
        error = SHT3x_GetTempAndHumiPolling(temperature, humidity,
                                            repeatability, timeout);
        break;
    default:
        error = PARM_ERROR;
        break;
    }
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_GetTempAndHumiClkStretch(float *temperature, float *humidity,
                                       e_SHT3x_Repeatability repeatability,
                                       uint8_t timeout)
{
    e_Error error;         // error code
    uint16_t rawValueTemp; // temperature raw value from sensor
    uint16_t rawValueHumi; // humidity raw value from sensor
    error = SHT3x_StartWriteAccess();
    // if no error ...
    if (error == NO_ERROR)
    {
        // start measurement in clock stretching mode
        // use depending on the required repeatability, the corresponding command
        switch (repeatability)
        {
        case REPEATAB_LOW:
            error = SHT3x_WriteCommand(CMD_MEAS_CLOCKSTR_L);
            break;
        case REPEATAB_MEDIUM:
            error = SHT3x_WriteCommand(CMD_MEAS_CLOCKSTR_M);
            break;
        case REPEATAB_HIGH:
            error = SHT3x_WriteCommand(CMD_MEAS_CLOCKSTR_H);
            break;
        default:
            error = PARM_ERROR;
            break;
        }
    }
    // if no error, start read access
    if (error == NO_ERROR)
        error = SHT3x_StartReadAccess();
    // if no error, read temperature raw values
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&rawValueTemp, ACK, timeout);
    // if no error, read humidity raw values
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
    SHT3x_StopAccess();
    // if no error, calculate temperature in °C and humidity in %RH
    if (error == NO_ERROR)
    {
        *temperature = SHT3x_CalcTemperature(rawValueTemp);
        *humidity = SHT3x_CalcHumidity(rawValueHumi);
    }
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_GetTempAndHumiPolling(float *temperature, float *humidity,
                                    e_SHT3x_Repeatability repeatability,
                                    uint8_t timeout)
{
    e_Error error;         // error code
    uint16_t rawValueTemp; // temperature raw value from sensor
    uint16_t rawValueHumi; // humidity raw value from sensor
    error = SHT3x_StartWriteAccess();
    // if no error ...
    if (error == NO_ERROR)
    {
        // start measurement in polling mode
        // use depending on the required repeatability, the corresponding command
        switch (repeatability)
        {
        case REPEATAB_LOW:
            error = SHT3x_WriteCommand(CMD_MEAS_POLLING_L);
            break;
        case REPEATAB_MEDIUM:
            error = SHT3x_WriteCommand(CMD_MEAS_POLLING_M);
            break;
        case REPEATAB_HIGH:
            error = SHT3x_WriteCommand(CMD_MEAS_POLLING_H);
            break;
        default:
            error = PARM_ERROR;
            break;
        }
    }
    // if no error, wait until measurement ready
    if (error == NO_ERROR)
    {
        // poll every 1ms for measurement ready until timeout
        while (timeout--)
        {
            // check if the measurement has finished
            error = SHT3x_StartReadAccess();
            // if measurement has finished -> exit loop
            if (error == NO_ERROR)
                break;
            // delay 1ms
            DelayMicroSeconds(1000);
        }
        // if no error, read temperature and humidity raw values
        if (error == NO_ERROR)
        {
            error |= SHT3x_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
            error |= SHT3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
        }
        SHT3x_StopAccess();
        // if no error, calculate temperature in °C and humidity in %RH
        if (error == NO_ERROR)
        {
            *temperature = SHT3x_CalcTemperature(rawValueTemp);
            *humidity = SHT3x_CalcHumidity(rawValueHumi);
        }
        return error;
    }
}
//-----------------------------------------------------------------------------
e_Error SHT3x_StartPeriodicMeasurment(e_SHT3x_Repeatability repeatability,
                                      e_SHT3x_Freq frequency)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // if no error, start periodic measurement
    if (error == NO_ERROR)
    {
        // use depending on the required repeatability and frequency,
        // the corresponding command
        switch (repeatability)
        {
        case REPEATAB_LOW: // low repeatability
            switch (frequency)
            {
            case FREQUENCY_HZ5: // low repeatability, 0.5 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_05_L);
                break;
            case FREQUENCY_1HZ: // low repeatability, 1.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_1_L);
                break;
            case FREQUENCY_2HZ: // low repeatability, 2.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_2_L);
                break;
            case FREQUENCY_4HZ: // low repeatability, 4.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_4_L);
                break;
            case FREQUENCY_10HZ: // low repeatability, 10.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_10_L);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;
        case REPEATAB_MEDIUM: // medium repeatability
            switch (frequency)
            {
            case FREQUENCY_HZ5: // medium repeatability, 0.5 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_05_M);
                break;
            case FREQUENCY_1HZ: // medium repeatability, 1.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_1_M);
                break;
            case FREQUENCY_2HZ: // medium repeatability, 2.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_2_M);
                break;
            case FREQUENCY_4HZ: // medium repeatability, 4.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_4_M);
                break;
            case FREQUENCY_10HZ: // medium repeatability, 10.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_10_M);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;
        case REPEATAB_HIGH: // high repeatability
            switch (frequency)
            {
            case FREQUENCY_HZ5: // high repeatability, 0.5 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_05_H);
                break;
            case FREQUENCY_1HZ: // high repeatability, 1.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_1_H);
                break;
            case FREQUENCY_2HZ: // high repeatability, 2.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_2_H);
                break;
            case FREQUENCY_4HZ: // high repeatability, 4.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_4_H);
                break;
            case FREQUENCY_10HZ: // high repeatability, 10.0 Hz
                error |= SHT3x_WriteCommand(CMD_MEAS_PERI_10_H);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;
        default:
            error |= PARM_ERROR;
            break;
        }
    }
    SHT3x_StopAccess();
    return error;
}

//-----------------------------------------------------------------------------
e_Error SHT3x_ReadMeasurementBuffer(float *temperature, float *humidity)
{
    e_Error error;         // error code
    uint16_t rawValueTemp; // temperature raw value from sensor
    uint16_t rawValueHumi; // humidity raw value from sensor
    error = SHT3x_StartWriteAccess();
    // if no error, read measurements
    if (error == NO_ERROR){}
        error = SHT3x_WriteCommand(CMD_FETCH_DATA);
    if (error == NO_ERROR)
        error = SHT3x_StartReadAccess();
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
    if (error == NO_ERROR)
        error = SHT3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
    // if no error, calculate temperature in °C and humidity in %RH
    if (error == NO_ERROR)
    {
        *temperature = SHT3x_CalcTemperature(rawValueTemp);
        *humidity = SHT3x_CalcHumidity(rawValueHumi);
    }
    SHT3x_StopAccess();
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_EnableHeater(void)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // if no error, write heater enable command
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_HEATER_ENABLE);
    SHT3x_StopAccess();
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_DisableHeater(void)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // if no error, write heater disable command
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_HEATER_DISABLE);
    SHT3x_StopAccess();
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_SetAlertLimits(float humidityHighSet, float temperatureHighSet,
                             float humidityHighClear, float temperatureHighClear,
                             float humidityLowClear, float temperatureLowClear,
                             float humidityLowSet, float temperatureLowSet)
{
    e_Error error; // error code
    // write humidity & temperature alter limits, high set
    error = SHT3x_StartWriteAccess();
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_W_AL_LIM_HS);
    if (error == NO_ERROR)
        error = SHT3x_WriteAlertLimitData(humidityHighSet,
                                          temperatureHighSet);
    SHT3x_StopAccess();
    if (error == NO_ERROR)
    {
        // write humidity & temperature alter limits, high clear
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_W_AL_LIM_HC);
        if (error == NO_ERROR)
            error = SHT3x_WriteAlertLimitData(humidityHighClear,
                                              temperatureHighClear);
        SHT3x_StopAccess();
    }
    if (error == NO_ERROR)
    {
        // write humidity & temperature alter limits, low clear
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_R_AL_LIM_LC);
        if (error == NO_ERROR)
            error = SHT3x_WriteAlertLimitData(humidityLowClear,
                                              temperatureLowClear);
        SHT3x_StopAccess();
    }
    if (error == NO_ERROR)
    {
        // write humidity & temperature alter limits, low set
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_W_AL_LIM_LS);
        if (error == NO_ERROR)
            error = SHT3x_WriteAlertLimitData(humidityLowSet,
                                              temperatureLowSet);
        SHT3x_StopAccess();
    }
    return error;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_GetAlertLimits(float *humidityHighSet, float *temperatureHighSet,
                             float *humidityHighClear, float *temperatureHighClear,
                             float *humidityLowClear, float *temperatureLowClear,
                             float *humidityLowSet, float *temperatureLowSet)
{
    e_Error error; // error code
    // read humidity & temperature alter limits, high set
    error = SHT3x_StartWriteAccess();
    if (error == NO_ERROR)
        error = SHT3x_WriteCommand(CMD_R_AL_LIM_HS);
    if (error == NO_ERROR)
        error = SHT3x_StartReadAccess();
    if (error == NO_ERROR)
        error = SHT3x_ReadAlertLimitData(humidityHighSet,
                                         temperatureHighSet);
    SHT3x_StopAccess();
    if (error == NO_ERROR)
    {
        // read humidity & temperature alter limits, high clear
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_R_AL_LIM_HC);
        if (error == NO_ERROR)
            error = SHT3x_StartReadAccess();
        if (error == NO_ERROR)
            error = SHT3x_ReadAlertLimitData(humidityHighClear,
                                             temperatureHighClear);
        SHT3x_StopAccess();
    }
    if (error == NO_ERROR)
    {
        // read humidity & temperature alter limits, low clear
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_R_AL_LIM_LC);
        if (error == NO_ERROR)
            error = SHT3x_StartReadAccess();
        if (error == NO_ERROR)
            error = SHT3x_ReadAlertLimitData(humidityLowClear,
                                             temperatureLowClear);
        SHT3x_StopAccess();
    }
    if (error == NO_ERROR)
    {
        // read humidity & temperature alter limits, low set
        error = SHT3x_StartWriteAccess();
        if (error == NO_ERROR)
            error = SHT3x_WriteCommand(CMD_R_AL_LIM_LS);
        if (error == NO_ERROR)
            error = SHT3x_StartReadAccess();
        if (error == NO_ERROR)
            error = SHT3x_ReadAlertLimitData(humidityLowSet,
                                             temperatureLowSet);
        SHT3x_StopAccess();
    }
    return error;
}
//-----------------------------------------------------------------------------
e_bool SHT3x_ReadAlert(void)
{
    // read alert pin
    // return (ALERT_READ != 0) ? TRUE : FALSE;
    return TRUE;
}
//-----------------------------------------------------------------------------
e_Error SHT3x_SoftReset(void)
{
    e_Error error; // error code
    error = SHT3x_StartWriteAccess();
    // write reset command
    error |= SHT3x_WriteCommand(CMD_SOFT_RESET);
    SHT3x_StopAccess();
    // if no error, wait 50 ms after reset
    if (error == NO_ERROR)
        DelayMicroSeconds(50000);
    return error;
}
//-----------------------------------------------------------------------------
void SHT3x_HardReset(void)
{
    // // set reset low
    // RESET_LOW();
    // // wait 100 ms
    // DelayMicroSeconds(100000);
    // // release reset
    // RESET_HIGH();
    // // wait 50 ms after reset
    // DelayMicroSeconds(50000);
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_WriteAlertLimitData(float humidity, float temperature)
{
    e_Error error; // error code
    uint16_t rawHumidity;
    uint16_t rawTemperature;
    if ((humidity < 0.0f) || (humidity > 100.0f) || (temperature < -45.0f) || (temperature > 130.0f))
    {
        error = PARM_ERROR;
    }
    else
    {
        rawHumidity = SHT3x_CalcRawHumidity(humidity);
        rawTemperature = SHT3x_CalcRawTemperature(temperature);
        error = SHT3x_Write2BytesAndCrc((rawHumidity & 0xFE00) | ((rawTemperature >> 7) & 0x001FF));
    }
    return error;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_ReadAlertLimitData(float *humidity, float *temperature)
{
    e_Error error; // error code
    uint16_t data;
    error = SHT3x_Read2BytesAndCrc(&data, NACK, 0);
    if (error == NO_ERROR)
    {
        *humidity = SHT3x_CalcHumidity(data & 0xFE00);
        *temperature = SHT3x_CalcTemperature(data << 7);
    }
    return error;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_StartWriteAccess(void)
{
    e_Error error; // error code
    // write a start condition
    I2c_StartCondition();
    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1);
    return error;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_StartReadAccess(void)
{
    e_Error error; // error code
    // write a start condition
    I2c_StartCondition();
    // write the sensor I2C address with the read flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x01);
    return error;
}
//-----------------------------------------------------------------------------
static void SHT3x_StopAccess(void)
{
    // write a stop condition
    I2c_StopCondition();
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_WriteCommand(e_SHT3x_Command command)
{
    e_Error error; // error code
    // write the upper 8 bits of the command to the sensor
    error = I2c_WriteByte(command >> 8);
    // write the lower 8 bits of the command to the sensor
    error |= I2c_WriteByte(command & 0xFF);
    return error;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_Read2BytesAndCrc(uint16_t *data, e_ACK finaleAckNack,
                                      uint8_t timeout)
{
    e_Error error;    // error code
    uint8_t bytes[2]; // read data array
    uint8_t checksum; // checksum byte
    // read two data bytes and one checksum byte
    error = I2c_ReadByte(&bytes[0], ACK, timeout);
    if (error == NO_ERROR)
        error = I2c_ReadByte(&bytes[1], ACK, 0);
    if (error == NO_ERROR)
        error = I2c_ReadByte(&checksum, finaleAckNack, 0);
    // verify checksum
    if (error == NO_ERROR)
        error = SHT3x_CheckCrc(bytes, 2, checksum);
    // combine the two bytes to a 16-bit value
    *data = (bytes[0] << 8) | bytes[1];
    return error;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_Write2BytesAndCrc(uint16_t data)
{
    e_Error error;    // error code
    uint8_t bytes[2]; // read data array
    uint8_t checksum; // checksum byte
    bytes[0] = data >> 8;
    bytes[1] = data & 0xFF;
    checksum = SHT3x_CalcCrc(bytes, 2);
    // write two data bytes and one checksum byte
    error = I2c_WriteByte(bytes[0]); // write data MSB
    if (error == NO_ERROR)
        error = I2c_WriteByte(bytes[1]); // write data LSB
    if (error == NO_ERROR)
        error = I2c_WriteByte(checksum); // write checksum
    return error;
}
//-----------------------------------------------------------------------------
static uint8_t SHT3x_CalcCrc(uint8_t data[], uint8_t nbrOfBytes)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
    {
        crc ^= (data[byteCtr]);
        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}
//-----------------------------------------------------------------------------
static e_Error SHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
    uint8_t crc; // calculated checksum
    // calculates 8-Bit checksum
    crc = SHT3x_CalcCrc(data, nbrOfBytes);
    // verify checksum
    if (crc != checksum)
        return CHECKSUM_ERROR;
    else
        return NO_ERROR;
}
//-----------------------------------------------------------------------------
static float SHT3x_CalcTemperature(uint16_t rawValue)
{
    // calculate temperature [°C]
    // T = -45 + 175 * rawValue / (2^16-1)
    return 175.0f * (float)rawValue / 65535.0f - 45.0f;
}
//-----------------------------------------------------------------------------
static float SHT3x_CalcHumidity(uint16_t rawValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / (2^16-1) * 100
    return 100.0f * (float)rawValue / 65535.0f;
}
//-----------------------------------------------------------------------------
static uint16_t SHT3x_CalcRawTemperature(float temperature)
{
    // calculate raw temperature [ticks]
    // rawT = (temperature + 45) / 175 * (2^16-1)
    return (temperature + 45.0f) / 175.0f * 65535.0f;
}
//-----------------------------------------------------------------------------
static uint16_t SHT3x_CalcRawHumidity(float humidity)
{
    // calculate raw relative humidity [ticks]
    // rawRH = humidity / 100 * (2^16-1)
    return humidity / 100.0f * 65535.0f;
}

#include "myI2C.h"

static e_Error I2c_WaitWhileClockStreching(uint8_t timeout);

void DelayMicroSeconds(uint32_t nbrOfUs) /* -- adapt this delay for your uC -- */
{
    uint32_t i;
    for (i = 0; i < nbrOfUs; i++)
    {
        __NOP(); // nop's may be added or removed for timing adjustment
        __NOP();
        __NOP();
        __NOP();
    }
}
//-----------------------------------------------------------------------------
void I2c_Init(void) /* -- adapt the init for your uC -- */
{
    RCC->APB1ENR |= 0x00200000; // I/O port B clock enabled
    SDA_OPEN();                 // I2C-bus idle mode SDA released
    SCL_OPEN();                 // I2C-bus idle mode SCL released
    GPIOB->CRL &= 0x00FFFFFF; // set open-drain output for SDA and SCL
    GPIOB->CRL |= 0x55000000; //
}
//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
    SDA_OPEN();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(10); // hold time start condition (t_HD;STA)
    SCL_LOW();
    DelayMicroSeconds(10);
}
//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
    SCL_LOW();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(10); // set-up time stop condition (t_SU;STO)
    SDA_OPEN();
    DelayMicroSeconds(10);
}
//-----------------------------------------------------------------------------
e_Error I2c_WriteByte(uint8_t txByte)
{
    e_Error error = NO_ERROR;
    uint8_t mask;
    for (mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
    {
        /* Transfer MSB first */
        if ((mask & txByte) == 0)
            SDA_LOW(); // masking txByte, write bit to SDA-Line
        else
            SDA_OPEN();
        DelayMicroSeconds(1); // data set-up time (t_SU;DAT)
        SCL_OPEN();           // generate clock pulse on SCL
        DelayMicroSeconds(5); // SCL high time (t_HIGH)
        SCL_LOW();
        DelayMicroSeconds(1); // data hold time(t_HD;DAT)
    }
    SDA_OPEN();           // release SDA-line
    SCL_OPEN();           // clk #9 for ack
    DelayMicroSeconds(1); // data set-up time (t_SU;DAT)
    if (SDA_READ)
        error = ACK_ERROR; // check ack from i2c slave
    SCL_LOW();
    DelayMicroSeconds(20); // wait to see byte package on scope
    return error;          // return error code
}
//-----------------------------------------------------------------------------
e_Error I2c_ReadByte(uint8_t *rxByte, e_ACK ack, uint8_t timeout)
{
    e_Error error = NO_ERROR;
    uint8_t mask;
    *rxByte = 0x00;
    SDA_OPEN();                             // release SDA-line
    for (mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
    {
        SCL_OPEN();                                   // start clock on SCL-line
        DelayMicroSeconds(1);                         // clock set-up time (t_SU;CLK)
        error = I2c_WaitWhileClockStreching(timeout); // wait while clock streching
        DelayMicroSeconds(3);                         // SCL high time (t_HIGH)
        if (SDA_READ)
            *rxByte |= mask; // read bit
        SCL_LOW();
        DelayMicroSeconds(1); // data hold time(t_HD;DAT)
    }
    if (ack == ACK)
        SDA_LOW(); // send acknowledge if necessary
    else
        SDA_OPEN();
    DelayMicroSeconds(1); // data set-up time (t_SU;DAT)
    SCL_OPEN();           // clk #9 for ack
    DelayMicroSeconds(5); // SCL high time (t_HIGH)
    SCL_LOW();
    SDA_OPEN();            // release SDA-line
    DelayMicroSeconds(20); // wait to see byte package on scope
    return error;          // return with no error
}
//-----------------------------------------------------------------------------
e_Error I2c_GeneralCallReset(void)
{
    e_Error error;
    I2c_StartCondition();
    error = I2c_WriteByte(0x00);
    if (error == NO_ERROR)
        error = I2c_WriteByte(0x06);
    return error;
}
//-----------------------------------------------------------------------------
static e_Error I2c_WaitWhileClockStreching(uint8_t timeout)
{
    e_Error error = NO_ERROR;
    while (SCL_READ == 0)
    {
        if (timeout-- == 0)
            return TIMEOUT_ERROR;
        DelayMicroSeconds(1000);
    }
    return error;
}

#include "myI2C.h"
#include "retarget.h"

static void delay_ms(uint32_t ms);

void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Clock Port B Enable - IOPBEN bit = 1 */
    RCC->APB2ENR |= (1 << 3);

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    I2C_SCL_SET;
    I2C_SDA_SET;
}

void I2C_Start(void)
{
    I2C_SDA_OUT();
    I2C_SCL_SET;
    I2C_SDA_SET;
    delay_ms(4);
    I2C_SDA_RESET;
    delay_ms(4);
    I2C_SCL_RESET;
}

void I2C_Stop(void)
{
    I2C_SDA_OUT();
    I2C_SCL_RESET;
    I2C_SDA_RESET;
    delay_ms(4);
    I2C_SDA_SET;
    I2C_SCL_SET;
    delay_ms(4);
}

void I2C_Send_Byte(uint8_t txd)
{
    int i = 0;
    I2C_SDA_OUT();
    I2C_SCL_RESET;
    for (i = 0; i < 8; i++)
    {
        uint8_t send_bit = (txd & 0x80) >> 7;
        if (send_bit == 1)
        {
            I2C_SDA_SET;
        }
        else
        {
            I2C_SDA_RESET;
        }
        txd <<= 1;
        delay_ms(2);
        I2C_SCL_SET;
        delay_ms(2);
        I2C_SCL_RESET;
    }
}

void I2C_SendACK(void)
{
    I2C_SCL_RESET;
    I2C_SDA_OUT();
    I2C_SDA_RESET;
    delay_ms(2);
    I2C_SCL_SET;
    delay_ms(2);
    I2C_SCL_RESET;
}

void I2C_SendNACK(void)
{
    I2C_SCL_RESET;
    I2C_SDA_OUT();
    I2C_SDA_SET;
    delay_ms(2);
    I2C_SCL_SET;
    delay_ms(2);
    I2C_SCL_RESET;
}

uint8_t I2C_Read_Byte(unsigned char ack)
{
    int i = 0;
    uint8_t rec = 0;
    I2C_SDA_IN();
    for (i = 0; i < 8; i++)
    {
        I2C_SCL_RESET;
        delay_ms(2);
        I2C_SCL_SET;
        delay_ms(2);
        rec <<= 1;
        if (I2C_SDA_READ)
        {
            rec++;
        }
    }
    if (!ack)
    {
        I2C_SendACK();
    }
    else
    {
        I2C_SendNACK();
    }
    return rec;
}

uint8_t I2C_Wait_Ack(void)
{
    uint8_t time = 0;
    I2C_SDA_IN();
    I2C_SDA_SET;
    delay_ms(1);
    I2C_SCL_SET;
    delay_ms(1);
    while (I2C_SDA_READ)
    {
        time++;
        if (time > 10000)
        {
            printf("I2C Bus Timout\r\n");
            I2C_Stop();
            return 0; /* Return 0 to avoid application halt */
        }
    }
    I2C_SCL_RESET;
    return 0;
}

void I2C_Cmd_Write(uint8_t add, uint8_t reg, uint8_t data)
{
    I2C_Start();

    I2C_Send_Byte(add | 0);
    while (I2C_Wait_Ack())
        ;
    I2C_Send_Byte(reg);
    while (I2C_Wait_Ack())
        ;
    I2C_Send_Byte(data);
    while (I2C_Wait_Ack())
        ;

    I2C_Stop();
    delay_ms(2);
}

uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C_Start();

    I2C_Send_Byte(addr | 0);
    if (I2C_Wait_Ack())
    {
        I2C_Stop();
        return 1;
    }
    I2C_Send_Byte(reg);
    if (I2C_Wait_Ack())
    {
        I2C_Stop();
        return 1;
    }
    I2C_Send_Byte(data);
    if (I2C_Wait_Ack())
    {
        I2C_Stop();
        return 1;
    }

    I2C_Stop();
    delay_ms(2);
    return 0;
}

uint8_t Read_I2C(uint8_t addr, uint8_t reg)
{
    I2C_Start();
    I2C_Send_Byte(addr | 0);
    while (I2C_Wait_Ack())
        ;
    I2C_Send_Byte(reg);
    while (I2C_Wait_Ack())
        ;

    I2C_Start();
    I2C_Send_Byte(addr | 1);
    while (I2C_Wait_Ack())
        ;
    reg = I2C_Read_Byte(1);
    I2C_Stop();
    delay_ms(2);
    return reg;
}

uint8_t I2C_ReadMulti(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    I2C_Start();
    I2C_Send_Byte(addr | 0);
    while (I2C_Wait_Ack())
        ;
    I2C_Send_Byte(reg);
    while (I2C_Wait_Ack())
        ;

    I2C_Start();
    I2C_Send_Byte(addr | 1);
    while (I2C_Wait_Ack())
        ;

    while (len)
    {
        if (len == 1)
            *buf = I2C_Read_Byte(1);
        else
            *buf = I2C_Read_Byte(0);
        len--;
        buf++;
    }

    I2C_Stop();
    return 0;
}

uint8_t I2C_WriteMulti(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i = 0;
    I2C_Start();
    I2C_Send_Byte(addr | 0);
    while (I2C_Wait_Ack())
        ;
    I2C_Send_Byte(reg);
    while (I2C_Wait_Ack())
        ;

    for (i = 0; i < len; i++)
    {
        I2C_Send_Byte(buf[i]);
        if (I2C_Wait_Ack())
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_Stop();
    return 0;
}

static void delay_ms(uint32_t ms)
{
    volatile uint32_t delay = 1000 * ms;
    while (delay--)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}

/********************************************************************************************************************/

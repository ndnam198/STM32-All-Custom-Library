#ifndef __MY_I2C_2_H
#define __MY_I2C_2_H

#include "main.h"

#define I2C_SDA_IN()              \
	{                             \
		GPIOB->CRL &= 0X0FFFFFFF; \
		GPIOB->CRL |= 8 << 28;    \
	} //PB7IN
#define I2C_SDA_OUT()             \
	{                             \
		GPIOB->CRL &= 0X0FFFFFFF; \
		GPIOB->CRL |= 3 << 28;    \
	} //PB7OUT

#define I2C_SCL PBout(6) //SCL
#define I2C_SDA PBout(7) //SDA
#define READ_SDA PBin(7) //SDA IN

/********************************************************************************************************************/
/* I2C Pin Port */
#define I2C_SCL_Port (GPIOB)
#define I2C_SDA_Port (GPIOB)

/* I2C1: SDA - PB7; SCL - PB6 */
#define I2C_SCL_Pos (6)
#define I2C_SDA_Pos (7)

#define I2C_SCL_Pin_Msk (1u << I2C_SCL_Pos) /* 0x00000040 */
#define I2C_SDA_Pin_Msk (1u << I2C_SDA_Pos) /* 0x00000080 */

#define I2C_SCL_SET (I2C_SCL_Port->BSRR = I2C_SCL_Pin_Msk)
#define I2C_SCL_RESET (I2C_SCL_Port->BRR = I2C_SCL_Pin_Msk)
#define I2C_SCL_READ (I2C_SCL_Port->IDR & I2C_SCL_Pin_Msk)

#define I2C_SDA_SET (I2C_SDA_Port->BSRR |= I2C_SDA_Pin_Msk)
#define I2C_SDA_RESET (I2C_SDA_Port->BRR |= I2C_SDA_Pin_Msk)
#define I2C_SDA_READ (I2C_SDA_Port->IDR & I2C_SDA_Pin_Msk)
//I2C

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Send_Byte(uint8_t txd);
uint8_t I2C_Read_Byte(unsigned char ack);
uint8_t I2C_Wait_Ack(void);
void I2C_SendACK(void);
void I2C_SendNACK(void);
///////////////////////////////
void I2C_Cmd_Write(uint8_t add, uint8_t reg, uint8_t data);
uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t Read_I2C(uint8_t addr, uint8_t reg);
uint8_t I2C_ReadMulti(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t I2C_WriteMulti(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif
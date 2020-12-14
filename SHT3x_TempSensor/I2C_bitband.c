#include "myI2C.h"

void I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	/* Clock Port B Enable - IOPBEN bit = 1 */
	RCC->APB2ENR |= (1 << 3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  
	I2C_SCL = 1;
	I2C_SDA = 1;
}

void I2C_Start(void)
{
	SDA_OUT();
	I2C_SCL = 1;
	I2C_SDA = 1;
	delay_us(4);
	I2C_SDA = 0;
	delay_us(4);
	I2C_SCL = 0;
}

void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SCL = 0;
	I2C_SDA = 0;
	delay_us(4);
	I2C_SDA = 1;
	I2C_SCL = 1;
	delay_us(4);
}

void I2C_Send_Byte(uint8_t txd)
{ //11111111&10000000
	int i = 0;
	SDA_OUT();
	I2C_SCL = 0;
	for (i = 0; i < 8; i++)
	{
		I2C_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2);
		I2C_SCL = 1;
		delay_us(2);
		I2C_SCL = 0;
	}
}

void I2C_Ack(void)
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 0;
	delay_us(2);
	I2C_SCL = 1;
	delay_us(2);
	I2C_SCL = 0;
}

void I2C_NAck(void)
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 1;
	delay_us(2);
	I2C_SCL = 1;
	delay_us(2);
	I2C_SCL = 0;
}

uint8_t I2C_Read_Byte(unsigned char ack)
{
	int i = 0;
	uint8_t rec = 0;
	SDA_IN();
	for (i = 0; i < 8; i++)
	{ //11111111
		I2C_SCL = 0;
		delay_us(2);
		I2C_SCL = 1;
		delay_us(2);
		rec <<= 1;
		if (READ_SDA)
			rec++;
	}
	if (!ack)
	{
		I2C_Ack();
	}
	else
	{
		I2C_NAck();
	}
	return rec;
}

uint8_t I2C_Wait_Ack(void)
{
	uint8_t time = 0;
	SDA_IN();
	I2C_SDA = 1;
	delay_us(1);
	I2C_SCL = 1;
	delay_us(1);
	while (READ_SDA)
	{
		time++;
		if (time > 250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL = 0;
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

//#include "myI2C.h"

//void I2C_Init(void){
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;			    //LED0-->PB.5
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //GPIOB.5
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);
//	GPIO_SetBits(GPIOB, GPIO_Pin_7);
//}

//void I2C_Start(void){
//	SDA_OUT();
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);
//	GPIO_SetBits(GPIOB, GPIO_Pin_7);
//	delay_us(4);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//	delay_us(4);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//}

//void I2C_Stop(void){
//	SDA_OUT();
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//	delay_us(4);
//	GPIO_SetBits(GPIOB, GPIO_Pin_7);
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);
//	delay_us(4);
//}

//void I2C_Send_Byte(uint8_t txd){//11111111&10000000
//	uint8_t buf = 0;
//	int i=0;
//	SDA_OUT();
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	for(i=0;i<8;i++){
//		buf=(txd&0x80)>>7;
//		if(buf == 0) GPIO_SetBits(GPIOB, GPIO_Pin_7);
//		else if(buf == 1) GPIO_SetBits(GPIOB, GPIO_Pin_7);
//		txd<<=1;
//		delay_us(2);
//		GPIO_SetBits(GPIOB, GPIO_Pin_6);
//		delay_us(2);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	}
//}

//void I2C_Ack(void){
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	SDA_OUT();
//	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//	delay_us(2);
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);
//	delay_us(2);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//}

//void I2C_NAck(void){
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	SDA_OUT();
//	GPIO_SetBits(GPIOB, GPIO_Pin_7);
//	delay_us(2);
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);
//	delay_us(2);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//}

//uint8_t I2C_Read_Byte(unsigned char ack){
//	int i=0;
//	uint8_t rec=0;
//	SDA_IN();
//	for(i=0;i<8;i++){//11111111
//		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//		delay_us(2);
//		GPIO_SetBits(GPIOB, GPIO_Pin_6);
//		delay_us(2);
//		rec<<=1;
//		if(READ_SDA) rec++;
//	}
//	if(!ack){
//		I2C_Ack();
//	}
//		else{
//		I2C_NAck();
//		}
//	return rec;
//}

//uint8_t I2C_Wait_Ack(void){
//	uint8_t time=0;
//	SDA_IN();
//	GPIO_SetBits(GPIOB, GPIO_Pin_7);delay_us(1);
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);delay_us(1);
//	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)){
//		time++;
//		if(time>250) {I2C_Stop(); return 1;}
//	}
//	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//	return 0;
//}

//void I2C_Cmd_Write(uint8_t add ,uint8_t reg,uint8_t data){
//	I2C_Start();
//
//	I2C_Send_Byte(add|0);
//	while(I2C_Wait_Ack());
//	I2C_Send_Byte(reg);
//	while(I2C_Wait_Ack());
//	I2C_Send_Byte(data);
//	while(I2C_Wait_Ack());
//
//	I2C_Stop();
//	delay_ms(2);
//}

//uint8_t I2C_Write(uint8_t addr,uint8_t reg,uint8_t data){
//	I2C_Start();
//
//	I2C_Send_Byte(addr|0);
//	if(I2C_Wait_Ack()){I2C_Stop(); return 1;}
//	I2C_Send_Byte(reg);
//	if(I2C_Wait_Ack()){I2C_Stop(); return 1;}
//	I2C_Send_Byte(data);
//	if(I2C_Wait_Ack()){I2C_Stop(); return 1;}
//
//	I2C_Stop();
//	delay_ms(2);
//	return 0;
//}

//uint8_t Read_I2C(uint8_t addr, uint8_t reg){
//	I2C_Start();
//	I2C_Send_Byte(addr|0);
//	while(I2C_Wait_Ack());
//	I2C_Send_Byte(reg);
//	while(I2C_Wait_Ack());
//
//	I2C_Start();
//	I2C_Send_Byte(addr|1);
//	while(I2C_Wait_Ack());
//	reg=I2C_Read_Byte(1);
//	I2C_Stop();
//	delay_ms(2);
//	return reg;
//}

//uint8_t I2C_ReadMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
//	I2C_Start();
//	I2C_Send_Byte(addr|0);
//	while(I2C_Wait_Ack());
//	I2C_Send_Byte(reg);
//	while(I2C_Wait_Ack());
//
//	I2C_Start();
//	I2C_Send_Byte(addr|1);
//	while(I2C_Wait_Ack());
//
//	while(len){
//		if(len==1) *buf= I2C_Read_Byte(1);
//		else *buf= I2C_Read_Byte(0);
//		len--;
//		buf++;
//	}
//
//	I2C_Stop();
//	return 0;
//}

//uint8_t I2C_WriteMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
//	uint8_t i=0;
//	I2C_Start();
//	I2C_Send_Byte(addr|0);
//	while(I2C_Wait_Ack());
//	I2C_Send_Byte(reg);
//	while(I2C_Wait_Ack());
//
//	for(i=0;i<len;i++){
//		I2C_Send_Byte(buf[i]);
//		if(I2C_Wait_Ack()){I2C_Stop(); return 1;}
//	}
//	I2C_Stop();
//	return 0;
//}

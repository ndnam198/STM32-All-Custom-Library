/**
 * @file myBootLoader.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief  This lib implement IAP (in application programming) feature which contains User Custom Bootloader
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_BOOTLOADER_H
#define __MY_BOOTLOADER_H

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "myDebug.h"
#include "myFlash.h"

/* Change this value based on your bootloader size */

extern UART_HandleTypeDef UART_DebugHandle;
extern UART_HandleTypeDef UART_BootloaderHandle;


#define UART_BOOT_CLK_DISABLE (__HAL_RCC_USART1_CLK_DISABLE())
#define UART_BOOT_GPIO_CLK_DISABLE (__HAL_RCC_GPIOA_CLK_DISABLE())

#define FLASH_USER_START_ADDR ADDR_FLASH_PAGE_30

#define MAX_LENGTH_OF_DATA (2048)  /* Bytes */
#define MAX_OPTION_BYTE (15)       /* Bytes */
#define PACKET_LENGTH_OF_FILE (15) /* Bytes */
#define PACKET_DATA (2048 + 13)    /* Bytes */

#define PING_TIMEOUT (1000)   //1s
#define UART_TIMEOUT (0x2710) //10s
#define CRC32INIT_VALUE (0xFFFFFFFF)

#define ACK_VALUE (0xFF)
#define NACK_VALUE (0x00)
/**
 * @brief UART_Buffer Option byte
 * 
 */
/***************************** Communication Protocol ***********************************************
 *
 * 				|-------------------- Packet Length of File (for erase flash )----------------------|
 *	Position: 	|   0	|     1    	|     2		|     3		|     4     |     5     |     6		 	|
 * 				|-----------------------------------------------------------------------------------|
 * 	Value:		| '*'	| NData[2] 	| NData[1]	| NData[0]	| !NData[2] | !NData[1]	| !NData[0]	 	|
 * 				|-----------------------------------------------------------------------------------|
 *	Position: 	|   7		|     8    	|     9  	|    10		|    11     |    12     |    13	 	|
 * 				|-----------------------------------------------------------------------------------|
 * 	Value:		| CRC32[3]	| CRC32[2] 	| CRC32[1]	| CRC32[0]	| !CRC32[3] | !CRC32[2]	| !CRC32[1]	|
 * 				|-----------------------------------------------------------------------------------|
 *	Position: 	|   14		|
 * 				|-----------|
 * 	Value:		| !CRC32[0]	|
 * 				|-----------|
 *
 *
 * 				|------------------ Packet Data (max data length = 2048 bytes) ---------------------|
 *	Position: 	|   0	|     1    	|     2		|     3		|     4     |     5     |      ...	 	|
 * 				|-----------------------------------------------------------------------------------|
 * 	Value:		| '!'	| NData[1] 	| NData[0]	| !NData[1]	| !NData[0] |  Data[0]	| 	   ...    	|
 * 				|-----------------------------------------------------------------------------------|
 *	Position: 	|   n+5		|   n+6    	|   n+7  	|   n+8		|   n+9     |   n+10    |   n+11 	|
 * 				|-----------------------------------------------------------------------------------|
 * 	Value:		| Data[n]	| CRC32[3] 	| CRC32[2]	| CRC32[1]	|  CRC32[0] | !CRC32[3]	| !CRC32[2]	|
 * 				|-----------------------------------------------------------------------------------|
 *	Position: 	|  n+12		|	n+13	|
 * 				|-----------------------|
 * 	Value:		| !CRC32[1]	| !CRC32[0]	|
 * 				|-----------|-----------|
 *
 */

/***************************** Define BootLoader Connection *************************************/

/* UART for Bootloader */

typedef void (*pFunction)(void);
extern uint8_t *UART_Buffer;

/**
 * @brief  This function is used to initialize the UART1 for communicate
 *      with PC application using Low Level Driver.
 *      UART - 8 bits data length, 1 bit stop, none parity bit
 *
 * @para    None
 * @retval None
 */
void bootloaderUARTInit(USART_TypeDef *Instance, UART_HandleTypeDef UART_BootloaderHandle, uint32_t baudrate);

/**
 * @brief  This function is used to initialize the UART6  for printing Debug information
 *      using HAL Driver.
 *      UART - 8 bits data length, 1 bit stop, none parity bit
 *
 * @para    None
 * @retval None
 */
void debugUARTInit(USART_TypeDef *Instance, UART_HandleTypeDef UART_DebugHandle, uint32_t baudrate);

/**
 * @brief Jump to User Application 
 * 
 */
void bootloaderJumpToApp(void);

/**
 * @brief Free all peripherals before jumping to app
 * 
 */
void bootloaderFreePeriph(void);

/**
 * @brief  	This function is used to send ACK or NACK to PC
 * @para	None
 * @retval 	CRC32 Value
 */
void bootloaderSendACK(uint8_t ack_value);

/**
 * @brief  This function is used to erase main application in flash
 * @para	None
 * @retval None
 */
void bootloaderEraseApplicationFlash(uint32_t application_size);

/**
 * @brief  	This function is used to calculate CRC32 from data buffer
 * @para	None
 * @retval 	CRC32 Value
 */
uint32_t calculateCRC(uint32_t initialValue, uint8_t *buffer, uint32_t bufferLength);

/**
 * @brief  This function writes a data buffer in flash (data are 8-bit aligned).
 * @note   After writing data buffer, the flash content is checked.
 * @param  FlashAddress: start address for writing data buffer
 * @param  Data: pointer on data buffer (this is 8 bit array)
 * @param  DataLength: length of data buffer (unit is 8-bit)
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 *         2: Written Data in flash memory is different from expected one
 */
uint32_t bootloaderWriteFlash(uint32_t FlashAddress, uint8_t *Data, uint32_t DataLength);

#endif // !_MY_BOOTLOADER_H

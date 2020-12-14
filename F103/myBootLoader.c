#include "myBootLoader.h"
#include "stm32f1xx_hal.h"

static void bootUSARTdeInit(void);

void bootloaderUARTInit(USART_TypeDef *Instance, UART_HandleTypeDef UART_BootloaderHandle, uint32_t baudrate)
{
    UART_BootloaderHandle.Instance = Instance;
    UART_BootloaderHandle.Init.BaudRate = baudrate;
    UART_BootloaderHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UART_BootloaderHandle.Init.StopBits = UART_STOPBITS_1;
    UART_BootloaderHandle.Init.Parity = UART_PARITY_NONE;
    UART_BootloaderHandle.Init.Mode = UART_MODE_TX_RX;
    UART_BootloaderHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_BootloaderHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UART_BootloaderHandle) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

void debugUARTInit(USART_TypeDef *Instance, UART_HandleTypeDef UART_DebugHandle, uint32_t baudrate)
{
    UART_DebugHandle.Instance = Instance;
    UART_DebugHandle.Init.BaudRate = baudrate;
    UART_DebugHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UART_DebugHandle.Init.StopBits = UART_STOPBITS_1;
    UART_DebugHandle.Init.Parity = UART_PARITY_NONE;
    UART_DebugHandle.Init.Mode = UART_MODE_TX_RX;
    UART_DebugHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_DebugHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UART_DebugHandle) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

void bootloaderFreePeriph(void)
{
    HAL_UART_DeInit(&DEBUG_USART);
    bootUSARTdeInit();

    HAL_RCC_DeInit();
    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
}

void bootloaderJumpToApp(void)
{
    uint32_t jump_address = *(volatile uint32_t *)(FLASH_USER_START_ADDR + 4);
    pFunction jump_function = (pFunction)jump_address; /* cast jump_address to void function*/

    PRINTF("\r\nJump to Application at ");
    PRINT_VAR(FLASH_USER_START_ADDR);

    free(UART_Buffer);
    /* DeInit all peripherals before jumping to application */
    bootloaderFreePeriph();

    /* Set the application stack pointer */
    __set_MSP(*(volatile uint32_t *)FLASH_USER_START_ADDR);

    jump_function();
    while (1)
    {
        /* If CPU jump here -> error occur */
        __NOP();
    }
}

void bootloaderSendACK(uint8_t ack_value)
{
    uint8_t send_data = ack_value;
    HAL_UART_Transmit(&UART_BootloaderHandle, (uint8_t *)&send_data, 1, UART_TIMEOUT);
}

void bootloaderEraseApplicationFlash(uint32_t application_size)
{
    uint32_t nb_of_delete_pages = (application_size / PAGE_SIZE) + 1;
    Flash_ErasePage(FLASH_USER_START_ADDR, nb_of_delete_pages);
}

/**
 * @brief  	This function is used to calculate CRC32 from data
 * @para	None
 * @retval 	CRC32 Value
 */
static uint32_t calculateCRCWord(uint32_t initial_value, uint8_t data)
{
    uint32_t temp = initial_value ^ data;

    for (uint32_t i = 0; i < 32; i++)
    {
        if ((temp & 0x80000000) != 0)
        {
            temp = (temp << 1) ^ 0x04C11DB7; // Polynomial used in STM32
        }
        else
        {
            temp <<= 1;
        }
    }

    return (temp);
}

uint32_t calculateCRC(uint32_t initialValue, uint8_t *buffer, uint32_t bufferLength)
{
    uint32_t temp = initialValue;
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        temp = calculateCRCWord(temp, *(buffer + i));
    }
    return temp;
}

uint32_t bootloaderWriteFlash(uint32_t FlashAddress, uint8_t *Data, uint32_t DataLength)
{
    uint32_t i = 0;
    uint8_t returnValue = 0;

    HAL_FLASH_Unlock();

    for (i = 0; (i < DataLength) && (FlashAddress <= (FLASH_END_ADDRESS - 4)); i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FlashAddress, *(uint8_t *)(Data + i)) == HAL_OK)
        {
            /* Check the written value */
            if (*(uint8_t *)FlashAddress != *(uint8_t *)(Data + i))
            {
                /* Flash content doesn't match SRAM content */
                returnValue = (2);
            }
            /* Increment FLASH destination address */
            FlashAddress += 1;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            returnValue = (1);
        }
    }

    HAL_FLASH_Lock();
    return returnValue;
}

static void bootUSARTdeInit(void)
{
	USART1->CR1 &=  ~USART_CR1_UE;
	USART1->CR1 = 0x0U;
	USART1->CR2 = 0x0U;
	USART1->CR3 = 0x0U;

    UART_BOOT_GPIO_CLK_DISABLE;
    UART_BOOT_CLK_DISABLE;
}

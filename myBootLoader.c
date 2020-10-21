#include "myBootLoader.h"

void bootloaderFreePeriph(void)
{
    LL_USART_DeInit(USART2);

    LL_RCC_DeInit();
    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
}

void bootloaderJumpToApp(void)
{
    uint32_t jump_address = *(volatile uint32_t *)(FLASH_USER_START_ADDR + 4);
    pFunction jump_function = (pFunction)jump_address; /* cast jump_address to void function*/

    print("\r\nJump to Application at ");
    printVar(FLASH_USER_START_ADDR);

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

void bootloaderTaskFunction(void)
{
}

int8_t getSector(uint32_t flash_address)
{
    uint32_t currentSector;
    currentSector = flash_address / ADDR_FLASH_PAGE_0;
    if (IS_PAGE_IN_RANGE(currentSector))
        return currentSector;
    else
    {
        return -1
    }
}

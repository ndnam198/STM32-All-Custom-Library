#include "myFlash.h"

Flash_Status_t Flash_WriteWord(uint32_t start_address, uint32_t end_address, uint32_t *data, uint32_t data_length)
{
    HAL_StatusTypeDef status = HAL_ERROR;
    Flash_Status_t flash_status = FLASH_ERROR;
    uint32_t index = 0;

    /* Struct for erase process */
    FLASH_EraseInitTypeDef EraseInitStruct;

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = start_address;
    EraseInitStruct.NbPages = (end_address - start_address) / FLASH_PAGE_SIZE;
    uint32_t PageError = 0xFFFFFFFF;

    /* Erase Flash based on initialize struct */
    status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    if (status != HAL_OK)
    {
        return flash_status;
    }

    /* Start program flash with given data pointer */
    for(; data_length > 0; --data_length)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_address, data[index]);
#if defined(PRINT_DEBUG) /* PRINT_DEBUG */
        PRINTF("Programing FLASH at address");
        GET_VAR(start_address);
        PRINTF("with data");
        GET_VAR(data[index]);
#endif /* !PRINT_DEBUG */
        if (status != HAL_OK)
        {
            return flash_status;
        }
        ++index;
        start_address += 4;
    }

    HAL_FLASH_Lock();
    return FLASH_SUCCESS;
}

uint32_t Flash_ReadAddress(uint32_t flash_address)
{
    return *(volatile uint32_t *)(flash_address);
}

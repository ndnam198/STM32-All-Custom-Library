/**
 * @file myFlash.h
 * @author Nam Nguyen (ndnam198@gmail.com)
 * @brief  This lib provides method to control internal Flash module read/write sequence of STM32F1xx Medium density chip
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_FLASH_H
#define __MY_FLASH_H

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "myDebug.h"

/* STM32F103C8 Medium Density Flash Define - BEGIN */

#define PAGE_SIZE (1024u)
#define MAX_FLASH_PAGES 64U
#define IS_PAGE_IN_RANGE(page) ((page >= 0) || (page < MAX_FLASH_PAGES))
#define ADDR_FLASH_PAGE_0 ((uint32_t)0x08000000)  /* Base Address Page 0, 1Kbytes each */
#define ADDR_FLASH_PAGE_1 ((uint32_t)0x08000400)  /* Base Address Page 1, 1Kbytes each */
#define ADDR_FLASH_PAGE_2 ((uint32_t)0x08000800)  /* Base Address Page 2, 1Kbytes each */
#define ADDR_FLASH_PAGE_3 ((uint32_t)0x08000c00)  /* Base Address Page 3, 1Kbytes each */
#define ADDR_FLASH_PAGE_4 ((uint32_t)0x08001000)  /* Base Address Page 4, 1Kbytes each */
#define ADDR_FLASH_PAGE_5 ((uint32_t)0x08001400)  /* Base Address Page 5, 1Kbytes each */
#define ADDR_FLASH_PAGE_6 ((uint32_t)0x08001800)  /* Base Address Page 6, 1Kbytes each */
#define ADDR_FLASH_PAGE_7 ((uint32_t)0x08001c00)  /* Base Address Page 7, 1Kbytes each */
#define ADDR_FLASH_PAGE_8 ((uint32_t)0x08002000)  /* Base Address Page 8, 1Kbytes each */
#define ADDR_FLASH_PAGE_9 ((uint32_t)0x08002400)  /* Base Address Page 9, 1Kbytes each */
#define ADDR_FLASH_PAGE_10 ((uint32_t)0x08002800) /* Base Address Page 10, 1Kbytes each */
#define ADDR_FLASH_PAGE_11 ((uint32_t)0x08002c00) /* Base Address Page 11, 1Kbytes each */
#define ADDR_FLASH_PAGE_12 ((uint32_t)0x08003000) /* Base Address Page 12, 1Kbytes each */
#define ADDR_FLASH_PAGE_13 ((uint32_t)0x08003400) /* Base Address Page 13, 1Kbytes each */
#define ADDR_FLASH_PAGE_14 ((uint32_t)0x08003800) /* Base Address Page 14, 1Kbytes each */
#define ADDR_FLASH_PAGE_15 ((uint32_t)0x08003c00) /* Base Address Page 15, 1Kbytes each */
#define ADDR_FLASH_PAGE_16 ((uint32_t)0x08004000) /* Base Address Page 16, 1Kbytes each */
#define ADDR_FLASH_PAGE_17 ((uint32_t)0x08004400) /* Base Address Page 17, 1Kbytes each */
#define ADDR_FLASH_PAGE_18 ((uint32_t)0x08004800) /* Base Address Page 18, 1Kbytes each */
#define ADDR_FLASH_PAGE_19 ((uint32_t)0x08004c00) /* Base Address Page 19, 1Kbytes each */
#define ADDR_FLASH_PAGE_20 ((uint32_t)0x08005000) /* Base Address Page 20, 1Kbytes each */
#define ADDR_FLASH_PAGE_21 ((uint32_t)0x08005400) /* Base Address Page 21, 1Kbytes each */
#define ADDR_FLASH_PAGE_22 ((uint32_t)0x08005800) /* Base Address Page 22, 1Kbytes each */
#define ADDR_FLASH_PAGE_23 ((uint32_t)0x08005c00) /* Base Address Page 23, 1Kbytes each */
#define ADDR_FLASH_PAGE_24 ((uint32_t)0x08006000) /* Base Address Page 24, 1Kbytes each */
#define ADDR_FLASH_PAGE_25 ((uint32_t)0x08006400) /* Base Address Page 25, 1Kbytes each */
#define ADDR_FLASH_PAGE_26 ((uint32_t)0x08006800) /* Base Address Page 26, 1Kbytes each */
#define ADDR_FLASH_PAGE_27 ((uint32_t)0x08006c00) /* Base Address Page 27, 1Kbytes each */
#define ADDR_FLASH_PAGE_28 ((uint32_t)0x08007000) /* Base Address Page 28, 1Kbytes each */
#define ADDR_FLASH_PAGE_29 ((uint32_t)0x08007400) /* Base Address Page 29, 1Kbytes each */
#define ADDR_FLASH_PAGE_30 ((uint32_t)0x08007800) /* Base Address Page 30, 1Kbytes each */
#define ADDR_FLASH_PAGE_31 ((uint32_t)0x08007c00) /* Base Address Page 31, 1Kbytes each */
#define ADDR_FLASH_PAGE_32 ((uint32_t)0x08008000) /* Base Address Page 32, 1Kbytes each */
#define ADDR_FLASH_PAGE_33 ((uint32_t)0x08008400) /* Base Address Page 33, 1Kbytes each */
#define ADDR_FLASH_PAGE_34 ((uint32_t)0x08008800) /* Base Address Page 34, 1Kbytes each */
#define ADDR_FLASH_PAGE_35 ((uint32_t)0x08008c00) /* Base Address Page 35, 1Kbytes each */
#define ADDR_FLASH_PAGE_36 ((uint32_t)0x08009000) /* Base Address Page 36, 1Kbytes each */
#define ADDR_FLASH_PAGE_37 ((uint32_t)0x08009400) /* Base Address Page 37, 1Kbytes each */
#define ADDR_FLASH_PAGE_38 ((uint32_t)0x08009800) /* Base Address Page 38, 1Kbytes each */
#define ADDR_FLASH_PAGE_39 ((uint32_t)0x08009c00) /* Base Address Page 39, 1Kbytes each */
#define ADDR_FLASH_PAGE_40 ((uint32_t)0x0800a000) /* Base Address Page 40, 1Kbytes each */
#define ADDR_FLASH_PAGE_41 ((uint32_t)0x0800a400) /* Base Address Page 41, 1Kbytes each */
#define ADDR_FLASH_PAGE_42 ((uint32_t)0x0800a800) /* Base Address Page 42, 1Kbytes each */
#define ADDR_FLASH_PAGE_43 ((uint32_t)0x0800ac00) /* Base Address Page 43, 1Kbytes each */
#define ADDR_FLASH_PAGE_44 ((uint32_t)0x0800b000) /* Base Address Page 44, 1Kbytes each */
#define ADDR_FLASH_PAGE_45 ((uint32_t)0x0800b400) /* Base Address Page 45, 1Kbytes each */
#define ADDR_FLASH_PAGE_46 ((uint32_t)0x0800b800) /* Base Address Page 46, 1Kbytes each */
#define ADDR_FLASH_PAGE_47 ((uint32_t)0x0800bc00) /* Base Address Page 47, 1Kbytes each */
#define ADDR_FLASH_PAGE_48 ((uint32_t)0x0800c000) /* Base Address Page 48, 1Kbytes each */
#define ADDR_FLASH_PAGE_49 ((uint32_t)0x0800c400) /* Base Address Page 49, 1Kbytes each */
#define ADDR_FLASH_PAGE_50 ((uint32_t)0x0800c800) /* Base Address Page 50, 1Kbytes each */
#define ADDR_FLASH_PAGE_51 ((uint32_t)0x0800cc00) /* Base Address Page 51, 1Kbytes each */
#define ADDR_FLASH_PAGE_52 ((uint32_t)0x0800d000) /* Base Address Page 52, 1Kbytes each */
#define ADDR_FLASH_PAGE_53 ((uint32_t)0x0800d400) /* Base Address Page 53, 1Kbytes each */
#define ADDR_FLASH_PAGE_54 ((uint32_t)0x0800d800) /* Base Address Page 54, 1Kbytes each */
#define ADDR_FLASH_PAGE_55 ((uint32_t)0x0800dc00) /* Base Address Page 55, 1Kbytes each */
#define ADDR_FLASH_PAGE_56 ((uint32_t)0x0800e000) /* Base Address Page 56, 1Kbytes each */
#define ADDR_FLASH_PAGE_57 ((uint32_t)0x0800e400) /* Base Address Page 57, 1Kbytes each */
#define ADDR_FLASH_PAGE_58 ((uint32_t)0x0800e800) /* Base Address Page 58, 1Kbytes each */
#define ADDR_FLASH_PAGE_59 ((uint32_t)0x0800ec00) /* Base Address Page 59, 1Kbytes each */
#define ADDR_FLASH_PAGE_60 ((uint32_t)0x0800f000) /* Base Address Page 60, 1Kbytes each */
#define ADDR_FLASH_PAGE_61 ((uint32_t)0x0800f400) /* Base Address Page 61, 1Kbytes each */
#define ADDR_FLASH_PAGE_62 ((uint32_t)0x0800f800) /* Base Address Page 62, 1Kbytes each */
#define ADDR_FLASH_PAGE_63 ((uint32_t)0x0800fc00) /* Base Address Page 63, 1Kbytes each */
#define FLASH_END_ADDRESS ((uint32_t)0x08010000)

typedef enum
{
    FLASH_SUCCESS = 0x00U,
    FLASH_ERROR,
} Flash_Status_t;

/**
 * @brief Write data to flash
 * 
 * @param start_address 
 * @param end_address 
 * @param data 
 * @param data_length 
 * @return Flash_Status_t 
 */
Flash_Status_t Flash_WriteWord(uint32_t start_address, uint32_t end_address, uint32_t *data, uint32_t data_length);

/**
 * @brief Read data from a specific flash address
 * 
 * @param flash_address 
 * @return uint32_t 
 */
uint32_t Flash_ReadAddress(uint32_t flash_address);

/**
 * @brief Get the Sector object
 * 
 * @param flash_address[in] a 32 bit interger flash address 
 * @return int8_t the page that the passed-in flash address belongs to or else -1
 */
int8_t Flash_getAddressPage(uint32_t flash_address);

/**
 * @brief Erase number of pages
 * 
 */
void Flash_ErasePage(uint32_t start_address, uint32_t nb_of_page);

#endif // !__MY_FLASH_H

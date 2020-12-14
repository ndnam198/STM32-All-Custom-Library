/**
 * @file myUartHandle.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MY_UART_HANDLE_H
#define __MY_UART_HANDLE_H /* __MY_UART_HANDLE_H */

#include "main.h"

#define USART_RX_BUFFER_SIZE (100u)

typedef struct USART_StringReceive {
    uint8_t rx_index;
    uint8_t rx_buffer[USART_RX_BUFFER_SIZE];
    uint8_t rx_data[2];
    uint8_t rx_cplt_flag;
} USART_StringReceive_t;

/**
 * @brief Initialize UART used for serving CLI command 
 * 
 * @param huart 
 * @param USARTx 
 * @param uart_receive_handle 
 */
void vUART_CLI_Init(UART_HandleTypeDef *huart, USART_StringReceive_t *uart_receive_handle);

/**
 * @brief Re-define UART receive complete callback function
 * 
 * @param huart 
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
 * @brief Parse command received
 * 
 * @param command 
 * @param target_string_array 
 * @return uint8_t index of the equal stirng
 */
__weak int32_t ucParseCLICommand(uint8_t *command, uint8_t *target_string_array);

/**
 * @brief Execute corresponding command function
 * 
 * @param uart_receive_handle 
 */
void vExecuteCLIcmd(USART_StringReceive_t *uart_receive_handle);


#endif /* !__MY_UART_HANDLE_H */

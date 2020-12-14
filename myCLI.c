#include "myCLI.h"
#include "myMisc.h"
#include "myDebug.h"
#include "myF767.h"

/* Should be excluded if not used to prevent build errors */
extern USART_StringReceive_t uart_receive_handle;
extern MCUProcessingEvaluate_t mcu_process_time_handle;

void vUART_CLI_Init(UART_HandleTypeDef *huart, USART_StringReceive_t *uart_receive_handle)
{
    huart->Instance = USARTX; /* Select this parameter according to USART Instance configured in .ioc */
    huart->Init.BaudRate = 115200;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(huart) != HAL_OK) /* Inside HAL_UART_Init also initialize GPIO used for USART */
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    /* Enable ISR when receive via USART */
    HAL_UART_Receive_IT(huart, (uint8_t *)(&uart_receive_handle->rx_data), 2);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t i;
    /* Process USART2 Receive_Cplt_IT */
    if (huart->Instance == USARTX)
    {
        /* Reset Receive Buffer whenever index_value = 0 */
        if (uart_receive_handle.rx_index == 0)
        {
            for (i = 0; i < USART_RX_BUFFER_SIZE; i++)
            {
                uart_receive_handle.rx_buffer[i] = 0;
            }
        }

        /* Case when user input data not equal to "\r" */
        if (uart_receive_handle.rx_data[0] != 13)
        {
            uart_receive_handle.rx_buffer[uart_receive_handle.rx_index++] = uart_receive_handle.rx_data[0];
        }

        else /* Case when user input data = "\r" */
        {
            uart_receive_handle.rx_index = 0;
            uart_receive_handle.rx_cplt_flag = 1;
        }

        /* Trigger to Receive and jump into ISR on each ISR process is necessary */
        HAL_UART_Receive_IT(huart, (uint8_t *)(&uart_receive_handle.rx_data), 1);
    }
}

void vExecuteCLIcmd(USART_StringReceive_t *uart_receive_handle)
{

    char *input_string = (char *)&uart_receive_handle->rx_buffer;
    /* Clear receive complete flag */
    uart_receive_handle->rx_cplt_flag = 0;
    // PRINTF("Command string: \"%s\"\r\n", uart_receive_handle->rx_buffer);
    if (IS_STRING(input_string, "help"))
    {
        printf("/* -------------------------------------------------------------------------- */\r\n");
        printf("/*                               CLI - HELP MENU                              */\r\n");
        printf("/*--------------------------------------------------------------------------- */\r\n");
        printf("\"help\"                 : Display help menu\r\n");
        printf("\"<LED_color> <state>\"  : Control equivalent color LED on or off\r\n");
        printf("\"time\"                 : Get MCU working time\r\n");
        printf("\"process\"              : Evaluate superloop processing time\r\n");
        printf("\"reboot\"               : Perform chip reset\r\n");
        printf("\"clock\"                : MCU clock\r\n");
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, ""))
    {
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "red 1"))
    {
        WRITE_LED(LED_RED, ON);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "red 0"))
    {
        WRITE_LED(LED_RED, OFF);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "blue 1"))
    {
        WRITE_LED(LED_BLUE, ON);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "blue 0"))
    {
        WRITE_LED(LED_BLUE, OFF);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "green 1"))
    {
        WRITE_LED(LED_GREEN, ON);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "green 0"))
    {
        WRITE_LED(LED_GREEN, OFF);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "time"))
    {
        printf("MCU working time: ");
        __PRINT_TIME_STAMP();
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "process"))
    {
        vPrintProcessingTime(&mcu_process_time_handle);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "clock"))
    {
        printf("RCC_HCLK Freq: %lu\r\n", HAL_RCC_GetHCLKFreq());
        printf("Tick Freq: %d\r\n", 1000 / HAL_GetTickFreq());
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "reboot"))
    {
        NVIC_SystemReset();
    }
    else
    {
        printf("Unknown Command: \"%s\"\r\n", input_string);
        printf("\r\n\r\n>>> ");
    }

}

__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */
    if (htim->Instance == CLI_TIMER)
    {
        static uint32_t count;
        count++;
        if (count == 50) /* Every 50ms */
        {
            count = 0;
            if (uart_receive_handle.rx_cplt_flag == 1)
            {
                __PRINT_TIME_STAMP();
                vExecuteCLIcmd((USART_StringReceive_t *)&uart_receive_handle);
            }
        }
    }
}

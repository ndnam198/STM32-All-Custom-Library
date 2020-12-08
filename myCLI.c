#include "myCLI.h"
#include "myMisc.h"
#include "myDebug.h"
#include "myF103.h"

#define COMMAND_INDEX (0U)
/* Should be excluded if not used to prevent build errors */
extern USART_StringReceive_t uart_receive_handle;
extern MCUProcessingEvaluate_t mcu_process_time_handle;

void vUART_Init(UART_HandleTypeDef *huart, USART_TypeDef *USARTx, USART_StringReceive_t *uart_receive_handle)
{
    huart->Instance = USARTx; /* Select this parameter according to USART Instance configured in .ioc */
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
    if (huart->Instance == USART2)
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

    // ""          /* 0 */
    // "help",     /* 1 */
    // "led1 on",  /* 2 */
    // "led1 off", /* 3 */
    // "led4 on",  /* 4 */
    // "led4 off", /* 5 */
    // "time",     /* 6 */
    // "evaluate", /* 7 */
    char *input_string = (char *)&uart_receive_handle->rx_buffer;
    // PRINTF("Command string: \"%s\"\r\n", uart_receive_handle->rx_buffer);
    printf("/* -------------------------------------------------------------------------- */\r\n");
    printf("Your command: %s\r\n", input_string);
    if (IS_STRING(input_string, "help"))
    {
        printf("/* -------------------------------------------------------------------------- */\r\n");
        printf("/*                               CLI - HELP MENU                              */\r\n");
        printf("/*--------------------------------------------------------------------------- */\r\n");
        printf("\"help\"            : Display help menu\r\n");
        printf("\"led<x> <state>\"  : Control LED x in range 1 or 4, state either 1-on or 0-off\r\n");
        printf("\"time\"            : Get MCU working time\r\n");
        printf("\"process\"         : Evaluate superloop processing time\r\n");
        printf("\"reboot\"          : Perform chip reset\r\n");
        printf("\"clock\"           : MCU clock\r\n");
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, ""))
    {
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "led1 1"))
    {
        __MY_WRITE_LED(LED_1, ON);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "led1 0"))
    {
        __MY_WRITE_LED(LED_1, OFF);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "led4 1"))
    {
        __MY_WRITE_LED(LED_4, ON);
        printf("\r\n\r\n>>> ");
    }
    else if (IS_STRING(input_string, "led4 0"))
    {
        __MY_WRITE_LED(LED_4, OFF);
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
        printf("Tick Freq: %d\r\n", 1000/HAL_GetTickFreq());
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
    /* Clear receive complete flag */
    uart_receive_handle->rx_cplt_flag = 0;
}

__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */
    if (htim->Instance == TIM3)
    {
        static uint32_t count;
        count++;
        if (count == 50) /* Every 50ms */
        {
            count = 0;
            if (uart_receive_handle.rx_cplt_flag == 1)
            {
                vExecuteCLIcmd((USART_StringReceive_t *)&uart_receive_handle);
            }
        }
    }
}

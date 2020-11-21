/* User-lib */
#include "myDebug.h"
#include "myF103.h"
// #include "myFlash.h"
// #include "myBootLoader.h"
#include "myMisc.h"
#include "myRingBuffer.h"
// #include "myRTOSaddons.h"
#include "retarget.h"
#include "myCLI.h"
// #include "mySHT31.h"

#define BLINK_LED_FREQ (500 * HAL_GetTickFreq())
#define APP1_FREQ (100 * HAL_GetTickFreq())
#define APP2_FREQ (1000 * HAL_GetTickFreq())

/* --------------------------- VARIABLE-DEFINITION -------------------------- */
uint32_t prev_time_blink_led = 0;
uint32_t prev_time_app_1 = 0;
uint32_t prev_time_app_2 = 0;
uint32_t superloop_first_tick = 0;
uint32_t superloop_process_time = 0;
volatile USART_StringReceive_t uart_receive_handle = {0};
MCUProcessingEvaluate_t mcu_process_time_handle = {
    .current_process_time = 0,
    .max_process_time = 0,
    .min_process_time = 5,
};
// SHT31_TypeDef_t sht31_handle = {0};

/* --------------------------- FUNCTION-PROTOTYPE --------------------------- */
void _Error_Handler(char *file, int line);

/* -------------------------------------------------------------------------- */
/*                              IN-MAIN-FUNCTION                              */
/* -------------------------------------------------------------------------- */

/* Initialize bitband I2C */
// I2C_Init();
/* Initialize Watchdog Timer */
vIWDG_Init(&hiwdg, IWDG_TIME);
/* Initialize USART for printing DEBUG information and supporting CLI */
vUART_Init(&huart2, USART2, (USART_StringReceive_t *)&uart_receive_handle);
/* USER CODE END 2 */

/* Retarget USART to use printf function instead */
__RETARGET_INIT(DEBUG_USART);
/* Get MCU reset cause */
__PRINT_RESET_CAUSE();
__MY_OFF_ALL_LED();

HAL_TIM_Base_Start_IT(&htim3);
printf("Start Application\r\n");

/* Infinite loop */
/* USER CODE BEGIN WHILE */
prev_time_blink_led = prev_time_app = HAL_GetTick();
while (1)
{
    superloop_first_tick = HAL_GetTick();
    /* Task blink led 500ms */
    if (superloop_first_tick - prev_time_blink_led >= BLINK_LED_FREQ)
    {
        prev_time_blink_led = superloop_first_tick;
        __MY_TOGGLE_LED(LED_4);
    }

    /* Task read sensor 1000ms */
    if (superloop_first_tick - prev_time_app_1 >= APP1_FREQ)
    {
        prev_time_app_1 = superloop_first_tick;
    }

    if (superloop_first_tick - prev_time_app_2 >= APP2_FREQ)
    {
        prev_time_app_2 = superloop_first_tick;
    }

    superloop_process_time = HAL_GetTick() - superloop_first_tick;
    vMCUProcessTimeUpdate(&mcu_process_time_handle, superloop_process_time);
    HAL_IWDG_Refresh(&hiwdg);
}

/* ------------------------- FUNCTION-IMPLEMENTATION ------------------------ */

/* --------------------------- CLI-Command-Execute -------------------------- */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
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
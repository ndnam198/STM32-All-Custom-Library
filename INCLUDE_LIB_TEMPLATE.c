/* User-lib */
#include "myDebug.h"
#include "myF103.h"
// #include "myFlash.h"
// #include "myBootLoader.h"
#include "myMisc.h"
#include "myRingBuffer.h"
// #include "myRTOSaddons.h"
#include "retarget.h"

#define BLINK_LED_FREQ (500u)
#define APP1_FREQ   1000u)
#define IWDG_TIME (5000u)

uint32_t prev_time_blink_led = 0;
uint32_t prev_time_app = 0;
uint32_t superloop_first_tick = 0;

void _Error_Handler(char *file, int line);

/* Initialize Watchdog Timer */
vIWDG_Init(&hiwdg, IWDG_TIME);
__RETARGET_INIT(DEBUG_USART);
__PRINT_RESET_CAUSE();
__MY_OFF_ALL_LED();
printf("IWDG set %lums\r\n", IWDG_TIME);
/* USER CODE END 2 */

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
    if (superloop_first_tick - prev_time_app >= APP1_FREQ)
    {
        prev_time_app = superloop_first_tick;
        __PRINT_TIME_STAMP();
        __MY_TOGGLE_LED(LED_1);
    }
    // printf("SUPERLOOP process time: %d\r\n", HAL_GetTick() - superloop_first_tick);
    HAL_IWDG_Refresh(&hiwdg);
}

void _Error_Handler(char *file, int line)
{
    while (1)
    {
        printf("\r\nError file %s line %d", file, line);
    }
}

/* Replace all Error_Handler() with _Error_Handler(__FILE__, __LINE__)*/
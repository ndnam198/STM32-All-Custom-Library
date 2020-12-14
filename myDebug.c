#include "myDebug.h"

#if defined(configHAL_UART) /* configHAL_UART */
void vUARTSend(UART_HandleTypeDef huart, uint8_t *String)
{
	HAL_UART_Transmit(&huart, (uint8_t *)String, strlen((char *)String), 100);
}
#endif /* !configHAL_UART */

#if defined(configLL_UART) /* configLL_UART */
void vUARTSend(USART_TypeDef *USARTx, uint8_t *String)
{
	uint32_t ulStringLen = 0;
	uint32_t i = 0;
	uint32_t ulBlockTime = 10000;
	ulStringLen = strlen((char *)String);

	while (ulStringLen)
	{
		LL_USART_TransmitData8(USARTx, (uint8_t)String[i]);
		i++;
		ulStringLen--;
		/* Check if transfer 1 byte completed */
		while (!LL_USART_IsActiveFlag_TC(USARTx))
		{
			if ((ulBlockTime--) == 0)
				break;
		}
	}
}
#endif /* !configLL_UART */

reset_cause_t resetCauseGet(void)
{
	/*
		RCC_FLAG_BORRST: BOR reset flag
		RCC_FLAG_PINRST: NRST pin reset flag
		RCC_FLAG_PORRST: POR/PDR reset flag
		RCC_FLAG_SFTRST: Software reset flag
		RCC_FLAG_IWDGRST: Independent watchdog reset flag
		RCC_FLAG_WWDGRST: Window watchdog reset flag
		RCC_FLAG_LPWRRST: Low power reset flag 
	*/
	reset_cause_t reset_cause;

	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
	{
		reset_cause = eRESET_CAUSE_LOW_POWER_RESET;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
	{
		reset_cause = eRESET_CAUSE_WINDOW_WATCHDOG_RESET;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		reset_cause = eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	{
		reset_cause = eRESET_CAUSE_SOFTWARE_RESET; // This reset is induced by calling the ARM CMSIS `NVIC_SystemReset()` function!
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
	{
		reset_cause = eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
	{
		reset_cause = eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET;
	}
#ifndef STM32_F1
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
	{
		reset_cause = eRESET_CAUSE_BROWNOUT_RESET;
	}
#endif /* !STM32_F1 */
	else
	{
		reset_cause = eRESET_CAUSE_UNKNOWN;
	}

	__HAL_RCC_CLEAR_RESET_FLAGS();

	return reset_cause;
}

const char *resetCauseGetName(reset_cause_t reset_cause)
{
	const char *reset_cause_name = "TBD";

	switch (reset_cause)
	{
	case eRESET_CAUSE_UNKNOWN:
		reset_cause_name = "UNKNOWN";
		break;
	case eRESET_CAUSE_LOW_POWER_RESET:
		reset_cause_name = "LOW_POWER_RESET";
		break;
	case eRESET_CAUSE_WINDOW_WATCHDOG_RESET:
		reset_cause_name = "WINDOW_WATCHDOG_RESET";
		break;
	case eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET:
		reset_cause_name = "INDEPENDENT_WATCHDOG_RESET";
		break;
	case eRESET_CAUSE_SOFTWARE_RESET:
		reset_cause_name = "SOFTWARE_RESET";
		break;
	case eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET:
		reset_cause_name = "POWER-ON_RESET (POR) / POWER-DOWN_RESET (PDR)";
		break;
	case eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET:
		reset_cause_name = "EXTERNAL_RESET_PIN_RESET";
		break;
	case eRESET_CAUSE_BROWNOUT_RESET:
		reset_cause_name = "BROWNOUT_RESET (BOR)";
		break;
	}
	return reset_cause_name;
}

void vIWDG_Init(IWDG_HandleTypeDef *hiwdg, uint32_t millis)
{

	/* Select INDEPENDENT_WATCHDOG */
	hiwdg->Instance = IWDG;
	/* Use prescaler LSI/128 */
	hiwdg->Init.Prescaler = IWDG_PRESCALER_32;
	hiwdg->Init.Reload = millis;
#ifndef STM32_F1
	hiwdg->Init.Window = millis;
#endif /* !STM32_F1 */
	if (HAL_IWDG_Init(hiwdg) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

#ifdef PRINT_DEBUG
	printf("Set IWDG %lums\r\n", IWDG_TIME);
#endif
}

__weak void _Error_Handler(char *file, int line)
{
	while (1)
	{
		printf("\r\nError file %s line %d", file, line);
	}
}

void vTimeStamp(uint32_t now_tick)
{
	uint8_t second, minute, hour;
	uint32_t millis_second, now_second;
	uint32_t hal_tick_freq;
	uint32_t divider;

	// HAL_TICK_FREQ_10HZ = 100U,
	// HAL_TICK_FREQ_100HZ = 10U,
	// HAL_TICK_FREQ_1KHZ = 1U,
	// HAL_TICK_FREQ_DEFAULT = HAL_TICK_FREQ_1KHZ
	hal_tick_freq = HAL_GetTickFreq();
	if (hal_tick_freq == HAL_TICK_FREQ_1KHZ)
	{
		divider = 1000;
	}
	else if (hal_tick_freq == HAL_TICK_FREQ_100HZ)
	{
		divider = 10000;
	}
	if (hal_tick_freq == HAL_TICK_FREQ_10HZ)
	{
		divider = 100000;
	}
	/* ex: 450235ms => now_second = 450 */
	now_second = now_tick / divider;
	/* ex: 450235ms => millis_second = 235 */
	millis_second = now_tick - now_second * divider;
	if (millis_second > 10000)
	{
		millis_second /= 100;
	}
	else if (millis_second > 1000)
	{
		millis_second /= 10;
	}
	/* ex: 450235ms => 450/60 = 7 */
	minute = now_second / 60;
	/* ex: 450235ms => 450 - 7*60 = 30 */
	second = now_second - minute * 60;
	if (minute > 60)
	{
		hour = minute / 60;
		minute = minute - hour * 60;
	}
	else
	{
		hour = 0;
	}

#if (defined(PRINT_DEBUG))
	printf("[%02d:%02d:%02d.%03lu]\r\n", hour, minute, second, millis_second);
#endif
	return;
}

void vMCUProcessTimeUpdate(MCUProcessingEvaluate_t *mcu_process_time_handle, uint32_t current_processing_time)
{
	//    HAL_TICK_FREQ_10HZ         = 100U,
	//    HAL_TICK_FREQ_100HZ        = 10U,
	//    HAL_TICK_FREQ_1KHZ         = 1U,
	uint32_t tick_freq, current_process_tick;
	tick_freq = HAL_GetTickFreq();
	if (tick_freq == HAL_TICK_FREQ_1KHZ)
	{
		current_process_tick = current_processing_time;
	}
	else if (tick_freq == HAL_TICK_FREQ_100HZ)
	{
		current_process_tick = current_processing_time / 10;
	}
	else
	{
		current_process_tick = current_processing_time / 100;
	}
	mcu_process_time_handle->current_process_time = current_process_tick;

	if (current_process_tick > mcu_process_time_handle->max_process_time)
	{
		mcu_process_time_handle->max_process_time = current_process_tick;
	}
	else
	{
	}
	if (current_process_tick < mcu_process_time_handle->min_process_time)
	{
		mcu_process_time_handle->min_process_time = current_process_tick;
	}
	else
	{
	}
	return;
}

void vPrintProcessingTime(MCUProcessingEvaluate_t *mcu_process_time_handle)
{
	newline;
	printf("Current processing time: %ldms\r\n", mcu_process_time_handle->current_process_time);
	printf("Max processing time: %ldms\r\n", mcu_process_time_handle->max_process_time);
	printf("Min processing time: %ldms\r\n", mcu_process_time_handle->min_process_time);
}

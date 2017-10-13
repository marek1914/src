
#include "hal_local.h"
#include "hal_time.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

static t_hal_timout_fn m_time_fn = m_null;
static TIM_HandleTypeDef    time_hd_config;
static TIM_OC_InitTypeDef 	 time_oc_config;

t_void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM21 && m_time_fn)
	{
		m_time_fn();
	}
}

t_void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM21)
	{
		__HAL_RCC_TIM21_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM21_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM21_IRQn);
	}
}

t_void SysTick_Handler(void)
{
	HAL_IncTick();
}

t_void TIM21_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&time_hd_config);
}

t_void hal_dly_next(t_u16 us)
{
	TIM21->CCR1 = TIM21->CCR1 + us;
}

t_void hal_dly_sync(t_u16 us)
{
	TIM21->CCR1 = TIM21->CNT + us;
}

t_u32 hal_get_ms_32()
{
	return HAL_GetTick();
}

t_u16 hal_get_us_16()
{
	return TIM21->CNT;
}

t_void hal_dly_init(void)
{
	time_hd_config.Instance = TIM21;
	time_hd_config.Init.Period = 0xffff;
	time_hd_config.Init.Prescaler = ((SystemCoreClock / 1000000) - 1);
	time_hd_config.Init.ClockDivision = 0;
	time_hd_config.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	if(HAL_TIM_Base_Init(&time_hd_config) != HAL_OK)
	{
	}
	
	/*##-2- Configure the Output Compare channels #########################################*/ 
	/* Common configuration for all channels */
	time_oc_config.OCMode     	= TIM_OCMODE_TIMING;
	time_oc_config.OCPolarity 	= TIM_OCPOLARITY_HIGH;
	time_oc_config.Pulse 		= 0;  
	time_oc_config.OCFastMode	= TIM_OCFAST_DISABLE;	
	if(HAL_TIM_OC_ConfigChannel(&time_hd_config, &time_oc_config, TIM_CHANNEL_1) != HAL_OK)
	{
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if(HAL_TIM_OC_Start_IT(&time_hd_config, TIM_CHANNEL_1) != HAL_OK)
	{
	}
}

t_void hal_dly_callback(t_hal_timout_fn pFn)
{
	m_time_fn = pFn;
}

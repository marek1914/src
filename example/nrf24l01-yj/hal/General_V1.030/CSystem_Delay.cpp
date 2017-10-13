#include "CTypes.h"
#include "CRadioChip.h"
#include "CStateTask.h"
#include "CSystem_Local.h"

#include "stm32f0xx.h"


t_vu32 msTimingTick = 0;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern "C" void SysTick_Handler(void)
{
	task.tick(++msTimingTick);
}

extern "C" void TIM14_IRQHandler(t_u16 idx)
{	
	if (TIM14->DIER & TIM14->SR & TIM_IT_CC1)
	{
		TIM14->SR = (uint16_t)(~TIM_IT_CC1);
		
		radiochip.timeout();
	}
}

void Dev_Delay_init(void)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000)-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 1;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);
	TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
	
	TIM14->CCER &= (uint16_t)~(TIM_CCER_CC1E);

	NVIC_EnableIRQ(TIM14_IRQn);
	NVIC_SetPriority(TIM14_IRQn, 3);

	/* TIM enable counter */
	TIM_Cmd(TIM14, ENABLE);

	SysTick_Config(SystemCoreClock / 1000);    
}

void Dev_Delay_Timout_Start(t_u16 us)
{
	__disable_irq();
	TIM14->CCR1  = TIM14->CNT + us;
	TIM14->CCER |= TIM_CCER_CC1E;
	TIM14->DIER |= TIM_IT_CC1;
	TIM14->SR    = (uint16_t)(~TIM_IT_CC1);
	__enable_irq();
}

void Dev_Delay_Timout_Stop(void)
{
	__disable_irq();
	TIM14->CCER &= (uint16_t)(~TIM_CCER_CC1E);
	TIM14->DIER &= (uint16_t)(~TIM_IT_CC1);
	TIM14->SR    = (uint16_t)(~TIM_IT_CC1);
	__enable_irq();
}

void Dev_Delay_Timout_Next(t_u16 us)
{
	TIM14->CCR1 = TIM14->CCR1 + us;
}


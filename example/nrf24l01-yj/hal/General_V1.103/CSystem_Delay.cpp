#include "CTypes.h"
#include "CRadioChip.h"
#include "CSystem.h"
#include "CSystem_Delay.h"

#include "stm32f10x.h"


t_vu32 msTimingTick = 0;

extern "C" void SysTick_Handler(void)
{
	msTimingTick++;
}

extern "C" void TIM3_IRQHandler(void)
{
	if (TIM3->DIER & TIM3->SR & TIM_IT_CC1)
	{
		TIM3->SR = (uint16_t)(~TIM_IT_CC1);
		
		radiochip.timeout();
	}
}

void Dev_Delay_init(void)
{  	
    RCC_ClocksTypeDef clk;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_GetClocksFreq(&clk);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = (clk.PCLK2_Frequency/1000000)-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 1;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    /* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	
	TIM3->CCER &= (uint16_t)~(TIM_CCER_CC1E);

	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 3);
	
	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
    
	SysTick_Config(SystemCoreClock / 1000);    
}

void Dev_Delay_Delay(t_u16 nms)
{
	t_u32 tick = msTimingTick + nms;
	
	while((msTimingTick < tick));
}

void Dev_Delay_Timout_Start(t_u16 us)
{
	__disable_irq();
	TIM3->CCR1  = TIM3->CNT + us;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->DIER |= TIM_IT_CC1;
	TIM3->SR    = (uint16_t)(~TIM_IT_CC1);
	__enable_irq();
}

void Dev_Delay_Timout_Stop(void)
{
	__disable_irq();
	TIM3->CCER &= (uint16_t)(~TIM_CCER_CC1E);
	TIM3->DIER &= (uint16_t)(~TIM_IT_CC1);
	TIM3->SR    = (uint16_t)(~TIM_IT_CC1);
	__enable_irq();
}

void Dev_Delay_Timout_Next(t_u16 us)
{
	TIM3->CCR1 = TIM3->CCR1 + us;
}

void Dev_Delay_Timout_Sync(t_u16 us)
{
	TIM3->CCR1 = TIM3->CNT + us;
}

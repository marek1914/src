#include "hal_local.h"
#include "hal_radio.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

static t_hal_interrupt_fn m_radioint = m_null;

void EXTI4_15_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET) 
	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
		
		if (m_radioint)
		{
			m_radioint();
		}
	}
}

//////////////////////////////////////////////////////////////////
void hal_radio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin       = GPIO_PIN_6;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	 /* Configure PB8 pin as input floating */
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin  = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Enable and set EXTI4_15 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

t_void hal_radio_enable(t_u8 en)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, en ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

t_void hal_radio_enable_delay(void)
{
	int i;
	for(i=0;i<100;i++);
}

t_void hal_radio_callback(t_hal_interrupt_fn pFn)
{
	m_radioint = pFn;
}

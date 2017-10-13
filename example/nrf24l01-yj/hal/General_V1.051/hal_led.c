#include "hal_local.h"
#include "hal_led.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"


t_void hal_led_onoff(t_u8 cha, t_u8 on)
{
	if (cha==0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, on ?  GPIO_PIN_SET : GPIO_PIN_RESET);
	}
	if (cha==1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, on ?  GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

t_void hal_led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	hal_led_onoff(0, 0);
	
	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_2;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin       = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

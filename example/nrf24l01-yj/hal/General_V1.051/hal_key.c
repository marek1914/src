#include "hal_local.h"
#include "hal_spi.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

void hal_key_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	__HAL_RCC_GPIOB_CLK_ENABLE();

	
	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_0;
	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

t_u8 hal_key_mask(void)
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) ? 0x00 : 0x01;
}


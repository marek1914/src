#include "hal_local.h"
#include "hal_spi.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

SPI_HandleTypeDef SpiHandle;

void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&SpiHandle);
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	//SCK MISO MOSI CE
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Enable SPI clock */
	__HAL_RCC_SPI1_CLK_ENABLE(); 
	
//	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	
	//SPI CE
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_3;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* SPI CE GPIO pin configuration  */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode= GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  

	/*##-3- Configure the NVIC for SPI #########################################*/
	/* NVIC for SPI */
	HAL_NVIC_SetPriority(SPI1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(SPI1_IRQn);	
}

t_u8 hal_spi_rw(t_u8 val)
{
	SPI1->CR1 |= SPI_CR1_SPE;
	SPI1->DR   = val;
	while(!(SPI1->SR & SPI_FLAG_RXNE));
	return SPI1->DR;
}

t_void hal_spi_w_buf(t_u8* buf, t_u8 len)
{
	t_u8 temp;
	SPI1->CR1 |= SPI_CR1_SPE;
	while(len--)
	{
		SPI1->DR = *buf++;
		while(!(SPI1->SR & SPI_FLAG_RXNE));
		temp = SPI1->DR;
		temp = temp;
	}
}
t_void hal_spi_r_buf(t_u8* buf, t_u8 len)
{
	SPI1->CR1 |= SPI_CR1_SPE;
	while(len--)
	{
		SPI1->DR = 0x00;
		while(!(SPI1->SR & SPI_FLAG_RXNE));
		*buf++ = SPI1->DR;
	}
}

void hal_spi_csn(t_bool en)
{
	if(en){
		GPIOA->BRR = GPIO_PIN_15 ;
	}else{
		GPIOA->BSRR = GPIO_PIN_15;
	}
}

void hal_spi_init(void)
{
	/*##-1- Configure the SPI peripheral #######################################*/
	/* Set the SPI parameters */
	SpiHandle.Instance               = SPI1;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.Mode              = SPI_MODE_MASTER;

	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
	{
	}
}

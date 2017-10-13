#include <stdarg.h>
#include <string.h>

#include "CDebugLog.h"
#include "CRadioChip.h"
#include "CSystem_Nrf24.h"
#include "stm32f10x.h"

extern "C" void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);	
		radiochip.interrupt();
	}
}


void Dev_Nrf24_Enable(t_bool en)
{
	if (en)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
	}
}

t_u8 Dev_Nrf24_RW(t_u8 val)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* Send t_u8 through the SPI2 peripheral */
	SPI_I2S_SendData(SPI2, val);

	/* Wait to receive a t_u8 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the t_u8 read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}

void Dev_Nrf24_W(t_u8 val)
{
	Dev_Nrf24_RW(val);
}

t_u8 Dev_Nrf24_R(void)
{
	return Dev_Nrf24_RW(0x00);
}

void Dev_Nrf24_CE(t_bool bHigh)
{
	if (bHigh){
		GPIO_SetBits	(GPIOB, GPIO_Pin_5);
	}else{
		GPIO_ResetBits	(GPIOB, GPIO_Pin_5);
	}
}

void Dev_Nrf24_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	//Test
	/* Configure I/O for the Chip Enable */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure the interruption (EXTI Source) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure I/O for the Chip Enable */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure SPI pins: SCK and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |	GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure I/O for the Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* disable the chip enable */
	Dev_Nrf24_CE(0);
	/* disable the chip select */
	Dev_Nrf24_Enable(m_false);	
	
	/* Enable SPI and GPIO clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	// Clock the radio with 16MHz
	RCC_MCOConfig(RCC_MCO_HSE);

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//9MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	/* Enable the SPI  */
	SPI_Cmd(SPI2, ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Dev_Nrf24_Test(t_bool bHigh)
{
	if (bHigh){
		GPIO_SetBits	(GPIOB, GPIO_Pin_0);
	}else{
		GPIO_ResetBits	(GPIOB, GPIO_Pin_0);
	}
}

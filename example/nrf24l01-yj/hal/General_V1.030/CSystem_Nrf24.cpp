#include <stdarg.h>
#include <string.h>

#include "CDebugLog.h"
#include "CRadioChip.h"
#include "CSystem_Nrf24.h"
#include "CSystem_Gpio.h"
#include "stm32f0xx.h"

#define CH0_RW(mask)	{	\
			if (val & mask){	\
				GPIOA->BSRR = 1 << 7;	\
			}else{	\
				GPIOA->BRR = 1 << 7;	\
			}	\
			GPIOB->BSRR = 1 << 0;	\
			if (GPIOA->IDR &(1 << 6)){	\
				ret |= mask;	\
			}	\
			GPIOB->BRR = 1 << 0;	\
		}

#define CH1_RW(mask)	{	\
			if (val & mask){	\
				GPIOC->BSRR = 1 << 15;	\
			}else{	\
				GPIOC->BRR = 1 << 15;	\
			}	\
			GPIOA->BSRR = 1 << 0;	\
			if (GPIOC->IDR &(1 <<14)){	\
				ret |= mask;	\
			}	\
			GPIOA->BRR = 1 << 0;	\
		}

#define CH0_R(mask)	{	\
			GPIOB->BSRR = 1 << 0;	\
			if (GPIOA->IDR &(1 << 6)){	\
				ret |= mask;	\
			}	\
			GPIOB->BRR = 1 << 0;	\
		}
#define CH1_R(mask)	{	\
			GPIOA->BSRR = 1 << 0;	\
			if (GPIOC->IDR &(1 <<14)){	\
				ret |= mask;	\
			}	\
			GPIOA->BRR = 1 << 0;	\
		}
#define CH0_W(mask)	{	\
			if (val & mask){	\
				GPIOA->BSRR = 1 << 7;	\
			}else{	\
				GPIOA->BRR = 1 << 7;	\
			}	\
			GPIOB->BSRR = 1 << 0;	\
			GPIOB->BRR = 1 << 0;	\
		}

#define CH1_W(mask)	{	\
			if (val & mask){	\
				GPIOC->BSRR = 1 << 15;	\
			}else{	\
				GPIOC->BRR = 1 << 15;	\
			}	\
			GPIOA->BSRR = 1 << 0;	\
			GPIOA->BRR = 1 << 0;	\
		}

extern "C" void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		radiochip.interrupt();
	}
}

void Dev_Nrf24_Delay(void)
{
	__NOP();
    __NOP();
	__NOP();
    //__NOP();
}

void Dev_Nrf24_Enable(t_bool en)
{
	if (en){
		GPIOB->BRR = 1 << 1;
	}else{
		GPIOB->BSRR = 1 << 1;
	}
}

t_u8 Dev_Nrf24_RW(t_u8 val)
{
	t_u8 ret = 0;

	CH0_RW(0x80);
	CH0_RW(0x40);
	CH0_RW(0x20);
	CH0_RW(0x10);
	CH0_RW(0x08);
	CH0_RW(0x04);
	CH0_RW(0x02);
	CH0_RW(0x01);
	return ret;
}
void Dev_Nrf24_W(t_u8 val)
{
	CH0_W(0x80);
	CH0_W(0x40);
	CH0_W(0x20);
	CH0_W(0x10);
	CH0_W(0x08);
	CH0_W(0x04);
	CH0_W(0x02);
	CH0_W(0x01);
}
t_u8 Dev_Nrf24_R(void)
{
	t_u8 ret = 0;	
	GPIOA->BRR = 1 << 7;
	CH0_R(0x80);
	CH0_R(0x40);
	CH0_R(0x20);
	CH0_R(0x10);
	CH0_R(0x08);
	CH0_R(0x04);
	CH0_R(0x02);
	CH0_R(0x01);
	return ret;
}
void Dev_Nrf24_CE(t_bool bHigh)
{
	if (bHigh){
		GPIOB->BSRR = 1 << 10;
	}else{
		GPIOB->BRR  = 1 << 10;
	}
}

void Dev_Nrf24_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	//Test
	Dev_Gpio_Init(GPIOB, 12, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//Power GND
	GPIOB->BRR = 1 << 2;
	Dev_Gpio_Init(GPIOB, 2, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//Power VCC
	GPIOB->BSRR= 1 << 11;
	Dev_Gpio_Init(GPIOB,11, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//IRQ
	Dev_Gpio_Init(GPIOA, 5, GPIO_AF_0, GPIO_OType_OD, GPIO_Mode_IN);
	
	//NRF24L CE
	GPIOB->BRR = 1 << 10;
	Dev_Gpio_Init(GPIOB,10, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//NRF24L SPI CS
	GPIOB->BSRR = 1 << 1;
	Dev_Gpio_Init(GPIOB, 1, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//NRF24L SPI SCK
	GPIOB->BRR = 1 << 0;
	Dev_Gpio_Init(GPIOB, 0, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//NRF24L SPI MOSI
	GPIOA->BRR = 1 << 7;
	Dev_Gpio_Init(GPIOA, 7, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_OUT);
	
	//NRF24L SPI MISO
	Dev_Gpio_Init(GPIOA, 6, GPIO_AF_0, GPIO_OType_PP, GPIO_Mode_IN);

	/* Connect EXTI9 Line to PC9 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);

	/* Configure EXTI9 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void Dev_Nrf24_Test(t_bool bHigh)
{
	if (bHigh){
		GPIOB->BSRR = 1 << 12;
	}else{
		GPIOB->BRR  = 1 << 12;
	}
}

#include <stdarg.h>
#include <string.h>

#include "CDebugLog.h"
#include "CSystem_Uart.h"
#include "stm32f0xx.h"

#define T_MASK			(USART_CR1_TE | USART_CR1_TCIE)


t_bool Dev_SBUS_Irq(void);
/**
  * @brief  This function handles USART4 global interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void USART1_IRQHandler(t_u16 id)
{
	t_u8 nChar;
	if (USART1->ISR & USART1->CR1 & USART_ISR_TC)
	{
		/* Write one byte to the transmit data register */
		if (dlog.get_char(nChar))
		{
			USART1->TDR = nChar;
		}
		else
		{
            USART1->ICR = USART_ISR_RXNE | USART_ISR_TC;
            USART1->CR1 = USART1->CR1 & (~T_MASK);
		}
	}
}
//////////////////////////////////////////////////////////////////
void Dev_Uart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 3);

	USART_InitStructure.USART_BaudRate = 921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* ?5?:?? USART, ???? */
	USART_Cmd(USART1, ENABLE);
}
void Dev_Uart_Request(void)
{
	USART1->CR1 |= T_MASK;
}

#include <stdarg.h>
#include <string.h>

//#include "CDebugLog.h"
#include "hal_local.h"
#include "hal_uart.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"


#define T_MASK			(USART_CR1_TE | USART_CR1_TCIE)

static UART_HandleTypeDef UartHandle;
static t_hal_uart_fn m_uart_getchar = m_null;

/**
  * @brief  This function handles USART4 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	t_u8 nChar;
	if (((USART1->ISR & USART_ISR_TC) != RESET) && 
		((USART1->CR1 & USART_CR1_TCIE) != RESET))
	{
		if (m_uart_getchar && 
			m_uart_getchar(&nChar))
		{
			USART1->TDR = nChar;
		}
		else
		{
			USART1->CR1 &= ~T_MASK;
		}
	}
}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/* Enable USART2 clock */
	__HAL_RCC_USART1_CLK_ENABLE(); 

	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_9;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_USART1;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*##-3- Configure the NVIC for UART ########################################*/
	/* NVIC for USART1 */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	/*##-1- Reset peripherals ##################################################*/
	__HAL_RCC_USART1_FORCE_RESET();
	__HAL_RCC_USART1_RELEASE_RESET();

	/*##-2- Disable peripherals and GPIO Clocks #################################*/
	/* Configure UART Tx as alternate function  */
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

	/*##-3- Disable the NVIC for UART ##########################################*/
	HAL_NVIC_DisableIRQ(USART1_IRQn);
}
//////////////////////////////////////////////////////////////////
void hal_uart_init(void)
{
	UartHandle.Instance        = USART1;
	UartHandle.Init.BaudRate   = 921600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX;

	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
	}
}

void hal_uart_request(void)
{
	USART1->CR1 |= T_MASK;
}

t_void hal_uart_callback(t_hal_uart_fn pFn)
{
	m_uart_getchar = pFn;
}


#include "hal_local.h"
#include "hal_time.h"
#include "hal_spi.h"
#include "hal_led.h"
#include "hal_uart.h"
#include "hal_radio.h"
#include "hal_key.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

static t_u32  m_lock_cnt = 0;

/************************************************************************
**
************************************************************************/
t_void  hal_init_system_clock(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is 
	clocked below the maximum system frequency, to update the voltage scaling value 
	regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
	RCC_OscInitStruct.HSICalibrationValue = 0x11;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);  

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}
/************************************************************************
**
************************************************************************/
t_void hal_init(void)
{
	HAL_Init();
	
	/* Configure the system clock to 32 Mhz */
	hal_init_system_clock(); 
	
	hal_led_init();
	hal_spi_init();
	hal_uart_init();
	hal_radio_init();
	hal_dly_init();
	hal_key_init();
	
//	Dev_Delay_init();
//	Dev_Uart_init();
//	Dev_Nrf24_init();
	
//	dlog.print("Hello");
}
/************************************************************************
**
************************************************************************/
t_void hal_standby(void)
{
	PWR->CR &= ~6;
	__WFI();
}
/************************************************************************
**
************************************************************************/
t_void hal_unlock(void)
{
	if (!m_lock_cnt || !(--m_lock_cnt))
	{
		__enable_irq();
	}
}
/************************************************************************
**
************************************************************************/
t_void hal_lock(void)
{
	__disable_irq();
	m_lock_cnt++;
}
/************************************************************************
**
************************************************************************/
t_void hal_read_uid(t_u8 *uid, t_u8 len)
{
	memcpy(uid, (t_u8*)0x1FF80050, len);
}
/************************************************************************
**
************************************************************************/
t_void hal_system_reset(void)
{
	HAL_NVIC_SystemReset();
}

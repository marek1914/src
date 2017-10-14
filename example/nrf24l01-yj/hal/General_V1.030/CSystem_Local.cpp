#include "string.h"
#include "CSystem_Local.h"
#include "CSystem_Delay.h"
#include "CSystem_Uart.h"
#include "CSystem_Nrf24.h"
#include "stm32f0xx.h"

CSystem_Local _sys_;
CSystem& sys = _sys_;

/******************************************************************************/


/************************************************************************/
/*                                                                      */
/************************************************************************/
CSystem_Local::CSystem_Local(void)
{
	m_lock_cnt = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CSystem_Local::~CSystem_Local(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::init(void)
{
	RCC_ClearFlag();

    //FLASH read Protection
#if defined(BSTONE_BOOT)
	if(SET != FLASH_OB_GetRDP())
    {
        FLASH_Unlock();
        FLASH_OB_Unlock();
        FLASH_OB_RDPConfig(OB_RDP_Level_1);
        FLASH_OB_Launch();
        FLASH_OB_Lock();
        FLASH_Lock();
    }
#endif
    /* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* GPIOx clock enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA |
						   RCC_AHBPeriph_GPIOB |
						   RCC_AHBPeriph_GPIOC |
						   RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	
	Dev_Delay_init();
	Dev_Uart_init();
	Dev_Nrf24_init();
}
/*************************************************************************
**
*************************************************************************/
void CSystem_Local::StateInit(void)
{
}
/*************************************************************************
**
*************************************************************************/
void CSystem_Local::StateEntry(t_s32 evt)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::standby(void)
{
    PWR->CR &= ~6;
    __WFI();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::unlock(void)
{
	if (!m_lock_cnt || !(--m_lock_cnt))
	{
		__enable_irq();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::lock(void)
{
	__disable_irq();
	m_lock_cnt++;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_tick CSystem_Local::get_ms(void)
{
	return Dev_Delay_GetMs();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_u16 CSystem_Local::get_us_16(void)
{
	return Dev_Delay_GetUs();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::dlog_request(void)
{
	Dev_Uart_Request();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::us_dly_start(t_u16 us)
{
	Dev_Delay_Timout_Start(us);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::us_dly_stop(void)
{
	Dev_Delay_Timout_Stop();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::us_dly_next(t_u16 us)
{
	Dev_Delay_Timout_Next(us);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSystem_Local::nrf24_enable(t_bool en)
{
	Dev_Nrf24_Enable(en);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_u8 CSystem_Local::nrf24_rw(t_u8 val)
{
	return Dev_Nrf24_RW(val);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSystem_Local::nrf24_w(t_u8 val)
{
	Dev_Nrf24_W(val);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_u8 CSystem_Local::nrf24_r(void)
{
	return Dev_Nrf24_R();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSystem_Local::nrf24_ce(t_bool bHigh)
{
	Dev_Nrf24_CE(bHigh);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void CSystem_Local::test(t_bool bHigh)
{
	Dev_Nrf24_Test(bHigh);
}
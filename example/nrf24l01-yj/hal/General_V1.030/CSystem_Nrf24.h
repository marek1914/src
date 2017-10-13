#ifndef __C_BSTONE_HAL_NRF24_H
#define __C_BSTONE_HAL_NRF24_H

void Dev_Nrf24_init(void);
void Dev_Nrf24_Enable(t_bool en);
t_u8 Dev_Nrf24_RW(t_u8 val);
void Dev_Nrf24_W(t_u8 val);
t_u8 Dev_Nrf24_R(void);
void Dev_Nrf24_CE(t_bool bHigh);
void Dev_Nrf24_Test(t_bool bHigh);

#endif	   


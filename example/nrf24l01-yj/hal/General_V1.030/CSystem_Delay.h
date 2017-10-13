#ifndef __C_BSTONE_SYSTEM_HAL_DELAY_H
#define __C_BSTONE_SYSTEM_HAL_DELAY_H

extern void Dev_Delay_init(void);
extern void Dev_Delay_Timout_Start(t_u16 us);
extern void Dev_Delay_Timout_Stop(void);
extern void Dev_Delay_Timout_Next(t_u16 us);

extern t_vu32 msTimingTick;

#define Dev_Delay_GetMs()	msTimingTick
#define Dev_Delay_GetUs()	TIM14->CNT

#endif	   


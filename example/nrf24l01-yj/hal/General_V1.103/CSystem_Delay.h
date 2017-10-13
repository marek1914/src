#ifndef __C_BSTONE_SYSTEM_HAL_DELAY_H
#define __C_BSTONE_SYSTEM_HAL_DELAY_H

extern t_vu32 msTimingTick;

extern void Dev_Delay_init(void);
extern void Dev_Delay_Delay(t_u16 nms);
extern void Dev_Delay_Timout_Start(t_u16 us);
extern void Dev_Delay_Timout_Stop(void);
extern void Dev_Delay_Timout_Next(t_u16 us);
extern void Dev_Delay_Timout_Sync(t_u16 us);

#define Dev_Delay_GetMs()	msTimingTick
#define Dev_Delay_GetUs()	TIM3->CNT

#endif	   


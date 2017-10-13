#ifndef __C_BSTONE_SYSTEM_GPIO_H
#define __C_BSTONE_SYSTEM_GPIO_H


#include "stm32f0xx.h"

void Dev_Gpio_Init(GPIO_TypeDef* GPIOx,t_u16 pin, t_u16 AFidx, t_u16 OType, GPIOMode_TypeDef mode);

#endif	   


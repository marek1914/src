#include <stdarg.h>
#include <string.h>

#include "CTypes.h"
#include "CSystem_Gpio.h"

#include "stm32f0xx.h"


void Dev_Gpio_Init(GPIO_TypeDef* GPIOx,t_u16 pin, t_u16 AFidx, t_u16 OType, GPIOMode_TypeDef mode)
{
	#define PNUM	pin
	GPIOx->OSPEEDR   |= (GPIO_Speed_Level_3 << (PNUM * 2));
	GPIOx->MODER      = (GPIOx->MODER  & (~(GPIO_MODER_MODER0  << (PNUM * 2)))) | (mode             << (PNUM * 2));
	GPIOx->OTYPER     = (GPIOx->OTYPER & (~(GPIO_OTYPER_OT_0   << (PNUM))))     | (OType		    << (PNUM));
	GPIOx->PUPDR      = (GPIOx->PUPDR  & (~(GPIO_PUPDR_PUPDR0  << (PNUM))))     | (GPIO_PuPd_NOPULL << (PNUM));
	if (PNUM >= 8){
		GPIOx->AFR[1]= (GPIOx->AFR[1] & (~(0xF<<(4*(PNUM-8))))) | (AFidx<<(4*(PNUM-8)));
	}else{
		GPIOx->AFR[0]= (GPIOx->AFR[0] & (~(0xF<<(4*(PNUM-0))))) | (AFidx<<(4*(PNUM-0)));
	}
	#undef PNUM
}


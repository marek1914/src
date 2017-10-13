#include <stdarg.h>
#include <string.h>

//#include "CDebugLog.h"
#include "hal_local.h"
#include "hal_uart.h"

#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

#define H_FLASH_BASE		(0x08000000)
#define H_FLASH_PAGESIZE	(128)
#define H_FLASH_SIZE		(512*H_FLASH_PAGESIZE)
#define H_PARAM_BASE		(H_FLASH_BASE + H_FLASH_SIZE - H_FLASH_PAGESIZE)


t_void hal_param_load(void* pbuf, t_u8 len)
{
	memcpy(pbuf, (void*)H_PARAM_BASE, len);
}

t_void hal_param_save(void* pbuf, t_u8 len)
{
	t_u8* pByte = (t_u8*)pbuf;
	uint32_t PageError = 0, offset=0, temp;
	FLASH_EraseInitTypeDef EraseInitStruct;
	
	HAL_FLASH_Unlock();
	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = H_PARAM_BASE;
	EraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		HAL_FLASH_Lock(); 
		return;
	}
	
	for (offset=0; offset < len; offset+=4, pByte+=4)
	{
		temp =  ((t_u32)pByte[0] << 0) |
				((t_u32)pByte[1] << 8) |
				((t_u32)pByte[2] <<16) |
				((t_u32)pByte[3] <<24);
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, H_PARAM_BASE + offset, temp) != HAL_OK)
		{
			HAL_FLASH_Lock(); 
			return;
		}
	}
	HAL_FLASH_Lock(); 
}

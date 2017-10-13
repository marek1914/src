#include "hal_local.h"
#include "pub_protocol.h"
#include "pub_param.h"

#define MASK_CLEAR	0x01

static t_u8  key_clear = 0;
static t_u8  key_val = 0;
static t_u8  key_last = 0;
static t_u32 key_msec = 0;
static t_u32 key_dsec = 0;
static t_u8  key_reset = 0;
	
void dev_key_init(void)
{
}

void dev_key_task(void)
{
	t_u8 val = hal_key_mask();
	if (val != key_last ||
		val == key_val)
	{
		key_msec = hal_get_ms_32()+50;
	}
	
	if (val != key_val && hal_get_ms_32() > key_msec)
	{
		key_val = val;
		
		if (val & MASK_CLEAR){
			hal_led_onoff(0,1);
			key_clear = 1;
			key_dsec = hal_get_ms_32()+2000;
		}else{
			hal_led_onoff(0,0);
			key_clear = 0;
			if (key_reset)
			{
				hal_system_reset();
			}
		}
	}
	
	if (key_clear & hal_get_ms_32() > key_dsec)
	{
		hal_led_onoff(0,0);
		key_reset = 1;
		key_clear = 0;
#ifdef SLAVE
		param.host_addr = 0xFFFFFFFF;
#else
		memset(param.dev_addr,0xFF,sizeof(param.dev_addr));
#endif
		pub_param_save();
	}
	key_last = val;
}

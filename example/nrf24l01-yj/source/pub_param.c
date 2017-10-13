#include "hal_local.h"
#include "pub_protocol.h"
#include "pub_param.h"

#define FLAG_MASK	0xAB551234

sParam param;

t_u32 pub_make_sum(void *pbuf, t_u8 len)
{
	t_u32 temp,sum = 0;
	t_u8 i,* pByte = (t_u8*)pbuf;
	for(i = 0; i < len; i+=4)
	{
		temp =  ((t_u32)pByte[0] << 0) |
				((t_u32)pByte[1] << 8) |
				((t_u32)pByte[2] <<16) |
				((t_u32)pByte[3] <<24);
		sum ^= temp;
	}
	return ~sum;
}

void pub_param_init(void)
{
	hal_param_load(&param,sizeof(param));
	if (param.flag != FLAG_MASK||
		param.sum  != pub_make_sum(&param,sizeof(param)))
	{
#ifdef SLAVE
		param.dev_addr 	= 0x00AA0001;
		param.host_addr = 0xFFFFFFFF;
#else
		param.host_addr = 0xAABB0001;
		memset(param.dev_addr,0xFF,sizeof(param.dev_addr));
#endif
		pub_param_save();
	}
}

void pub_param_save(void)
{
	param.flag 		= FLAG_MASK;
	param.version	= 1;
	param.sum  		= pub_make_sum(&param,sizeof(param));
	hal_param_save(&param,sizeof(param));
}

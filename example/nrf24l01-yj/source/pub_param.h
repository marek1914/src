#ifndef __C_PUBLIC_PARAM_H
#define __C_PUBLIC_PARAM_H

typedef struct{
	t_u32 flag;
	t_u8  version;
	t_u32 sum;
#ifdef SLAVE
	t_u32 dev_addr;
	t_u32 host_addr;
#else
	t_u32 host_addr;
	t_u32 dev_addr[SLOT_NUM];
#endif
}sParam;

extern void pub_param_init(void);
extern void pub_param_save(void);

extern sParam param;

#endif


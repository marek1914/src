#ifndef __C_BSTONE_SYSTEM_LOCAL_H
#define __C_BSTONE_SYSTEM_LOCAL_H

#include "CTypes.h"
#include "CSystem.h"

class CSystem_Local : public CSystem
{
public:
	CSystem_Local(void);
	~CSystem_Local(void);
public:
	virtual void StateInit(void){};
	virtual void StateEntry(t_s32 evt){};
private:
	t_u32  m_lock_cnt;
public:
	virtual t_void init(void);
	virtual t_void standby(void);
	virtual t_void unlock(void);
	virtual t_void lock(void);
		
	virtual t_tick get_ms(void);
	virtual t_u16  get_us_16(void);

	virtual t_void dlog_request(void);
	
	virtual t_void us_dly_start(t_u16 us);
	virtual t_void us_dly_stop(void);
	virtual t_void us_dly_next(t_u16 us);
	virtual t_void us_dly_sync(t_u16 us);

	virtual void nrf24_enable(t_bool en);
	virtual t_u8 nrf24_rw(t_u8 val);
	virtual void nrf24_w(t_u8 val);
	virtual t_u8 nrf24_r(void);
	virtual void nrf24_ce(t_bool bHigh);

	virtual t_void test(t_bool bHigh);
};

#endif



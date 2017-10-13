#include "hal_local.h"
#include "stk_slave.h"
#include "rad_include.h"
#include "pub_protocol.h"
#include "pub_math.h"
#include "pub_param.h"

enum{
	eST_INIT   = 0,
	eST_BOARDCAST_INIT,
	eST_BOARDCAST_TIMOUT,
	eST_TRANSFER_INIT,
	eST_TRANSFER_WAIT
};

static t_u8			m_state;
static t_u32		m_host_addr;
static t_u8			m_freq_table[FREQ_NUM];
static t_u8			m_freq_didx;
static t_u8			m_freq_lidx;
static t_u8         m_freq_cnt;
static t_u8			m_error_cnt;
static t_d2h_sync 	m_d2h;

/************************************************************************/
/*                                                                      */
/************************************************************************/
static t_u8 cycle_increase(t_u8 cur, t_u8 max)
{
	if ((++cur)>=max){
		cur = 0;
	}
	return cur;
}
/************************************************************************
**
************************************************************************/
t_void stk_slave_timout(void)
{
	switch(m_state)
	{
	case eST_INIT://初始化
		hal_dly_next(32000);	
		m_state = eST_BOARDCAST_INIT;
		rad_rx_test(m_true);
		break;
	/*********************************************************/
	case eST_BOARDCAST_INIT:	//准备发送广播
		hal_dly_next(1500);
		m_state = eST_BOARDCAST_TIMOUT;
		
		//准备广播数据
		m_d2h.pak_type = ePType_Broadcast;
		m_d2h.udata.broadcast.host_addr = param.host_addr;
		
		//广播频率调整
		if ((++m_freq_cnt)>=9)
		{
			m_freq_cnt = 0;
			m_freq_lidx = cycle_increase(m_freq_lidx, b_arraysize(freq_listen));
		}
		//发送广播
		rad_set_freq(freq_listen[m_freq_lidx]);
		rad_set_tx_mode(addr_boardcast, (t_u8*)&m_d2h, sizeof(m_d2h));
		rad_enable_send();
		break;
	case eST_BOARDCAST_TIMOUT://关闭无线
		hal_dly_next(5000);
		m_state = eST_BOARDCAST_INIT;
	
		rad_disable();
		break;
	/*********************************************************/
	case eST_TRANSFER_INIT:
		hal_dly_next(SLOT_US*3/2);
		m_state = eST_TRANSFER_WAIT;
		
		hal_led_onoff(1,1);
		m_freq_didx = cycle_increase(m_freq_didx, FREQ_NUM);
		hal_led_onoff(1,0);
	
		rad_set_freq(m_freq_table[m_freq_didx]);
		rad_set_rx_mode(m_d2h.dev_addr);	
		break;
	case eST_TRANSFER_WAIT:
		hal_dly_next(CYCLE_US - SLOT_US*3/2);
		m_state = eST_TRANSFER_INIT;

		rad_disable();
		if ((++m_error_cnt)>100)
		{
			m_state = eST_BOARDCAST_INIT;
		}
		break;
	}
}
/************************************************************************
**
************************************************************************/
t_void stk_slave_send(void)
{
	if (m_state == eST_BOARDCAST_TIMOUT)
	{
		rad_set_rx_mode(m_d2h.dev_addr);
	}
	else
	{
		rad_disable();
	}
}
/************************************************************************
**
************************************************************************/
t_void stk_slave_revcive(void)
{
	t_bool bSendPayload;
	t_h2d_sync* h2d;
	
	if (m_state == eST_BOARDCAST_TIMOUT ||
		m_state == eST_TRANSFER_WAIT ||
		param.host_addr == 0xFFFFFFFF ||
		param.host_addr == h2d->hot_addr)
	{
		if (m_state == eST_TRANSFER_WAIT){
			bSendPayload = m_true;
			hal_dly_sync(CYCLE_US - SLOT_US/2);
		}else{
			bSendPayload = m_false;
			hal_dly_sync(CYCLE_US - SLOT_US);
		}
		m_state = eST_TRANSFER_INIT;
		
		h2d = (t_h2d_sync*)rad_rx_buffer();	
		
		memcpy(m_freq_table,h2d->frq_table, FREQ_NUM);
		
		m_error_cnt = 0;
		m_host_addr = h2d->hot_addr;
		m_freq_didx = h2d->frq_idx;
		
		//发送回复数据
		if (bSendPayload)
		{
			m_d2h.pak_type = ePType_Payload;
			m_d2h.udata.payload.idx = h2d->dat_idx;
			m_d2h.udata.payload.data[0] = 0;
			m_d2h.udata.payload.data[1] = 0;
			m_d2h.udata.payload.data[2] = 0;
			m_d2h.udata.payload.data[3] = 0;
			
			rad_set_tx_mode(m_host_addr, (t_u8*)&m_d2h, sizeof(m_d2h));
			rad_enable_send();
		}
		else if (param.host_addr == 0xFFFFFFFF)
		{
			param.host_addr = h2d->hot_addr;
			pub_param_save();
		}
	}
	hal_led_onoff(1,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
t_void stk_slave_init(void)
{	
	m_freq_didx 	= 0;
	m_freq_lidx 	= 0;
	m_freq_cnt		= 0;
	m_error_cnt		= 0;
	
	m_state = eST_INIT;
	
	m_d2h.dev_addr = param.dev_addr;
	
	hal_dly_sync(32000);
	
	rad_set_send_fn(stk_slave_send);
	rad_set_timeout_fn(stk_slave_timout);
	rad_set_receivce_fn(stk_slave_revcive);
}


#include "hal_local.h"
#include "stk_master.h"
#include "rad_include.h"
#include "pub_protocol.h"
#include "pub_math.h"
#include "pub_param.h"
#include "dbg_log.h"

enum{
	eST_TRANSFER = 0,
	eST_LISTEN,
	eST_IDLE,
};
typedef struct{
	t_u32 addr;
	t_u8  state;
	t_u32 timms;
}sDevState;

static t_u8		 m_state;
static t_u8		 m_slot_idx;
static t_u8		 m_freq_lidx;
static t_u8		 m_freq_didx;
static t_u8		 m_send_revc;
static sDevState m_device[SLOT_NUM];

t_h2d_sync m_h2d;


/************************************************************************
**
************************************************************************/
static t_u8 cycle_increase(t_u8 cur, t_u8 max)
{
	if ((++cur)>=max)
	{
		cur = 0;
	}
	return cur;
}
/************************************************************************
**
************************************************************************/
static t_void stk_master_send_h2d(void)
{
	rad_set_tx_mode(m_device[m_slot_idx].addr, (t_u8*)&m_h2d, sizeof(m_h2d));
	rad_enable_send();
}
/************************************************************************
**
************************************************************************/
t_void stk_master_timout(void)
{
	switch(m_state)
	{
	case eST_TRANSFER:	
		//下一个时间槽
		m_slot_idx = cycle_increase(m_slot_idx, SLOT_NUM);
		
		//调频
		if (m_slot_idx == 0)
		{
			m_freq_lidx = cycle_increase(m_freq_lidx, b_arraysize(freq_listen));
			m_freq_didx = cycle_increase(m_freq_didx, FREQ_NUM);
			
			hal_led_onoff(1,1);
			hal_led_onoff(1,0);
		}
		else
		{
			hal_led_onoff(1,1);
			hal_radio_enable_delay();
			hal_led_onoff(1,0);
		}
		
		//准备要发送的数据
		m_h2d.frq_idx = m_freq_didx;
		
		//接收广播或与从设备通讯
		if (m_device[m_slot_idx].state &&
			m_device[m_slot_idx].timms > hal_get_ms_32())
		{	//时间槽有设备
			hal_dly_next(SLOT_US);
			m_send_revc = m_true;
			
			rad_set_freq (m_h2d.frq_table[m_freq_didx]);
			
			stk_master_send_h2d();
		}
		else
		{	//监听这个时间
			hal_dly_next(SLOT_US/2);
			m_state = eST_LISTEN;
			m_send_revc = m_false;
			
			m_device[m_slot_idx].state = 0;
			m_device[m_slot_idx].addr  = 0;
			m_device[m_slot_idx].timms = 0;
	
			rad_set_freq   (freq_listen[m_freq_lidx]);
			rad_set_rx_mode(addr_boardcast);
		}
		break;
	case eST_LISTEN:
		hal_dly_next(SLOT_US/2);
		m_state = eST_TRANSFER;
	
		if (m_device[m_slot_idx].state){
			stk_master_send_h2d();
		}else{
			rad_disable();
		}
		break;
	}
}
/************************************************************************
**
************************************************************************/
t_void stk_master_send(void)
{
	if (m_state == eST_TRANSFER && m_send_revc)
	{
		rad_set_rx_mode(m_h2d.hot_addr);
	}
	else
	{
		rad_disable();
	}
}
/************************************************************************
**
************************************************************************/
t_void stk_master_revcive(void)
{
	t_u8 i,nul_idx, sav_idx;
	t_d2h_sync *d2h = (t_d2h_sync*)rad_rx_buffer();
	if (m_state == eST_TRANSFER)
	{
		if (d2h->pak_type == ePType_Payload)
		{//接收到有效数据
			m_h2d.dat_idx = d2h->udata.payload.idx + 1;
			
			m_device[m_slot_idx].timms = hal_get_ms_32() + 1000;
		}
		
		rad_disable();
	}
	else if (m_state == eST_LISTEN)
	{
		if (d2h->pak_type == ePType_Broadcast)
		{
			//检查是否已经重复
			nul_idx = SLOT_NUM;
			sav_idx = SLOT_NUM;
			for (i = 0; i < SLOT_NUM; i++)
			{
				if (m_device[i].addr == d2h->dev_addr)
				{
					m_device[i].addr  = 0;
					m_device[i].state = 0;
					m_device[i].timms = 0;
				}
				if (param.dev_addr[i] == d2h->dev_addr)
				{
					sav_idx = i;
				}
				else if (param.dev_addr[i] == 0xFFFFFFFF)
				{
					nul_idx = i;
				}
			}
			if (nul_idx < SLOT_NUM || sav_idx < SLOT_NUM)
			{
				if ((d2h->udata.broadcast.host_addr == 0xFFFFFFFF) ||
					(d2h->udata.broadcast.host_addr == m_h2d.hot_addr && sav_idx < SLOT_NUM))
				{
					hal_led_onoff(1,1);
					m_device[m_slot_idx].addr  = d2h->dev_addr;
					m_device[m_slot_idx].state = 1;
					m_device[m_slot_idx].timms = hal_get_ms_32() + 1000;
					hal_led_onoff(1,0);
					
					dbg_print('L',d2h->dev_addr);
					
					if (sav_idx >= SLOT_NUM){
						param.dev_addr[nul_idx] = d2h->dev_addr;
						pub_param_save();
					}
				}
			}
		}
	}
	hal_led_onoff(1,1);
	hal_led_onoff(1,0);
}
/************************************************************************
**
************************************************************************/
t_void stk_master_make_freqtable(void)
{
	t_u32 sead = 0;
	t_u8 i,j,f,uuid[12];
	
	hal_read_uid(uuid,12);
	for(i=0;i<12;i++){
		sead += uuid[i];
	}
	srand(sead);
	for(i=0;i<FREQ_NUM;i++)
	{
		do{
			f = 1 + rand()%rad_freq_num();
			for (j=0;j<sizeof(freq_listen);j++)
			{
				if (f==freq_listen[j]){
					break;
				}
			}
		}while(j<sizeof(freq_listen));
		
		m_h2d.frq_table[i] = f;
	}
}
/************************************************************************
**
************************************************************************/
t_void stk_master_init(void)
{
	rad_set_send_fn(stk_master_send);
	rad_set_timeout_fn(stk_master_timout);
	rad_set_receivce_fn(stk_master_revcive);
	
	memset(&m_device,0,sizeof(m_device));
	memset(&m_h2d,   0,sizeof(m_h2d));
	
	m_state		= eST_TRANSFER;
	m_slot_idx  = 0;
	m_freq_didx = 0;
	m_freq_lidx = 0;
	
	stk_master_make_freqtable();
	
	m_h2d.hot_addr = param.host_addr;
	
	hal_dly_sync(32000);
	rad_rx_test(m_true);
	
	
}

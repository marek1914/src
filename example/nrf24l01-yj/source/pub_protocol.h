#ifndef __C_BSTONE_PROTOCOL_H
#define __C_BSTONE_PROTOCOL_H

/*************************************************************
|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |
| 监听| 同步|  D0 |  D1 |  D2 |  D3 |  D4 |  D5 |  D6 |  D7 |
| 监听频率  |  数据通讯频率                                 |
**************************************************************
|------------- Slot -------------|
| 100us |      400us             |
**************************************************************
1 监听开始需要600us调频时间
2 数据通讯前需要600us调频时间
3 NRF24L01 500us最多处理2个包，此时利用了FIFO，
  600us处理两个包，从波形看上，接收端没有使用FIFO,一个一个接受处理
**************************************************************/
#define ADRR_WIDTH   4 
#define PLOAD_WIDTH	 sizeof(t_d2h_sync)

#define FREQ_NUM	6
#define SLOT_NUM	8
#define SLOT_US		1000//1250
#define CYCLE_US	(SLOT_US*SLOT_NUM)

/**************************************************/

typedef enum{
	ePType_Broadcast = 0,
	ePType_Payload
}ePakType;

static t_u32 addr_boardcast = 0xFFFFFF01; 
static t_u8  freq_listen[3] = {1, 3, 5};

#pragma pack(push)
#pragma pack(1)
typedef struct{
	t_u32 hot_addr;			//主设备地址
	t_u8  dat_idx;
	t_u8  frq_idx;				//当前频段索引
	t_u8  frq_table[FREQ_NUM];	//频率表
}t_h2d_sync;

typedef struct{
	t_u32 dev_addr;
	t_u8  pak_type;				//0:广播 1:payload
	union{
		struct{
			t_u8  idx;			//数据包ID
			t_u8  data[20];		//有效数据
		}payload;
		struct{
			t_u32 host_addr;
		}broadcast;
	}udata;
}t_d2h_sync;
#pragma pack(pop)

#endif

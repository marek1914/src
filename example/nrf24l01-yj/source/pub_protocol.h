#ifndef __C_BSTONE_PROTOCOL_H
#define __C_BSTONE_PROTOCOL_H

/*************************************************************
|  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |
| ����| ͬ��|  D0 |  D1 |  D2 |  D3 |  D4 |  D5 |  D6 |  D7 |
| ����Ƶ��  |  ����ͨѶƵ��                                 |
**************************************************************
|------------- Slot -------------|
| 100us |      400us             |
**************************************************************
1 ������ʼ��Ҫ600us��Ƶʱ��
2 ����ͨѶǰ��Ҫ600us��Ƶʱ��
3 NRF24L01 500us��ദ��2��������ʱ������FIFO��
  600us�������������Ӳ��ο��ϣ����ն�û��ʹ��FIFO,һ��һ�����ܴ���
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
	t_u32 hot_addr;			//���豸��ַ
	t_u8  dat_idx;
	t_u8  frq_idx;				//��ǰƵ������
	t_u8  frq_table[FREQ_NUM];	//Ƶ�ʱ�
}t_h2d_sync;

typedef struct{
	t_u32 dev_addr;
	t_u8  pak_type;				//0:�㲥 1:payload
	union{
		struct{
			t_u8  idx;			//���ݰ�ID
			t_u8  data[20];		//��Ч����
		}payload;
		struct{
			t_u32 host_addr;
		}broadcast;
	}udata;
}t_d2h_sync;
#pragma pack(pop)

#endif

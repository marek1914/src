/********************************************************
�ļ����ƣ�User_InitSys.h
���ߣ�������
�汾��20131104
˵�������ļ�ΪUser_InitSys��ͷ�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#ifndef USER_INITSYS_H
#define USER_INITSYS_H

#include <string.h>

#include "User_GlobalVar.h"			//�û������ȫ�ֱ���
#include "DSP28x_Project.h"			// Device Headerfile and Examples Include File
#include "User_MacroCfg.h"
#include "User_Flash.h"

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------��Ҫ�޸Ĳ���----------------------------------------------

//�ڵ�����(1~32)
#define CANMSGTOTLALCNT 32 								//����������

//����ID����
#define CANARXMSGCNT 	1								//���ڵ�ʹ�õ�CANA��Ϣ�������(����)
#define CANBRXMSGCNT 	1 								//���ڵ�ʹ�õ�CANB��Ϣ�������(����)

//����������
#define CANARXMAILBOX	4								//CANA����ʹ����������(ʣ��ȫ��Ϊ����ʹ��)
#define CANBRXMAILBOX	4								//CANB����ʹ����������(ʣ��ȫ��Ϊ����ʹ��)

//������
#define CANABAUDRATE	B_250K 							//CANA������ 
#define CANBBAUDRATE	B_250K 							//CANB������     

//------UART--------
	//����
#define COMA	0 			//����A  
#define COMB	1 			//����B
#define COMC	2 			//����C 
	
	//����������
#define B2400			2400		//������2400
#define B4800			4800		//������4800
#define B7200			7200		//������7200
#define B9600			9600		//������9600
#define B19200			19200		//������19200
#define B38400			38400		//������38400
#define B57600			57600		//������57600
#define B115200			115200		//������115200
#define B230400			230400		//������230400
#define B460800			460800		//������460800
#define B921600			921600		//������921600
	
	//����λ����
#define BIT_1			0			//����λ1	
#define BIT_2			1			//����λ2
#define BIT_3			2			//����λ3
#define BIT_4			3			//����λ4
#define BIT_5			4			//����λ5		
#define BIT_6			5			//����λ6
#define BIT_7			6			//����λ7
#define BIT_8			7			//����λ8
	
	//ֹͣλ����
#define STOP_1			0			//1λֹͣλ
#define STOP_2			1			//2λֹͣλ
	
	//У����ʽ
#define P_NONE			0			//��У��
#define P_ODD			1			//��У��
#define P_EVEN			2			//żУ��



//--------------------------------����Ҫ�޸�-------------------------------------------
//���ݶ���

// ��Ϣ�����ʼ���ṹ��
typedef struct _CANMSG_INIT_STRU 
{
	U8	stu8_MsgNum;									//��Ϣ��(��Χ��1~32)
	U8	stu8_RdTxFlg;									//�շ�����(��Χ��'t' �� 'r')
	U16	stu16_MsgID;									//��Ϣ����ID(��Χ��0x0001��0x07fe)
	U8	stu8_DataLen;									//���ݳ���(��Χ��0~8)
}CANMSG_INIT_STRU,*PCANMSG_INIT_STRU;

//�ⲿ���õĲ���
//CANA
extern U16 gu16a_CANABuf[CANARXMSGCNT * 4];				//CAN��������ÿ��IDռ4��I16
extern I8 gi8a_CANART[CANARXMSGCNT];					//CAN���ա����ͱ�־��'r'���յ������ݣ�'t'�����ݴ�����
extern U16 gu16a_CANACount[CANARXMSGCNT];				//CAN���߽��ռ���
extern U16 gu16a_CANACntCache[CANARXMSGCNT];			//CAN���߽��ռ�������

extern U16	gu16_CANAImptIndex;							//CAN��Ҫ��������
extern U16 gu16a_CANAImptBuf[100];						//CAN��Ҫ���ݻ�����

extern U32 gu32_CANARevF;								//CAN���ձ�־
extern U8 gu8_CANARevIndex;								//CAN��������
extern U16 gu16a_CANADataBuf[160];						//CAN���н������ݻ�����

extern const U16 gia_CANARXCfg[];						//CAN����ID
extern const U16 gia_CANATXCfg[];						//CAN����ID

//CANB
extern U16 gu16a_CANBBuf[CANBRXMSGCNT * 4];				//CAN��������ÿ��IDռ4��I16
extern I8 gi8a_CANBRT[CANBRXMSGCNT];					//CAN���ա����ͱ�־��'r'���յ������ݣ�'t'�����ݴ�����
extern U16 gu16a_CANBCount[CANBRXMSGCNT];				//CAN���߽��ռ���
extern U16 gu16a_CANBCntCache[CANBRXMSGCNT];			//CAN���߽��ռ�������

extern U32 gu32_CANBRevF;								//CAN���ձ�־
extern U8 gu8_CANBRevIndex;								//CAN��������
extern U16 gu16a_CANBDataBuf[160];						//CAN���н������ݻ�����

extern const U16 gia_CANBRXCfg[];						//CAN����ID
extern const U16 gia_CANBTXCfg[];						//CAN����ID

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------��������----------------------------------------
void User_InitSysCtrl(void);			//ϵͳ��ʼ��
void User_InitCan(void);				//CAN��ʼ��
void User_InitTimer(void);				//��ʱ����ʼ��
void User_InitWatchdog(void);			//���Ź���ʼ��
void User_InitUart(U8 pu8_Com, U32 pu32_BBaudRate,U8 Parity, U8 pu8_DataBits, U8 pu8_StopBits);//���ڳ�ʼ��
void User_InitGpio(void);				//GPIO����
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_INITSYS_H definition


/***************************************************/
/* Bit configuration parameters for 100 MHz SYSCLKOUT*/
/***************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
---------------------------------------------------
BT = 10, TSEG1 = 6, TSEG2 = 1, Sampling Point = 80%
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 10 	: CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 20 	: CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 40 	: CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 80 	: CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 100 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 200 : CAN clock = 0.5 MHz

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 20:
---------------------------------------------------
BT = 20
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 5
500 kbps : BRP(reg)+1 = 10
250 kbps : BRP(reg)+1 = 20
125 kbps : BRP(reg)+1 = 40
100 kbps : BRP(reg)+1 = 50
50  kbps : BRP(reg)+1 = 100

The table below shows how to achieve different sampling points with a BT of 20:
------------------------------------------------------------
Achieving desired SP by changing TSEG1 & TSEG2 with BT = 20
------------------------------------------------------------

TSEG1(reg) = 15, TSEG2(reg) = 2, SP = 85%
TSEG1(reg) = 14, TSEG2(reg) = 3, SP = 80%
TSEG1(reg) = 13, TSEG2(reg) = 4, SP = 75%
TSEG1(reg) = 12, TSEG2(reg) = 5, SP = 70%
TSEG1(reg) = 11, TSEG2(reg) = 6, SP = 65%
TSEG1(reg) = 10, TSEG2(reg) = 7, SP = 60%

Note: BRP(reg), TSEG1(reg) & TSEG2(reg) indicate the actual value that is written
 into the bit fields of the CAN control registers. These values are increased by 1
 by the CAN module when these registers are accessed.

*/
//===========================================================================
// End of file.
//===========================================================================

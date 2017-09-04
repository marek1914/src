/********************************************************
文件名称：User_InitSys.h
作者：王京城
版本：20131104
说明：本文件为User_InitSys的头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#ifndef USER_INITSYS_H
#define USER_INITSYS_H

#include <string.h>

#include "User_GlobalVar.h"			//用户定义的全局变量
#include "DSP28x_Project.h"			// Device Headerfile and Examples Include File
#include "User_MacroCfg.h"
#include "User_Flash.h"

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------需要修改部分----------------------------------------------

//节点数量(1~32)
#define CANMSGTOTLALCNT 32 								//邮箱总数量

//接收ID数量
#define CANARXMSGCNT 	1								//本节点使用的CANA消息对象个数(接收)
#define CANBRXMSGCNT 	1 								//本节点使用的CANB消息对象个数(接收)

//发送邮箱数
#define CANARXMAILBOX	4								//CANA接收使用邮箱数量(剩余全部为发送使用)
#define CANBRXMAILBOX	4								//CANB接收使用邮箱数量(剩余全部为发送使用)

//波特率
#define CANABAUDRATE	B_250K 							//CANA波特率 
#define CANBBAUDRATE	B_250K 							//CANB波特率     

//------UART--------
	//串口
#define COMA	0 			//串口A  
#define COMB	1 			//串口B
#define COMC	2 			//串口C 
	
	//波特率设置
#define B2400			2400		//波特率2400
#define B4800			4800		//波特率4800
#define B7200			7200		//波特率7200
#define B9600			9600		//波特率9600
#define B19200			19200		//波特率19200
#define B38400			38400		//波特率38400
#define B57600			57600		//波特率57600
#define B115200			115200		//波特率115200
#define B230400			230400		//波特率230400
#define B460800			460800		//波特率460800
#define B921600			921600		//波特率921600
	
	//数据位长度
#define BIT_1			0			//数据位1	
#define BIT_2			1			//数据位2
#define BIT_3			2			//数据位3
#define BIT_4			3			//数据位4
#define BIT_5			4			//数据位5		
#define BIT_6			5			//数据位6
#define BIT_7			6			//数据位7
#define BIT_8			7			//数据位8
	
	//停止位长度
#define STOP_1			0			//1位停止位
#define STOP_2			1			//2位停止位
	
	//校验形式
#define P_NONE			0			//无校验
#define P_ODD			1			//奇校验
#define P_EVEN			2			//偶校验



//--------------------------------不需要修改-------------------------------------------
//数据定义

// 消息对象初始化结构体
typedef struct _CANMSG_INIT_STRU 
{
	U8	stu8_MsgNum;									//消息号(范围：1~32)
	U8	stu8_RdTxFlg;									//收发类型(范围：'t' 或 'r')
	U16	stu16_MsgID;									//消息对象ID(范围：0x0001～0x07fe)
	U8	stu8_DataLen;									//数据长度(范围：0~8)
}CANMSG_INIT_STRU,*PCANMSG_INIT_STRU;

//外部调用的参数
//CANA
extern U16 gu16a_CANABuf[CANARXMSGCNT * 4];				//CAN缓冲区，每个ID占4个I16
extern I8 gi8a_CANART[CANARXMSGCNT];					//CAN接收、发送标志，'r'接收到新数据，'t'新数据待发送
extern U16 gu16a_CANACount[CANARXMSGCNT];				//CAN总线接收计数
extern U16 gu16a_CANACntCache[CANARXMSGCNT];			//CAN总线接收计数缓存

extern U16	gu16_CANAImptIndex;							//CAN重要数据索引
extern U16 gu16a_CANAImptBuf[100];						//CAN重要数据缓存区

extern U32 gu32_CANARevF;								//CAN接收标志
extern U8 gu8_CANARevIndex;								//CAN接收索引
extern U16 gu16a_CANADataBuf[160];						//CAN所有接收数据缓存区

extern const U16 gia_CANARXCfg[];						//CAN接收ID
extern const U16 gia_CANATXCfg[];						//CAN发送ID

//CANB
extern U16 gu16a_CANBBuf[CANBRXMSGCNT * 4];				//CAN缓冲区，每个ID占4个I16
extern I8 gi8a_CANBRT[CANBRXMSGCNT];					//CAN接收、发送标志，'r'接收到新数据，'t'新数据待发送
extern U16 gu16a_CANBCount[CANBRXMSGCNT];				//CAN总线接收计数
extern U16 gu16a_CANBCntCache[CANBRXMSGCNT];			//CAN总线接收计数缓存

extern U32 gu32_CANBRevF;								//CAN接收标志
extern U8 gu8_CANBRevIndex;								//CAN接收索引
extern U16 gu16a_CANBDataBuf[160];						//CAN所有接收数据缓存区

extern const U16 gia_CANBRXCfg[];						//CAN接收ID
extern const U16 gia_CANBTXCfg[];						//CAN发送ID

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------函数定义----------------------------------------
void User_InitSysCtrl(void);			//系统初始化
void User_InitCan(void);				//CAN初始化
void User_InitTimer(void);				//定时器初始化
void User_InitWatchdog(void);			//看门狗初始化
void User_InitUart(U8 pu8_Com, U32 pu32_BBaudRate,U8 Parity, U8 pu8_DataBits, U8 pu8_StopBits);//串口初始化
void User_InitGpio(void);				//GPIO配置
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

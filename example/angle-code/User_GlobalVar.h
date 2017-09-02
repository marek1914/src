/********************************************************
文件名称：User_GlobalVar.h
作者：王京城
版本：20131106
说明：本文件为程序的宏配置头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#ifndef USER_GLOBALVAR_H
#define USER_GLOBALVAR_H

#ifdef __cplusplus
extern "C" {
#endif

//定义软件运行的系统类型
#define SYSTEM_32	0				//32位系统（Windows、Linux等）
#define SYSTEM_16	1				//16位系统（DSP等）
#define SYSTEM_8	0				//8位系统（单片机等）

//用户添加数据类型定义
//32位系统
#if SYSTEM_32
typedef char				I8;
typedef unsigned char		U8;
typedef short				I16;
typedef unsigned short		U16;
typedef int					I32;
typedef unsigned int		U32;
typedef long				I64;
typedef unsigned long		U64;
typedef float				F32;
typedef double				F64;
#endif
//16位系统
#if SYSTEM_16
typedef char				I8;
typedef unsigned char		U8;
typedef int					I16;
typedef unsigned int		U16;
typedef long				I32;
typedef unsigned long		U32;
typedef float				F32;
typedef double				F64;
#endif
//8位系统
#if SYSTEM_8
typedef char				I8;
typedef unsigned char		U8;
typedef int					I16;
typedef unsigned int		U16;
typedef long				I32;
typedef unsigned long		U32;
typedef float				F32;
#endif


	


//------------------------------------------------------------------


//基本定时结构体
typedef struct _TIMERPAR
{	
	U16 stu16_tmr1mscnt;			//1ms定时计数
	U16 stu16_tmr2mscnt;			//2ms定时计数
	U16 stu16_tmr5mscnt;			//5ms定时计数
	U16 stu16_tmr10mscnt;			//10ms定时计数
	U16 stu16_tmr20mscnt;			//20ms定时计数
	U16 stu16_tmr50mscnt;			//50ms定时计数

	U16 stu16_tmr100mscnt;			//100ms定时计数
	U16 stu16_tmr200mscnt;			//200ms定时计数

	U16 stu16_tmr1scnt;				//1s定时计数	

}TIMERPAR,*pTIMERPAR;

//--------------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//
//-----------------------------------------------------------------

typedef struct _UART_STRU
{
	U8	stu8_ComCnt1;					//通信计数1
	U8	stu8_ComCnt2;					//通信计数2
	U8	stu8_IsrCnt;					//中断计数
	
	U8	stu8_RcvIndex;					//接收计数
	U8	stu8_TransIndex;				//发送计
	U8  stu8_RcvFlag;					//接收标志
	U8  stu8_CheckFlag;                 //校验标志
	U8	stu8a_RcvBuf[100];				//接收缓冲区
	U8	stu8a_TransBuf[200];			//发送缓冲区
	U8	stu8a_TransLength;				//发送标志
	U8	stu8a_RcvDealBuf1[100];			//UARTA接收处理区
	U8	stu8a_RcvDealBuf2[100];			//UARTB接收处理区
	U8	stu8a_RcvDealBuf3[100];			//UARTC接收处理区
	U8 stu8_Time;
	U8 stu8_Count;
	U8 stu8_Count1;
	U8 stu8_Count2;
	U8 stu8_Count3;
	U8	stu8a_TransData[200];			//发送数据
}UART;
extern UART gst_UARTA;
extern UART gst_UARTB;
extern UART gst_UARTC;

//星号极化方式
typedef struct _CMD_SATPOL
{
	U8 stu8_SatNum;					//星号		0:天通，1:天链
	U8 stu8_PolMod;					//极化方式	0:左旋，1:右旋
}CMD_SATPOL;

//波束指向
typedef struct _CMD_WAVEDIR
{
	F32 stf32_thita;				//thita角
	F32 stf32_pi;					//pi角
}CMD_WAVEDIR;

//相位校准
typedef struct _CMD_PHASEADJ
{
	U8 stu8_Index;					//索引
	U8 stu8a_Phase[819];			//相位
}CMD_PHASEADJ;

//振元设置
typedef struct _CMD_UNITSET
{
	U8 stu8_Line;					//行
	U8 stu8_Row;					//列
	U8 stu8a_Dim[4][4];				//衰减
	U8 stu8a_Phase[4][4];			//相位
	U8 stu8a_Power[4][4];			//电源
}CMD_UNITSET;

//命令解析结构体
typedef struct _RCVDATA_CMDDEAL
{
	U8 stu8_FramHead;				//帧头
	U8 stu8_CmdType1;				//帧命令1
	U8 stu8_CmdType2;				//帧命令2
	U8 stu8a_CmdData[8];			//帧内容
	U8 stu8_checkOut;				//校验
	U8 stu8_FramTail;				//帧尾
}RCVDATA_CMDDEAL;


//查询命令结构体
typedef struct _CMD_INQ
{
	I8 sti8_CmdType;				//命令类型, 负值无效的查询命令
	U8 stu8_Line;					//行
	U8 stu8_Row;					//列
	U8 stu8_Index;					//相位校准索引
}CMD_INQ;

//组件电压、电流、功率
typedef struct _CMD_VCW
{
	U8 stu8_Line;					//行
	U8 stu8_Row;					//列
	
	F32 stf32a_Vol[4][4];			//电压
	F32 stf32a_Cur[4][4];			//电流
	F32 stf32a_Power[4][4];			//功率
}CMD_VCW;

//组件温度
typedef struct _CMD_TEMP
{
	U8 stu8_Line;					//行
	U8 stu8_Row;					//列
	
	F32 stf32a_T[4][4];				//温度
}CMD_TEMP;

//设置结构体
typedef struct _SETDATA_STRU
{
	U8					stu8_SetFlag;	//设置命令标志，FALSE:不对下位机进行设置，TRUE:对下位机进行设置
	U8					stu8_SendFlag;	//发送标志，TRUE:发送数据帧,FALSE:不发送数据帧
	I32					sti32_TimerCnt;	//时间计数器，用于超时重发,50ms未收到回复则进行重发

	RCVDATA_CMDDEAL		st_CmdDeal;		//命令解析
	RCVDATA_CMDDEAL		st_SetCmd;		//设置命令
	RCVDATA_CMDDEAL		st_InqCmd;		//查询命令

	U8					stu8_WrkMod;	//工作模式
	F32					stf32_RcvFrq;	//接收频率
	F32					stf32_SendFrq;	//发射频率
	CMD_SATPOL			st_SatPol;		//星位、极化
	CMD_WAVEDIR			st_WaveDir;		//波束指向
	CMD_PHASEADJ		st_PhaseAdj;	//相位校准
	CMD_UNITSET			st_SendUnit;	//发射振元
	CMD_UNITSET			st_RcvUnit;		//接收振元
	CMD_VCW				st_UCW;			//电压、电流、功率
	CMD_TEMP			st_Temp;		//组件温度
	U32                 stu32_Count;
	U32                 stu32_Count1;
	//卫通指令
	F32					stf32_SatLong;	//卫星经度
	F32					stf32_PowerDim;	//功率衰减

	//卫通转发帧
	F32					stf32_Heading;	//航向
	F32					stf32_Pitch;	//俯仰角
	F32					stf32_Roll;		//横摇
	F32					stf32_Long;		//经度
	F32					stf32_Latitude;	//纬度
} SETDATA;

//版本日期
typedef struct _CMD_VERSION
{
	U16					stu16_Year;
	U8					stu8_Month;
	U8					stu8_Day;
}CMD_VERSION;

//状态结构体
typedef struct _STADATA_STRU
{
	U8					stu8_WrkMod;	//工作模式
	U8					stu8_Boot;		//FPGA启动状态 1:启动 2:未启动
	U32                 stu32_Count;
	U32                 stu32_Count1;
	F32					stf32_RcvFrq;	//接收频率
	F32					stf32_SendFrq;	//发射频率
	CMD_SATPOL			st_SatPol;		//星位、极化
	CMD_WAVEDIR			st_WaveDir;		//波束指向
	CMD_PHASEADJ		st_PhaseAdj;	//相位校准
	CMD_UNITSET			st_SendUnit;	//发射振元
	CMD_UNITSET			st_RcvUnit;		//接收振元
	CMD_VCW				st_UCW;			//电压、电流、功率
	CMD_TEMP			st_Temp;		//组件温度
	CMD_VERSION			st_FPGA;		//FPGA版本号
	CMD_VERSION			st_DSP;			//DSP版本号
} STADATA;

//--------------------------------------------------------------------------

#define RADMIN			0.00017				//弧度最小值 0.01°对应的弧度值(PI / 180 /100)
#define ChLiW
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_GLOBALVAR_H definition


//===========================================================================
// End of file.
//===========================================================================










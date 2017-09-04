/********************************************************
文件名称：User_ComFun.h
作者：王京城
版本：20131105
说明：本文件为User_ComFun的头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/


#ifndef USER_COMFUNCTION_H
#define USER_COMFUNCTION_H

#include <string.h>

#include "User_GlobalVar.h"			//用户定义的全局变量
#include "User_MacroCfg.h"			//用户定义的宏定义
//#include "User_Module.h"			//公共模块
#include <math.h>
#include "User_InitSys.h"
#include "User_ComFun.h"
#include "DSP28x_Project.h"			// Device Headerfile and Examples Include File

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//------------------------------------自用宏定义--------------------------------------


//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------函数定义----------------------------------------
void UartADataDeal(UART*pstp_Uart, SETDATA* pstp_WrkModSet);//2015年12月10日17:31:58
void UartBDataDeal(UART*pstp_Uart);//2015年12月10日17:31:58
void UartC_RcvDataDeal(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS);//2015年12月10日17:31:58

I32 MonPtrl_OutTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1);
I32 MonPtrl_InTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1);

I32 UartC_SendDataDeal(RCVDATA_CMDDEAL* pstp_CmdFrame, I8* pi8a_Buf);				//串口发送数据处理，由结构体变成数组

extern void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret);			//串口C发送函数
extern U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2);								
void UartC_SetInqCmdSend(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS);//查询、设置命令
void Uart_SetWaveDir(UART*pstp_Uart, SETDATA* pstp_WrkModSet);							//波束指向角设置
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

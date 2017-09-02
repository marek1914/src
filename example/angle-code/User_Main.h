/********************************************************
文件名称：User_Main.h
作者：王京城
版本：20131104
说明：本文件为主程序的头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#ifndef USER_MAIN_H
#define USER_MAIN_H

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "User_GlobalVar.h"			//用户定义的全局变量
#include "User_MacroCfg.h"			//用户定义的宏定义

#include "DSP28x_Project.h"			// Device Headerfile and Examples Include File
#include "User_InitSys.h"			//用户定义的设备初始化
#include "User_Flash.h"				//用户定义的全局变量
#include "User_ComFun.h"			//用户定义的公共函数
#include "User_ComFunction.h"		//用户定义的公共函数
#include "User_PositionChange.h"	//用户定义

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------数据定义----------------------------------------
TIMERPAR gst_TimerPar;							//基本定时
SETDATA gst_CMD;								//命令结构体
STADATA gst_STUS;								//状态结构体
//-----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------函数定义----------------------------------------
void InitGlobalVar(void);																//初始化全局变量
void UartA_SendData(UART *pstp_UARTA,STADATA *pstp_StaData, SETDATA * pstp_SETDATA);	//串口A发送函数																		//双天线时，航向处理:跟踪状态，自主模式，发送惯导航向值，且航向值有效，其他状态发送罗经航向值，航向值无效
void UartB_SendData(UART *pstp_UARTB);													//串口B发送函数
void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret);						//串口C发送函数

extern void CalSatInf(SETDATA * pstp_SETDATA);
//-----------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_MAIN_H definition


//===========================================================================
// End of file.
//===========================================================================

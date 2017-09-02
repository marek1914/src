/********************************************************
文件名称：User_ComFun.h
作者：王京城
版本：20131105
说明：本文件为User_ComFun的头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/


#ifndef USER_COMFUN_H
#define USER_COMFUN_H

#include <string.h>

#include "User_GlobalVar.h"			//用户定义的全局变量
#include "User_MacroCfg.h"			//用户定义的宏定义
#include <math.h>

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
//CAN相关
void ComFun_CANASend(U8 pu8_MsgNum, U16 *pu16p_Buf);						//CANA发送函数(消息发送)
void ComFun_CANBSend(U8 pu8_MsgNum, U16 *pu16p_Buf);						//CANB发送函数(消息发送)

void ComFun_CANAIDSend(U16 pu16_MsgID, U16 *pu16p_Buf);						//CANA发送函数(ID发送)
void ComFun_CANBIDSend(U16 pu16_MsgID, U16 *pu16p_Buf);						//CANB发送函数(ID发送)

//小函数
U16 ComFun_U16HToL(U16 pu16_Data);											//U16数据高低八位互换
U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2);								//两个U8转换为U16

U8 *User_strchr(I8 *pi8p_stringToSearch, 
				I32 pi32_StringMaxLen, I8 pi8_charToFind);					//查找数组中pi8_charToFind第一次出现的位置
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

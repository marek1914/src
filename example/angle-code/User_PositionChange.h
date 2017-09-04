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
#include <math.h>

#include "User_GlobalVar.h"			//用户定义的全局变量
#include "User_MacroCfg.h"			//用户定义的宏定义
#include "User_InitSys.h"
#include "User_ComFunction.h"
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

//星位计算所用参数
typedef	struct _PARAMETER_SatGeoAng_STRU
{
	F32 stf32_SatLong;				//卫星经度
	F32 stf32_LocLong;				//当前经度
	F32 stf32_LocLat;				//当前纬度
	
	U8 stu8_POLMode;				//极化方式
	
	F32 stf32_AZAng;				//方位角度
	F32 stf32_ELAng;				//俯仰角度
	F32 stf32_POLAng;				//极化角度
		
} PARAMETER_SatGeoAng, *PPARAMETER_SatGeoAng;

//坐标变换所用参数
typedef	struct _PARAMETER_TranCoor_STRU
{
	F32 stf32_Heading;				//航向
	F32 stf32_Roll;					//横倾
	F32 stf32_Pitch;				//纵倾	
	
	F32 stf32_AZGeoAng;				//方位地理角度
	F32 stf32_ELGeoAng;				//俯仰地理角度
	F32 stf32_POLGeoAng;			//极化地理角度
	
	F32 stf32_AZDeckAng;			//方位甲板角度
	F32 stf32_ELDeckAng;			//俯仰甲板角度
	F32 stf32_POLDeckAng;			//极化甲板角度
	F32 stf32_CRSDeckAng;			//交叉甲板角度
		
} PARAMETER_TranCoor, *PPARAMETER_TranCoor;

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------函数定义----------------------------------------
void CalSatInf(SETDATA * pstp_SETDATA);
void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng);
void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);
void MDL_DeckToBall(PPARAMETER_TranCoor pstp_TRANCOOR,SETDATA * pstp_SETDATA);
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);					//AE坐标变化极化角偏移计算(过程函数)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

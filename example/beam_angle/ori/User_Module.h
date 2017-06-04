/********************************************************
文件名称：User_Module.h
作者：王京城
版本：20130904
说明：本文件为User_Module的头文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#ifndef USER_MODULE_H
#define USER_MODULE_H

#include <math.h>
#include "User_GlobalVar.h"			//用户定义的全局变量
#include "User_MacroCfg.h"			//用户定义的宏定义

#ifdef __cplusplus
extern "C" {
#endif

#define RADMIN			PI / 180 / 100		//弧度最小值 0.01°对应的弧度值
#define AZGRSBETA		60.0				//方位轴与交叉轴夹角

#define POSONDECK		0					//姿态仪安装在甲板上
#define POSONAZAXIS		1					//姿态仪安装在方位轴上

//-----------------------------------参数定义----------------------------------------
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

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------函数定义----------------------------------------

//过程函数
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);					//AE坐标变化极化角偏移计算(过程函数)
F32 POLOffset_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR);					//ACE坐标变化极化角偏移计算(过程函数)

//外部调用函数
void MDL_DeckToGeo_RP(F32 pf32_AZDeckAng, F32 pf32_DeckRoll, F32 pf32_DeckPitch,
					  F32 *pf32p_GeoRoll, F32 *pf32p_GeoPitch);			//姿态仪随动方位，将横倾纵倾转换到船首尾线的位置

void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);		//AE座架地理角转变为甲板角(使用前提，横倾、纵倾为船首方向)
void MDL_TranCoor_DeckToGeo_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);		//AE座架甲板角转变为地理角(使用前提，横倾、纵倾为船首方向)	

void MDL_TranCoor_GeoToDeck_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR);	//ACE座架地理角转变为甲板角(使用前提，横倾、纵倾为随动方位转动)				
void MDL_TranCoor_DeckToGeo_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR);	//ACE座架甲板角转变为地理角(使用前提，横倾、纵倾为随动方位转动)	

void MDL_TranCoor_GeoToDeck_AECP(PPARAMETER_TranCoor pstp_TRANCOOR);	//AEC座架地理角转变为甲板角(使用前提，横倾、纵倾为船首方向)				
void MDL_TranCoor_DeckToGeo_AECP(PPARAMETER_TranCoor pstp_TRANCOOR);	//AEC座架甲板角转变为地理角(使用前提，横倾、纵倾为船首方向)	

void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng);				//计算卫星方位、俯仰和极化理论角

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_MAIN_H definition


//===========================================================================
// End of file.
//===========================================================================

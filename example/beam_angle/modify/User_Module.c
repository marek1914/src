
/********************************************************
文件名称：User_Module.c
作者：
版本：V1.00(20130904)
说明：模块程序

修改记录：
修改时间		修改人	修改内容

*********************************************************/

#include "User_Module.h"

/********************************************************
函数名称：MDL_DeckToGeo_RP
函数功能：姿态仪随动方位，将横倾纵倾转换到船首尾线的位置
入口参数：
pf32_AZDeckAng;				//方位甲板角
pf32_DeckRoll;				//输入横倾
pf32_DeckPitch;				//输入纵倾
出口参数：
pf32p_GeoRoll;				//输出横倾
pf32p_GeoPitch;				//输出纵倾
备    注：
********************************************************/
void MDL_DeckToGeo_RP(F32 pf32_AZDeckAng, F32 pf32_DeckRoll, F32 pf32_DeckPitch, F32 *pf32p_GeoRoll, F32 *pf32p_GeoPitch)
{
	F32 lf32_x;
	F32 lf32_y;	
	F32 lf32_R;
	F32 lf32_P;
	F32 lf32_A;
	F32 lf32_temp; 
	
	F32 sinld_A;
	F32 cosld_A; 
	F32 sinld_P;
	F32 cosld_P; 
	F32 sinld_R;
	F32 cosld_R; 
	
	lf32_A = pf32_AZDeckAng * DEGTORAD;
	lf32_R = pf32_DeckRoll * DEGTORAD;
	lf32_P = pf32_DeckPitch * DEGTORAD;
	
	sinld_A = sin(lf32_A);
	cosld_A = cos(lf32_A);
	sinld_P = sin(lf32_P);
	cosld_P = cos(lf32_P);
	sinld_R = sin(lf32_R);
	cosld_R = cos(lf32_R);
	
	lf32_y = cosld_A * sinld_R * cosld_P - sinld_A * sinld_P;
	lf32_x = cosld_R * cosld_P;
	
	if (lf32_x >= 0.0)
    {
		if (lf32_x < RADMIN) 
		{
			lf32_x = RADMIN;    //0.0  0.01°之间，置为0.01°
		}
    }//end of if (lf32_x >= 0.0)
	else
	{
		if (lf32_x > -RADMIN)
		{
			lf32_x = -RADMIN;  //0.0  -0.01°之间，置为-0.01°
		}
	}
	
	lf32_temp = atan(lf32_y / lf32_x) * RADTODEG;	   //(-90.0 90.0) 一或四象限	
	*pf32p_GeoRoll = lf32_temp;  
	
	lf32_x = cosld_A * sinld_P + sinld_A * sinld_R * cosld_P;
	lf32_temp = asin(lf32_x) * RADTODEG;  
	*pf32p_GeoPitch = lf32_temp;
	
	return;	
}

/********************************************************
函数名称：POLOffset_AEP
函数功能：AE轴坐标变化极化角偏移计算
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;				//纵倾(-90.00~90.00)
stf32_AZGeoAng;				//方位地理角度(0~359.999)
stf32_ELGeoAng;				//俯仰地理角度(-180.000~179.999)
stf32_AZDeckAng;			//方位甲板角度(0~359.999)
stf32_ELDeckAng;			//俯仰甲板角度(-180.000~179.999)
出口参数：
lf32_temp;					//极化偏移值
备    注：
********************************************************/
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x, lf32_y;
	F32 lf32_ELAng, lf32_AZAng, lf32_Pitch, lf32_AZAng2;
	F32 lf32_temp;
	
	//度变换为弧度
	lf32_ELAng = (F32)(pstp_TRANCOOR->stf32_ELGeoAng * DEGTORAD);
	lf32_AZAng = (F32)((pstp_TRANCOOR->stf32_AZGeoAng - pstp_TRANCOOR->stf32_Heading) * DEGTORAD);
	lf32_Pitch = (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);
	lf32_AZAng2 = (F32)(pstp_TRANCOOR->stf32_AZDeckAng * DEGTORAD);
	
	//
	lf32_y = (F32)(sin(lf32_ELAng) * cos(lf32_AZAng) * cos(lf32_Pitch) * sin(lf32_AZAng2)
        -cos(lf32_ELAng) * sin(lf32_Pitch) * sin(lf32_AZAng2)
        -sin(lf32_ELAng) * cos(lf32_AZAng) * sin(lf32_Pitch)* sin(lf32_Pitch) * cos(lf32_AZAng2)
        -cos(lf32_ELAng) * cos(lf32_Pitch) * sin(lf32_Pitch)* cos(lf32_AZAng2)
        -sin(lf32_ELAng )* sin(lf32_AZAng) * cos(lf32_Pitch)* cos(lf32_AZAng2));
	
	lf32_x = (F32)(sin(lf32_ELAng) * cos(lf32_AZAng) * cos(lf32_Pitch) * cos(lf32_AZAng2) * sin(lf32_ELAng)
        -cos(lf32_ELAng) * sin(lf32_Pitch) * cos(lf32_AZAng2) * sin(lf32_ELAng)
        +sin(lf32_ELAng) * cos(lf32_AZAng) * sin(lf32_Pitch) * sin(lf32_Pitch) * sin(lf32_AZAng2) * sin(lf32_ELAng)
        +cos(lf32_ELAng) * cos(lf32_Pitch) * sin(lf32_Pitch) * sin(lf32_AZAng2) * sin(lf32_ELAng)
        +sin(lf32_ELAng) * sin(lf32_AZAng) * cos(lf32_Pitch) * sin(lf32_AZAng2) * sin(lf32_ELAng)
        +sin(lf32_ELAng) * cos(lf32_AZAng) * sin(lf32_Pitch) * cos(lf32_Pitch) * cos(lf32_ELAng)
        +cos(lf32_ELAng) * cos(lf32_Pitch) * cos(lf32_Pitch) * cos(lf32_ELAng)
        -sin(lf32_ELAng) * sin(lf32_AZAng) * sin(lf32_Pitch) * cos(lf32_ELAng));
	
	if (lf32_x >= 0.0)
    {
		if (lf32_x < RADMIN) 
		{
			lf32_x = (F32)(RADMIN);							//0.0  0.01°之间，置为0.01°
		}
    }
	else
    {
		if (lf32_x > -RADMIN)
		{
			lf32_x = (F32)(-RADMIN);						//0.0  -0.01°之间，置为-0.01°
		}
    }	
	lf32_temp = (F32)(atan2(lf32_y,lf32_x) * RADTODEG);		//-90.0 - 90.0

	return lf32_temp;
}

/********************************************************
函数名称：POLOffset_ACEP
函数功能：ACE轴坐标变化极化角偏移计算
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZGeoAng;				//方位地理角度(0~359.999)
stf32_ELGeoAng;				//俯仰地理角度(-180.000~179.999)
stf32_AZDeckAng;			//方位甲板角度(0~359.999)
stf32_ELDeckAng;			//俯仰甲板角度(-180.000~179.999)
出口参数：
lf32_temp;					//极化偏移值
备    注：
********************************************************/
F32 POLOffset_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x,lf32_y;
	F32 lf32_ELAngle,lf32_CRSAngle,lf32_ELAngle2,lf32_Pitch,lf32_B;
	F32 lf32_temp;

	lf32_ELAngle = (F32)((pstp_TRANCOOR->stf32_ELGeoAng - pstp_TRANCOOR->stf32_Roll) * DEGTORAD);
	lf32_CRSAngle = (F32)((180.0 + pstp_TRANCOOR->stf32_CRSDeckAng) * DEGTORAD);
	lf32_ELAngle2 = (F32)((180.0 - pstp_TRANCOOR->stf32_ELDeckAng - AZGRSBETA) * DEGTORAD);
	lf32_Pitch = (F32)(pstp_TRANCOOR->stf32_Pitch  * DEGTORAD);
	lf32_B = (F32)(AZGRSBETA * DEGTORAD);
	
	lf32_y = (F32)(sin(lf32_ELAngle)*cos(lf32_B)*sin(lf32_CRSAngle)
		+cos(lf32_ELAngle)*cos(lf32_Pitch)*sin(lf32_B)*sin(lf32_CRSAngle)
		-cos(lf32_ELAngle)*sin(lf32_Pitch)*cos(lf32_CRSAngle));
	lf32_x = (F32)(sin(lf32_ELAngle)*cos(lf32_B)*cos(lf32_CRSAngle)*sin(lf32_ELAngle2)
        +cos(lf32_ELAngle)*cos(lf32_Pitch)*sin(lf32_B)*cos(lf32_CRSAngle)*sin(lf32_ELAngle2)
        +cos(lf32_ELAngle)*sin(lf32_Pitch)*sin(lf32_CRSAngle)*sin(lf32_ELAngle2)
        -sin(lf32_ELAngle)*sin(lf32_B)*cos(lf32_ELAngle2)
        +cos(lf32_ELAngle)*cos(lf32_Pitch)*cos(lf32_B)*cos(lf32_ELAngle2));  
	if (lf32_x >= 0.0)
	{
		if (lf32_x < RADMIN)
		{
			lf32_x = (F32)(RADMIN);							//0.0  0.01°之间，置为0.01°
		}
	}
	else
	{
		if (lf32_x > -RADMIN)
		{
			lf32_x = (F32)(-RADMIN);						//0.0  -0.01°之间，置为-0.01°
		}
	}
	
	lf32_temp = (F32)(atan2(lf32_y,lf32_x) * RADTODEG);		//(-180.0 180.0)

	return -lf32_temp;	
} 

/********************************************************
函数名称：MDL_TranCoor_GeoToDeck_AEP
函数功能：AE座架地理角转变为甲板角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZGeoAng;				//方位地理角度(0~359.999)
stf32_ELGeoAng;				//俯仰地理角度(-180.000~179.999)
出口参数：
stf32_AZDeckAng;			//方位甲板角度
stf32_ELDeckAng;			//俯仰甲板角度
stf32_POLDeckAng;			//极化甲板角度
备    注：用于AE座架坐标变化，地理角变换为甲板角
********************************************************/
void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x,lf32_y;
	F32 lf32_lf32_coseLAng,lf32_CosAZAng,lf32_CosPitch,lf32_lf32_cosroll;
	F32 lf32_lf32_sineLAng,lf32_SinAZAng,lf32_SinPitch,lf32_lf32_sinroll;
	F32 lf32_temp;

	//计算出参数的sin()与cos()值，避免程序中重复计算，提高效率
	lf32_temp = (F32)(pstp_TRANCOOR->stf32_ELGeoAng * DEGTORAD);
	lf32_lf32_coseLAng = (F32)cos(lf32_temp);
	lf32_lf32_sineLAng = (F32)sin(lf32_temp);
	lf32_temp = (F32)((pstp_TRANCOOR->stf32_AZGeoAng - pstp_TRANCOOR->stf32_Heading) * DEGTORAD);
	lf32_CosAZAng = (F32)cos(lf32_temp);
	lf32_SinAZAng = (F32)sin(lf32_temp);
	lf32_temp = (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);
	lf32_CosPitch = (F32)cos(lf32_temp);
	lf32_SinPitch = (F32)sin(lf32_temp);
	lf32_temp = (F32)(pstp_TRANCOOR->stf32_Pitch * DEGTORAD);
	lf32_lf32_cosroll = (F32)cos(lf32_temp);
	lf32_lf32_sinroll = (F32)sin(lf32_temp);
	
	//方位甲板角 
	lf32_x = (F32)(lf32_lf32_cosroll * lf32_lf32_coseLAng * lf32_CosAZAng + lf32_lf32_sinroll * lf32_lf32_sineLAng);	
	lf32_y = (F32)(lf32_lf32_coseLAng * (lf32_CosPitch * lf32_SinAZAng + lf32_SinPitch * lf32_lf32_sinroll * lf32_CosAZAng)
			- lf32_SinPitch * lf32_lf32_cosroll * lf32_lf32_sineLAng);	
	if (lf32_x >= 0.0)
    {
		if (lf32_x < RADMIN)
		{
			lf32_x = (F32)RADMIN;							 //0.0  0.01°之间，置为0.01°
		}
    }
	else
    {
		if (lf32_x > -RADMIN) 
		{
			lf32_x = (F32)-RADMIN;							 //0.0  -0.01°之间，置为-0.01°
		}
    }	
	lf32_temp = (F32)(atan(lf32_y / lf32_x) * RADTODEG);	 //(-90.0 90.0) 一或四象限
	if (lf32_x >= 0.0)										 //一四象限
    {
		if (lf32_y < 0.0) 
		{
			lf32_temp += 360.0;								 //四象限
		}
    }
	else
    {
		lf32_temp += 180.0;									 //二三象限
    }
	
	if (lf32_temp >= 360.0)
	{
		lf32_temp -= 360.0;
	}	
	else if (lf32_temp < 0.0)
	{
		lf32_temp += 360.0;
	}		
	pstp_TRANCOOR->stf32_AZDeckAng = (F32)lf32_temp;	
	
	//俯仰甲板角 
	lf32_x = (F32)(lf32_SinPitch * lf32_lf32_coseLAng * lf32_SinAZAng 
			- lf32_CosPitch * lf32_lf32_sinroll * lf32_lf32_coseLAng * lf32_CosAZAng
			+ lf32_CosPitch * lf32_lf32_cosroll * lf32_lf32_sineLAng);
	
	lf32_temp = (F32)(asin(lf32_x) * RADTODEG);  
	pstp_TRANCOOR->stf32_ELDeckAng= (F32)lf32_temp;

	//极化甲板角
	lf32_temp = POLOffset_AEP(pstp_TRANCOOR);

	lf32_temp = pstp_TRANCOOR->stf32_POLGeoAng - lf32_temp;		//-90 - +90
	if (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;
	}
	else if (lf32_temp < -90.0)
	{
		lf32_temp += 180.0;
	}
	pstp_TRANCOOR->stf32_POLDeckAng = lf32_temp;

	return;	
}

/********************************************************
函数名称：MDL_TranCoor_DeckToGeo_AEP
函数功能：AE座架甲板角转变为地理角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZDeckAng;			//方患装褰嵌?0~359.999)
stf32_ELDeckAng;			//俯仰甲板角度(-180.000~179.999)
出口参数：
stf32_AZGeoAng;				//方位地理角度
stf32_ELGeoAng;				//俯仰地理角度
stf32_POLGeoAng;			//极化地理角度
备    注：用于AE座架坐标变化，甲板角变换为地理角
********************************************************/
void MDL_TranCoor_DeckToGeo_AEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x,lf32_y;
	F32 lf32_lf32_coseLAng,lf32_CosAZAng,lf32_CosPitch,lf32_lf32_cosroll;
	F32 lf32_lf32_sineLAng,lf32_SinAZAng,lf32_SinPitch,lf32_lf32_sinroll;
	F32 lf32_temp;
	
	//计算出参数的sin()与cos()值，避免程序中重复计算，提高效率
	lf32_temp  = (F32)(pstp_TRANCOOR->stf32_ELDeckAng * DEGTORAD);
	lf32_lf32_coseLAng = (F32)cos(lf32_temp);
	lf32_lf32_sineLAng = (F32)sin(lf32_temp);
	lf32_temp =  (F32)(pstp_TRANCOOR->stf32_AZDeckAng * DEGTORAD);
	lf32_CosAZAng = (F32)cos(lf32_temp);
	lf32_SinAZAng = (F32)sin(lf32_temp);
	lf32_temp	= (F32)(pstp_TRANCOOR->stf32_Roll* DEGTORAD);
	lf32_CosPitch = (F32)cos(lf32_temp);
	lf32_SinPitch = (F32)sin(lf32_temp);
	lf32_temp	= (F32)(pstp_TRANCOOR->stf32_Pitch * DEGTORAD);
	lf32_lf32_cosroll = (F32)cos(lf32_temp);
	lf32_lf32_sinroll = (F32)sin(lf32_temp);
	
	//方位地理角
	lf32_x = (F32)(lf32_lf32_coseLAng * (lf32_lf32_cosroll * lf32_CosAZAng + lf32_lf32_sinroll * lf32_SinPitch * lf32_SinAZAng)
		- lf32_lf32_sinroll * lf32_CosPitch * lf32_lf32_sineLAng);	
	lf32_y = (F32)(lf32_lf32_coseLAng * lf32_CosPitch * lf32_SinAZAng + lf32_SinPitch * lf32_lf32_sineLAng);
	
	if (lf32_x >= 0.0)						   
    {
		if (lf32_x < RADMIN)
		{
			lf32_x = (F32)RADMIN;							//0.0  0.01°之间，置为0.01°
		}
    }
	else													//二、三象限
    {
		if (lf32_x > -RADMIN)
		{
			lf32_x = (F32)(-RADMIN);						//0.0  -0.01°之间，置为-0.01°
		}
    }	
	lf32_temp = (F32)(atan(lf32_y / lf32_x) * RADTODEG);			//(-90.0 90.0) 一或四象限
	if (lf32_x >= 0.0)										//一四象限
    {
		if (lf32_y < 0.0)
		{
			lf32_temp += 360.0;								//四象限 
		}
    }
	else
    {
		lf32_temp += 180.0;									//二三象限
    }
	lf32_temp += (F32)pstp_TRANCOOR->stf32_Heading;
	if (lf32_temp >= 360.0) 
	{
		lf32_temp -= 360.0;
	}
	pstp_TRANCOOR->stf32_AZGeoAng = (F32)lf32_temp;	
	
	//俯仰地理角
	lf32_x = (F32)(lf32_lf32_coseLAng * (lf32_lf32_sinroll * lf32_CosAZAng - lf32_lf32_cosroll * lf32_SinPitch * lf32_SinAZAng)
		+ lf32_lf32_cosroll * lf32_CosPitch * lf32_lf32_sineLAng);  
	lf32_temp = (F32)(asin(lf32_x) * RADTODEG);
	pstp_TRANCOOR->stf32_ELGeoAng = (F32)lf32_temp; 

	//极化地理角
	lf32_temp = POLOffset_AEP(pstp_TRANCOOR);

	lf32_temp = pstp_TRANCOOR->stf32_POLDeckAng + lf32_temp; //-90 - +90
	if (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;
	}
	else if (lf32_temp < -90.0)
	{
		lf32_temp += 180.0;
	}
	pstp_TRANCOOR->stf32_POLGeoAng = lf32_temp;

	return;				
}

/********************************************************
函数名称：MDL_TranCoor_GeoToDeck_ACEP
函数功能：ACE座架地理角转变为甲板角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZGeoAng;				//方位地理角度(0~359.999)
stf32_ELGeoAng;				//俯仰地理角度(-180.000~179.999)
stf32_POLGeoAng;			//极化地理角度(-180.000~179.999)
出口参数：
stf32_AZDeckAng;			//方位甲板角度
stf32_ELDeckAng;			//俯仰甲板角度
stf32_POLDeckAng;			//极化甲板角度
stf32_CRSDeckAng;			//交叉甲板角度
备    注：
********************************************************/
void MDL_TranCoor_GeoToDeck_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x, lf32_y, lf32_z;
	F32 lf32_ELAngle, lf32_Roll, lf32_B;
	F32 lf32_temp;
	
	lf32_ELAngle	= (F32)((pstp_TRANCOOR->stf32_ELGeoAng - pstp_TRANCOOR->stf32_Pitch) * DEGTORAD);
	lf32_Roll	= (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);
	lf32_B	= (F32)((-AZGRSBETA) * DEGTORAD);
 
	//方位甲板角
	lf32_temp = pstp_TRANCOOR->stf32_AZGeoAng - pstp_TRANCOOR->stf32_Heading; 
	if (lf32_temp >= 360.0) 
	{
		lf32_temp -= 360.0;
	}
	else if (lf32_temp < 0.0) 
	{
		lf32_temp += 360.0;
	}
	pstp_TRANCOOR->stf32_AZDeckAng = (F32)lf32_temp; 
	
	//俯仰甲板角
	lf32_z = (F32)(-cos(lf32_ELAngle) * sin(lf32_B) + sin(lf32_ELAngle) * cos(lf32_Roll) * cos(lf32_B));
	lf32_z = (F32)(asin(lf32_z) *  RADTODEG);  // -180 +180
	
	//交叉甲板角
	lf32_y = (F32)(sin(lf32_ELAngle) * sin(lf32_Roll));
	lf32_x = (F32)(cos(lf32_ELAngle) * cos(lf32_B) + sin(lf32_ELAngle) * cos(lf32_Roll) * sin(lf32_B));
	
	lf32_y = (F32)(atan2(lf32_y,lf32_x) * RADTODEG);   //-180 +180
	if (fabs(lf32_y) > 90.0)
	{
		if (lf32_y > 0.0) 
		{
			lf32_y -= 180.0;
		}
		else 
		{
			lf32_y += 180.0;
		}
		if (lf32_z >= 0.0) 
		{
			lf32_z = (F32)(180.0 - lf32_z);
		}
		else 
		{
			lf32_z = (F32)(-180.0 - lf32_z);
		}
	}
	lf32_z -= AZGRSBETA;
	if (lf32_z > 180.0) 
	{
		lf32_z -= 360.0;
	}
	pstp_TRANCOOR->stf32_ELDeckAng = lf32_z;
	pstp_TRANCOOR->stf32_CRSDeckAng = lf32_y;

	//极化甲板角
	lf32_temp = POLOffset_ACEP(pstp_TRANCOOR);
	lf32_temp = pstp_TRANCOOR->stf32_POLGeoAng - lf32_temp;
	while (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;		   //超范围保护
	}
	while (lf32_temp < -90.0)
	{
		lf32_temp += 180.0;
	}
	pstp_TRANCOOR->stf32_POLDeckAng = lf32_temp;
}

/********************************************************
函数名称：MDL_TranCoor_DeckToGeo_ACEP
函数功能：ACE座架甲板角转变为地理角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZDeckAng;			//方位甲板角度(0~359.999)
stf32_ELDeckAng;			//俯仰甲板角度(-180.000~179.999)
stf32_POLDeckAng;			//极化甲板角度(-180.000~179.999)
stf32_CRSDeckAng;			//交叉甲板角度(-180.000~179.999)
出口参数：
stf32_AZGeoAng;				//方位地理角度
stf32_ELGeoAng;				//俯仰地理角度
stf32_POLGeoAng;			//极化地理角度
备    注：
********************************************************/
void MDL_TranCoor_DeckToGeo_ACEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x, lf32_y, lf32_z;
	F32 lf32_AZAngle, lf32_ELAngle, lf32_CRSAngle, lf32_Roll, lf32_Pitch, lf32_B;
	F32 lf32_temp;
	
	lf32_AZAngle  = (F32)((pstp_TRANCOOR->stf32_AZDeckAng + pstp_TRANCOOR->stf32_Heading) * DEGTORAD);
	lf32_ELAngle =  (F32)((180.0 - pstp_TRANCOOR->stf32_ELDeckAng - AZGRSBETA) * DEGTORAD);
	lf32_CRSAngle =  (F32)((180.0 - pstp_TRANCOOR->stf32_CRSDeckAng) * DEGTORAD); 
	lf32_Roll	= (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);
	lf32_Pitch	= (F32)(pstp_TRANCOOR->stf32_Pitch * DEGTORAD);
	lf32_B = (F32)((AZGRSBETA) * DEGTORAD);
	
	//方位地理角转换  
	lf32_y = (F32)(cos(lf32_ELAngle) * cos(lf32_CRSAngle) * cos(lf32_B) * cos(lf32_Pitch) *sin(lf32_AZAngle)
        +sin(lf32_ELAngle) * sin(lf32_B) * cos(lf32_Pitch) * sin(lf32_AZAngle)
		+cos(lf32_ELAngle) * cos(lf32_CRSAngle) * sin(lf32_B) * cos(lf32_Roll) * sin(lf32_Pitch) * sin(lf32_AZAngle)
		-sin(lf32_ELAngle) * cos(lf32_B) * cos(lf32_Roll) * sin(lf32_Pitch) * sin(lf32_AZAngle)
		+cos(lf32_ELAngle) * sin(lf32_CRSAngle) * sin(lf32_Roll) * sin(lf32_Pitch) * sin(lf32_AZAngle)
        -cos(lf32_ELAngle) * cos(lf32_CRSAngle) * sin(lf32_B) * sin(lf32_Roll) * cos(lf32_AZAngle)
        +sin(lf32_ELAngle) * cos(lf32_B) * sin(lf32_Roll) * cos(lf32_AZAngle)
        +cos(lf32_ELAngle) * sin(lf32_CRSAngle) * cos(lf32_Roll) * cos(lf32_AZAngle));
	
	lf32_x = (F32)(cos(lf32_ELAngle) * cos(lf32_CRSAngle) * cos(lf32_B) * cos(lf32_Pitch) * cos(lf32_AZAngle)
		+sin(lf32_ELAngle) * sin(lf32_B) * cos(lf32_Pitch) * cos(lf32_AZAngle)
		+cos(lf32_ELAngle) * cos(lf32_CRSAngle) * sin(lf32_B) * cos(lf32_Roll) * sin(lf32_Pitch) * cos(lf32_AZAngle)
		-sin(lf32_ELAngle) * cos(lf32_B) * cos(lf32_Roll) * sin(lf32_Pitch) * cos(lf32_AZAngle)
		+cos(lf32_ELAngle) * sin(lf32_CRSAngle) * sin(lf32_Roll) * sin(lf32_Pitch) * cos(lf32_AZAngle)
		+cos(lf32_ELAngle) * cos(lf32_CRSAngle) * sin(lf32_B) * sin(lf32_Roll) * sin(lf32_AZAngle)
		-sin(lf32_ELAngle) * cos(lf32_B) * sin(lf32_Roll) * sin(lf32_AZAngle)
		-cos(lf32_ELAngle) * sin(lf32_CRSAngle) * cos(lf32_Roll) * sin(lf32_AZAngle));
	
	if (lf32_x >= 0.0)
    {
		if (lf32_x < RADMIN)
		{
			lf32_x = (F32)(RADMIN);    //0.0  0.01°之间，置为0.01°
		}
    }
	else
    {
		if (lf32_x > -RADMIN) 
		{
			lf32_x = (F32)(-RADMIN);  //0.0  -0.01°洌梦?0.01°
		}
    }
	
	lf32_temp = (F32)(atan2(lf32_y,lf32_x) * RADTODEG);	   //(-90.0 90.0) 一或四象限
	
	if (lf32_temp < 0.0)
	{
		lf32_temp += 360.0;
	}
	pstp_TRANCOOR->stf32_AZGeoAng = (F32)lf32_temp; 
	
	//俯仰地理角
	lf32_z = (F32)(cos(lf32_ELAngle) * cos(lf32_CRSAngle) * cos(lf32_B) * sin(lf32_Pitch)
		+sin(lf32_ELAngle) * sin(lf32_B) * sin(lf32_Pitch)
		-cos(lf32_ELAngle) * cos(lf32_CRSAngle) * sin(lf32_B) * cos(lf32_Roll) * cos(lf32_Pitch)
        +sin(lf32_ELAngle) * cos(lf32_B) * cos(lf32_Roll) * cos(lf32_Pitch)
		-cos(lf32_ELAngle) * sin(lf32_CRSAngle) * sin(lf32_Roll) * cos(lf32_Pitch));
	
	lf32_temp = (F32)(asin(lf32_z) *  RADTODEG);  
	pstp_TRANCOOR->stf32_ELGeoAng = (F32)lf32_temp;

	//极化地理角
	lf32_temp = POLOffset_ACEP(pstp_TRANCOOR);
	lf32_temp = pstp_TRANCOOR->stf32_POLDeckAng + lf32_temp;
	while (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;		   //超范围保护
	}
	while (lf32_temp < -90.0)
	{
		lf32_temp += 180.0;
	}
	pstp_TRANCOOR->stf32_POLGeoAng = lf32_temp;
}

/********************************************************
函数名称：MDL_TranCoor_GeoToDeck_AECP
函数功能：AEC座架地理角转变为甲板角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZGeoAng;				//方位地理角度(0~359.999)
stf32_ELGeoAng;				//俯仰地理角度(-180.000~179.999)
stf32_POLGeoAng;			//极化地理角度(-180.000~179.999)
出口参数：
stf32_AZDeckAng;			//方位甲板角度
stf32_ELDeckAng;			//俯仰甲板角度
stf32_POLDeckAng;			//极化甲板角度
stf32_CRSDeckAng;			//交叉甲板角度
备    注：
********************************************************/
void MDL_TranCoor_GeoToDeck_AECP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_f, lf32_fr, lf32_fp, lf32_f1, lf32_f2;
	F32 lf32_sinak, lf32_cosak, lf32_sine, lf32_cose, lf32_sinr, lf32_cosr, lf32_sinp, lf32_cosp;

	//方位目标甲板角
	lf32_f = pstp_TRANCOOR->stf32_AZGeoAng - pstp_TRANCOOR->stf32_Heading;
	if(lf32_f<0)
	{
		lf32_f += 360.0;
	}
	else if(lf32_f>=360.0)
	{
		lf32_f -= 360.0;
	}
	pstp_TRANCOOR->stf32_AZDeckAng = lf32_f;                                  
	
	 //俯仰目标甲板角
	lf32_f = (F32)(pstp_TRANCOOR->stf32_AZDeckAng * DEGTORAD);
	lf32_fr = (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);
	lf32_fp = (F32)(pstp_TRANCOOR->stf32_Pitch * DEGTORAD);
	lf32_sinak = (F32)(sin(lf32_f));
	lf32_cosak = (F32)(cos(lf32_f));
	lf32_sine = (F32)(sin(pstp_TRANCOOR->stf32_ELGeoAng * DEGTORAD));
	lf32_cose = (F32)(cos(pstp_TRANCOOR->stf32_ELGeoAng * DEGTORAD));
	lf32_sinr = (F32)(sin(lf32_fr));
	lf32_cosr = (F32)(cos(lf32_fr));
	lf32_sinp = (F32)(sin(lf32_fp));
	lf32_cosp = (F32)(cos(lf32_fp));
	lf32_f1 = (F32)(lf32_sine * lf32_cosp * lf32_cosr + lf32_cose * lf32_sinak * lf32_sinr 
			- lf32_cose * lf32_cosak * lf32_sinp * lf32_cosr);
	lf32_f2 = (F32)(lf32_cose * lf32_cosak * lf32_cosak * lf32_cosp + lf32_sine * lf32_cosak * lf32_sinp
			+lf32_cose * lf32_cosak * lf32_sinak * lf32_sinp * lf32_sinr-lf32_sine * lf32_sinak * lf32_cosp * lf32_sinr
			+lf32_cose * lf32_sinak * lf32_sinak * lf32_cosr);
	lf32_f = (F32)(atan2(lf32_f1, lf32_f2));
	lf32_f = (F32)(lf32_f * RADTODEG);
	pstp_TRANCOOR->stf32_ELDeckAng = lf32_f;                                  
	
	//交叉目标甲板角
	lf32_f = (F32)(asin(lf32_cose*lf32_cosak*lf32_cosak*lf32_sinp*lf32_sinr-lf32_sine*lf32_cosp*lf32_sinr*lf32_cosak+lf32_cose*lf32_sinak*lf32_cosak*lf32_cosr
			-lf32_cose*lf32_cosak*lf32_sinak*lf32_cosp-lf32_sine*lf32_sinp*lf32_sinak));
	lf32_f = (F32)(lf32_f * RADTODEG);
	pstp_TRANCOOR->stf32_CRSDeckAng = lf32_f;

	//极化目标甲板角
	lf32_f = (F32)(pstp_TRANCOOR->stf32_ELDeckAng * DEGTORAD);
	lf32_sine = (F32)(sin(lf32_f));
	lf32_cose = (F32)(cos(lf32_f));
	lf32_f = (F32)(pstp_TRANCOOR->stf32_AZDeckAng * DEGTORAD);	//方位甲板角换算
	lf32_sinak = (F32)(sin(lf32_f));
	lf32_cosak = (F32)(cos(lf32_f));
	lf32_f = (F32)(atan2((lf32_sinak * lf32_sinp + lf32_cosak * lf32_sinr * lf32_cosp) * lf32_cose, lf32_cosr * lf32_cosp));
	lf32_f = (F32)(lf32_f * RADTODEG);
	lf32_f = pstp_TRANCOOR->stf32_POLGeoAng + lf32_f;
	pstp_TRANCOOR->stf32_POLDeckAng = lf32_f;
}

/********************************************************
函数名称：MDL_TranCoor_DeckToGeo_AECP
函数功能：AEC座架甲板角转变为地理角
入口参数：
stf32_Heading;				//航向(0~359.999)
stf32_Roll;					//横倾(-90.00~90.00)
stf32_Pitch;					//纵倾(-90.00~90.00)
stf32_AZDeckAng;			//方位甲板角度(0~359.999)
stf32_ELDeckAng;			//俯仰甲板角度(-180.000~179.999)
stf32_POLDeckAng;			//极化甲板角度(-180.000~179.999)
stf32_CRSDeckAng;			//交叉甲板角度(-180.000~179.999)
出口参数：
stf32_AZGeoAng;				//方位地理角度
stf32_ELGeoAng;				//俯仰地理角度
stf32_POLGeoAng;			//极化地理角度
备    注：
********************************************************/	
void MDL_TranCoor_DeckToGeo_AECP(PPARAMETER_TranCoor pstp_TRANCOOR)		        //大地角计算函数
{
	F32 lf32_f,lf32_f1,lf32_f2,lf32_fa,lf32_fe,lf32_fc,lf32_fk,lf32_fr,lf32_fp;
	F32 lf32_sina,lf32_cosa,lf32_sine,lf32_cose,lf32_sinc,lf32_cosc,lf32_sinr,lf32_cosr,lf32_sinp,lf32_cosp,lf32_sink,lf32_cosk;

	lf32_fa = (F32)(pstp_TRANCOOR->stf32_AZDeckAng * DEGTORAD);		//方位甲板角
	lf32_fe = (F32)(pstp_TRANCOOR->stf32_ELDeckAng * DEGTORAD);		//俯仰甲板角
	lf32_fc = (F32)(pstp_TRANCOOR->stf32_CRSDeckAng * DEGTORAD);		//交叉甲板角
	lf32_fk = (F32)(pstp_TRANCOOR->stf32_Heading * DEGTORAD);			//航向角
	lf32_fr = (F32)(pstp_TRANCOOR->stf32_Roll * DEGTORAD);			//横滚角
	lf32_fp = (F32)(pstp_TRANCOOR->stf32_Pitch * DEGTORAD);			//纵倾角
	lf32_sina = (F32)(sin(lf32_fa));
	lf32_cosa = (F32)(cos(lf32_fa));
	lf32_sine = (F32)(sin(lf32_fe));
	lf32_cose = (F32)(cos(lf32_fe));
	lf32_sinc = (F32)(sin(lf32_fc));
	lf32_cosc = (F32)(cos(lf32_fc));
	lf32_sink = (F32)(sin(lf32_fk));
	lf32_cosk = (F32)(cos(lf32_fk));
	lf32_sinr = (F32)(sin(lf32_fr));
	lf32_cosr = (F32)(cos(lf32_fr));
	lf32_sinp = (F32)(sin(lf32_fp));
	lf32_cosp = (F32)(cos(lf32_fp));
	lf32_f = (F32)(asin(lf32_cosa*lf32_cose*lf32_cosc*lf32_sinp-lf32_sina*lf32_sinc*lf32_sinp
			+lf32_sine*lf32_cosc*lf32_cosp*lf32_cosr-lf32_sina*lf32_cose*lf32_cosc*lf32_sinr*lf32_cosp
			-lf32_cosa*lf32_sinc*lf32_sinr*lf32_cosp));
	lf32_f = (F32)(lf32_f * RADTODEG);
	pstp_TRANCOOR->stf32_ELGeoAng = lf32_f;		        //天线俯仰地理角	
	
	lf32_f1 = (F32)(lf32_cosa*lf32_cose*lf32_cosc*lf32_cosp*lf32_sink-lf32_sina*lf32_sinc*lf32_cosp*lf32_sink
		-lf32_sine*lf32_cosc*lf32_cosr*lf32_sinp*lf32_sink+lf32_sina*lf32_cose*lf32_cosc*lf32_sinr*lf32_sinp*lf32_sink
		+lf32_cosa*lf32_sinc*lf32_sinr*lf32_sinp*lf32_sink+lf32_sine*lf32_cosc*lf32_sinr*lf32_cosk
		+lf32_sina*lf32_cose*lf32_cosc*lf32_cosr*lf32_cosk+lf32_cosa*lf32_sinc*lf32_cosr*lf32_cosk);
	lf32_f2 = (F32)(lf32_cosa*lf32_cose*lf32_cosc*lf32_cosp*lf32_cosk-lf32_sina*lf32_sinc*lf32_cosp*lf32_cosk
		-lf32_sine*lf32_cosc*lf32_cosr*lf32_sinp*lf32_cosk+lf32_sina*lf32_cose*lf32_cosc*lf32_sinr*lf32_sinp*lf32_cosk
		+lf32_cosa*lf32_sinc*lf32_sinr*lf32_sinp*lf32_cosk-lf32_sine*lf32_cosc*lf32_sinr*lf32_sink
		-lf32_sina*lf32_cose*lf32_cosc*lf32_cosr*lf32_sink-lf32_cosa*lf32_sinc*lf32_cosr*lf32_sink);
	lf32_f = (F32)(atan2(lf32_f1, lf32_f2));
	lf32_f = (F32)(lf32_f * RADTODEG);
	if(lf32_f < 0)
	{
		lf32_f += 360.0;
	}
	else if (lf32_f < 0)
	{
		lf32_f -= 360.0;
	}
	pstp_TRANCOOR->stf32_AZGeoAng = lf32_f;		        //天线方位地理角
	
	lf32_f = (F32)(atan2((lf32_sina * lf32_sinp + lf32_cosa * lf32_sinr * lf32_cosp) * lf32_cose, lf32_cosr * lf32_cosp));
	lf32_f = (F32)(lf32_f * RADTODEG);
	lf32_f = pstp_TRANCOOR->stf32_POLDeckAng - lf32_f;
	pstp_TRANCOOR->stf32_POLGeoAng = lf32_f;		        //天线极化地理角
}

/********************************************************
函数名称：MDL_SatGeoAngCal
函数功能：根据卫星经度、站经纬度计算目标地理角度
入口参数：
stf32_SatLong;				//卫星经度(-360.000~360.000)
stf32_LocLong;				//当前经度(-180.000~179.999)
stf32_LocLat;				//当前纬度(-90.000~90.000)
stu8_POLMode;				//极化方式(0-水平极化；1-垂直极化)
出口参数：
stf32_AZAng;				//方位角度(0~359.999)
stf32_ELAng;				//俯仰角度(0~90.000)
stf32_POLAng;				//极化角度(-90.0~90.000)
备    注：
********************************************************/
void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng)
{
	F32 lf32_satlong, lf32_localong, lf32_localat;
	F32 lf32_temp;
	
	lf32_satlong = (F32)(pstp_SatGeoAng->stf32_SatLong * DEGTORAD);		//卫星经度 角度转弧度
	lf32_localong = (F32)(pstp_SatGeoAng->stf32_LocLong * DEGTORAD);		//本地经度 角度转弧度
	
	//本地纬度
	//因为纬度要作为分母参与运算，所以此处必须对低纬度进行判断和保护，以免出现“除以0”的错误
	//程序中设置的纬度精度为0.01°，所以此处限制的最小角度为0.01
	if ((pstp_SatGeoAng->stf32_LocLat <= -0.01)||(pstp_SatGeoAng->stf32_LocLat  >= 0.01))
	{
		lf32_localat = (F32)(pstp_SatGeoAng->stf32_LocLat * DEGTORAD); 	//纬度值不为0，即不在赤道上
	}
	else if (pstp_SatGeoAng->stf32_LocLat  < 0.00)
	{
		lf32_localat = (F32)(-RADMIN);  								//南半球低纬度
	}
	else 
	{
		lf32_localat = (F32)(RADMIN);									//北半球低纬度
	}
	
	//方位目标地理角度计算
	lf32_temp = (F32)(atan(tan(lf32_satlong - lf32_localong) / sin(lf32_localat)) * RADTODEG);  //(-90.0 90.0) 一或四象限
	lf32_temp = (F32)(180.0 - lf32_temp);
	if(lf32_temp < 0) 
	{
		lf32_temp += 360.0;
	}
	if(lf32_localat < 0)												//天线处于南半球的计算
	{
		lf32_temp += 180.0;
		if(lf32_temp >= 360.0)
		{
			lf32_temp = (F32)(lf32_temp - 360.0);
		}
	}
	pstp_SatGeoAng->stf32_AZAng = lf32_temp;	
	
	//俯仰目标地理角度计算
	//地球赤道半径 r = 6378.140km
	//同步轨道卫星高度（距地面） h = 35785.965km
	//同步轨道卫星半径（距地心） H = h + r = 42164.105km
	//则 r/R = r/(r+h) = 6378.140/42164.105 = 0.15126859683135690891577089090353
	lf32_temp = (F32)(atan((cos(lf32_satlong - lf32_localong) * cos(lf32_localat) - 0.1512686)
				/ sqrt(1.0 - cos(lf32_localat) * cos(lf32_satlong-lf32_localong) * cos(lf32_localat) * cos(lf32_satlong - lf32_localong))));
	pstp_SatGeoAng->stf32_ELAng = (F32)(lf32_temp * RADTODEG);
	
	//极化目标地理角度计算（角度以水平极化为0计算得出）
	lf32_temp = (F32)(atan(sin(lf32_satlong - lf32_localong) / tan(lf32_localat)));  //(-90.0 90.0) 一或四象限
	lf32_temp *= (F32)(RADTODEG);
	//垂直极化
	if (1 == pstp_SatGeoAng->stu8_POLMode) 
	{
		lf32_temp += 90.0;	//垂直极化，极化角度相差90°
	}
	if (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;
	}	
	pstp_SatGeoAng->stf32_POLAng = (F32)lf32_temp;
	
	return;	
}

//===========================================================================
// End of file.
//===========================================================================




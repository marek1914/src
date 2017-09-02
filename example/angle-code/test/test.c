#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


typedef float F32;
typedef unsigned char U8;

//用户自定义宏//
#define ERROR		0.0001			//小数
#define PI			3.1415926		//PI
#define DEGTORAD	0.01745329		//角度转换到弧度
#define RADTODEG	57.2957805		//弧度转换到角度
#define RADMIN		0.00017			//弧度最小值 0.01°对应的弧度值(PI / 180 /100)



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

//波束指向
typedef struct _CMD_WAVEDIR
{
	F32 stf32_thita;				//thita角
	F32 stf32_pi;					//pi角
}CMD_WAVEDIR;

void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng);
void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);
void MDL_DeckToBall(PPARAMETER_TranCoor pstp_TRANCOOR, CMD_WAVEDIR * st_WaveDir);
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);					//AE坐标变化极化角偏移计算(过程函数)


int main(int argc, char** argv)
{
	if (argc < 7) {
		return;
	}
	PARAMETER_SatGeoAng lst_SatGeoAng;
	PARAMETER_TranCoor lst_TranCoor;
	CMD_WAVEDIR wavedir;

	memset(&lst_SatGeoAng, 0, sizeof(lst_SatGeoAng));
	memset(&lst_TranCoor, 0, sizeof(lst_TranCoor));

	lst_SatGeoAng.stf32_SatLong = atof(argv[1]);
	lst_SatGeoAng.stu8_POLMode = 0;
	lst_SatGeoAng.stf32_LocLong = atof(argv[2]);
	lst_SatGeoAng.stf32_LocLat = atof(argv[3]);

	//计算卫星方位、俯仰和极化理论角
	MDL_SatGeoAngCal(&lst_SatGeoAng);

	lst_TranCoor.stf32_AZGeoAng = lst_SatGeoAng.stf32_AZAng;		//方位角
	lst_TranCoor.stf32_ELGeoAng = lst_SatGeoAng.stf32_ELAng;		//俯仰角 
	lst_TranCoor.stf32_POLGeoAng = lst_SatGeoAng.stf32_POLAng;		//极化角
	lst_TranCoor.stf32_Heading = atof(argv[4]);						//航向
	lst_TranCoor.stf32_Roll = atof(argv[5]);						//横摇
	lst_TranCoor.stf32_Pitch = atof(argv[6]);						//纵摇

	MDL_TranCoor_GeoToDeck_AEP(&lst_TranCoor);						//AE座架地理角转变为甲板角

	MDL_DeckToBall(&lst_TranCoor, &wavedir);

	printf("%f  %f \n", wavedir.stf32_pi, wavedir.stf32_thita);
	return 0;
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

void MDL_DeckToBall(PPARAMETER_TranCoor pstp_TRANCOOR, CMD_WAVEDIR * st_WaveDir)
{
	F32 lf_Aa,lf_Ea;

	lf_Aa = pstp_TRANCOOR->stf32_AZDeckAng;   //pstp_SETDATA->stf32_XiuZheng 先保留
	lf_Ea = pstp_TRANCOOR->stf32_ELDeckAng;
	if (lf_Aa <= 90)
	{
		st_WaveDir->stf32_pi = 90 - lf_Aa;
	}
	else
	{
		st_WaveDir->stf32_pi = 360 - (lf_Aa - 90);
	}
	st_WaveDir->stf32_pi = 360 - st_WaveDir->stf32_pi;
	while (st_WaveDir->stf32_pi > 360)
	{
		st_WaveDir->stf32_pi = st_WaveDir->stf32_pi - 360;
	}
	while (st_WaveDir->stf32_pi < 0)
	{
		st_WaveDir->stf32_pi = st_WaveDir->stf32_pi + 360;
	}
	st_WaveDir->stf32_thita = 90 - lf_Ea;
}


/********************************************************
函数名称：POLOffset_AEP
函数功能：AE轴坐标变化极化角偏移计算
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


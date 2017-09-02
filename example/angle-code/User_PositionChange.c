/********************************************************
�ļ����ƣ�User_ComFun.c
���ߣ�������
�汾��20131105
˵�������ļ�ΪUser_ComFun��.c�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#include "User_PositionChange.h"

///////////////////////////////////////////////////////////////////
//�������ƣ�CalSatInf
//�������ܣ����㵱ǰָ������ָ��Ƕ�
//��ڲ�����pstp_REVDATA
//���ڲ�����pstp_SEDDATA
//�� 	ע����
///////////////////////////////////////////////////////////////////
void CalSatInf(SETDATA * pstp_SETDATA)
{
	PARAMETER_SatGeoAng lst_SatGeoAng;
	PARAMETER_TranCoor lst_TranCoor;
	//F32 lf32_Temp;
	//U16 lu16_SatNo;

	//pstp_SETDATA->stf32_SatLong = 110.5;
    //pstp_SETDATA->stf32_Long = 114.43;
	//pstp_SETDATA->stf32_Latitude = 38.04;
	//��������ֵ
	memset(&lst_SatGeoAng, 0, sizeof(lst_SatGeoAng));
	memset(&lst_TranCoor, 0, sizeof(lst_TranCoor));
	lst_SatGeoAng.stf32_SatLong = pstp_SETDATA->stf32_SatLong;							//���Ǿ���
	lst_SatGeoAng.stu8_POLMode = 0;														//������ʽ:Բ����
	lst_SatGeoAng.stf32_LocLong = pstp_SETDATA->stf32_Long;								//����
	lst_SatGeoAng.stf32_LocLat = pstp_SETDATA->stf32_Latitude;							//γ��
	
	//�������Ƿ�λ�������ͼ������۽�
	MDL_SatGeoAngCal(&lst_SatGeoAng);

	lst_TranCoor.stf32_AZGeoAng = lst_SatGeoAng.stf32_AZAng;							//��λ��
	lst_TranCoor.stf32_ELGeoAng = lst_SatGeoAng.stf32_ELAng;							//������ 
	lst_TranCoor.stf32_POLGeoAng = lst_SatGeoAng.stf32_POLAng;							//������
	lst_TranCoor.stf32_Heading = pstp_SETDATA->stf32_Heading;							//����
	lst_TranCoor.stf32_Roll = pstp_SETDATA->stf32_Roll;									//��ҡ
	lst_TranCoor.stf32_Pitch = pstp_SETDATA->stf32_Pitch;								//��ҡ

	MDL_TranCoor_GeoToDeck_AEP(&lst_TranCoor);											//AE���ܵ����ת��Ϊ�װ��

	MDL_DeckToBall(&lst_TranCoor,pstp_SETDATA);
	
}

/********************************************************
�������ƣ�MDL_SatGeoAngCal
�������ܣ��������Ǿ��ȡ�վ��γ�ȼ���Ŀ�����Ƕ�
��ڲ�����
stf32_SatLong;				//���Ǿ���(-360.000~360.000)
stf32_LocLong;				//��ǰ����(-180.000~179.999)
stf32_LocLat;				//��ǰγ��(-90.000~90.000)
stu8_POLMode;				//������ʽ(0-ˮƽ������1-��ֱ����)
���ڲ�����
stf32_AZAng;				//��λ�Ƕ�(0~359.999)
stf32_ELAng;				//�����Ƕ�(0~90.000)
stf32_POLAng;				//�����Ƕ�(-90.0~90.000)
��    ע��
********************************************************/
void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng)
{
	F32 lf32_satlong, lf32_localong, lf32_localat;
	F32 lf32_temp;
	
	lf32_satlong = (F32)(pstp_SatGeoAng->stf32_SatLong * DEGTORAD);		//���Ǿ��� �Ƕ�ת����
	lf32_localong = (F32)(pstp_SatGeoAng->stf32_LocLong * DEGTORAD);		//���ؾ��� �Ƕ�ת����
	
	//����γ��
	//��Ϊγ��Ҫ��Ϊ��ĸ�������㣬���Դ˴�����Ե�γ�Ƚ����жϺͱ�����������֡�����0���Ĵ���
	//���������õ�γ�Ⱦ���Ϊ0.01�㣬���Դ˴����Ƶ���С�Ƕ�Ϊ0.01
	if ((pstp_SatGeoAng->stf32_LocLat <= -0.01)||(pstp_SatGeoAng->stf32_LocLat  >= 0.01))
	{
		lf32_localat = (F32)(pstp_SatGeoAng->stf32_LocLat * DEGTORAD); 	//γ��ֵ��Ϊ0�������ڳ����
	}
	else if (pstp_SatGeoAng->stf32_LocLat  < 0.00)
	{
		lf32_localat = (F32)(-RADMIN);  								//�ϰ����γ��
	}
	else 
	{
		lf32_localat = (F32)(RADMIN);									//�������γ��
	}
	
	//��λĿ�����Ƕȼ���
	lf32_temp = (F32)(atan(tan(lf32_satlong - lf32_localong) / sin(lf32_localat)) * RADTODEG);  //(-90.0 90.0) һ��������
	lf32_temp = (F32)(180.0 - lf32_temp);
	if(lf32_temp < 0) 
	{
		lf32_temp += 360.0;
	}
	if(lf32_localat < 0)												//���ߴ����ϰ���ļ���
	{
		lf32_temp += 180.0;
		if(lf32_temp >= 360.0)
		{
			lf32_temp = (F32)(lf32_temp - 360.0);
		}
	}
	pstp_SatGeoAng->stf32_AZAng = lf32_temp;	
	
	//����Ŀ�����Ƕȼ���
	//�������뾶 r = 6378.140km
	//ͬ��������Ǹ߶ȣ�����棩 h = 35785.965km
	//ͬ��������ǰ뾶������ģ� H = h + r = 42164.105km
	//�� r/R = r/(r+h) = 6378.140/42164.105 = 0.15126859683135690891577089090353
	lf32_temp = (F32)(atan((cos(lf32_satlong - lf32_localong) * cos(lf32_localat) - 0.1512686)
		/ sqrt(1.0 - cos(lf32_localat) * cos(lf32_satlong-lf32_localong) * cos(lf32_localat) * cos(lf32_satlong - lf32_localong))));
	pstp_SatGeoAng->stf32_ELAng = (F32)(lf32_temp * RADTODEG);
	
	//����Ŀ�����Ƕȼ��㣨�Ƕ���ˮƽ����Ϊ0����ó���
	lf32_temp = (F32)(atan(sin(lf32_satlong - lf32_localong) / tan(lf32_localat)));  //(-90.0 90.0) һ��������
	lf32_temp *= (F32)(RADTODEG);
	//��ֱ����
	if (1 == pstp_SatGeoAng->stu8_POLMode) 
	{
		lf32_temp += 90.0;	//��ֱ�����������Ƕ����90��
	}
	if (lf32_temp > 90.0)
	{
		lf32_temp -= 180.0;
	}	
	pstp_SatGeoAng->stf32_POLAng = (F32)lf32_temp;
	
	return;	
}


/********************************************************
�������ƣ�MDL_TranCoor_GeoToDeck_AEP
�������ܣ�AE���ܵ����ת��Ϊ�װ��
��ڲ�����
stf32_Heading;				//����(0~359.999)
stf32_Roll;					//����(-90.00~90.00)
stf32_Pitch;					//����(-90.00~90.00)
stf32_AZGeoAng;				//��λ����Ƕ�(0~359.999)
stf32_ELGeoAng;				//��������Ƕ�(-180.000~179.999)
���ڲ�����
stf32_AZDeckAng;			//��λ�װ�Ƕ�
stf32_ELDeckAng;			//�����װ�Ƕ�
stf32_POLDeckAng;			//�����װ�Ƕ�
��    ע������AE��������仯������Ǳ任Ϊ�װ��
********************************************************/
void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x,lf32_y;
	F32 lf32_lf32_coseLAng,lf32_CosAZAng,lf32_CosPitch,lf32_lf32_cosroll;
	F32 lf32_lf32_sineLAng,lf32_SinAZAng,lf32_SinPitch,lf32_lf32_sinroll;
	F32 lf32_temp;

	//�����������sin()��cos()ֵ������������ظ����㣬���Ч��
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
	
	//��λ�װ�� 
	lf32_x = (F32)(lf32_lf32_cosroll * lf32_lf32_coseLAng * lf32_CosAZAng + lf32_lf32_sinroll * lf32_lf32_sineLAng);	
	lf32_y = (F32)(lf32_lf32_coseLAng * (lf32_CosPitch * lf32_SinAZAng + lf32_SinPitch * lf32_lf32_sinroll * lf32_CosAZAng)
			- lf32_SinPitch * lf32_lf32_cosroll * lf32_lf32_sineLAng);	
	if (lf32_x >= 0.0)
    {
		if (lf32_x < RADMIN)
		{
			lf32_x = (F32)RADMIN;							 //0.0  0.01��֮�䣬��Ϊ0.01��
		}
    }
	else
    {
		if (lf32_x > -RADMIN) 
		{
			lf32_x = (F32)-RADMIN;							 //0.0  -0.01��֮�䣬��Ϊ-0.01��
		}
    }	
	lf32_temp = (F32)(atan(lf32_y / lf32_x) * RADTODEG);	 //(-90.0 90.0) һ��������
	if (lf32_x >= 0.0)										 //һ������
    {
		if (lf32_y < 0.0) 
		{
			lf32_temp += 360.0;								 //������
		}
    }
	else
    {
		lf32_temp += 180.0;									 //��������
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
	
	//�����װ�� 
	lf32_x = (F32)(lf32_SinPitch * lf32_lf32_coseLAng * lf32_SinAZAng 
			- lf32_CosPitch * lf32_lf32_sinroll * lf32_lf32_coseLAng * lf32_CosAZAng
			+ lf32_CosPitch * lf32_lf32_cosroll * lf32_lf32_sineLAng);
	
	lf32_temp = (F32)(asin(lf32_x) * RADTODEG);  
	pstp_TRANCOOR->stf32_ELDeckAng= (F32)lf32_temp;

	//�����װ��
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

void MDL_DeckToBall(PPARAMETER_TranCoor pstp_TRANCOOR,SETDATA * pstp_SETDATA)
{
	F32 lf_Aa,lf_Ea;

	lf_Aa = pstp_TRANCOOR->stf32_AZDeckAng;   //pstp_SETDATA->stf32_XiuZheng �ȱ���
	lf_Ea = pstp_TRANCOOR->stf32_ELDeckAng;
	if (lf_Aa <= 90)
	{
		pstp_SETDATA->st_WaveDir.stf32_pi = 90 - lf_Aa;
	}
	else
	{
		pstp_SETDATA->st_WaveDir.stf32_pi = 360 - (lf_Aa - 90);
	}
	pstp_SETDATA->st_WaveDir.stf32_pi = 360 - pstp_SETDATA->st_WaveDir.stf32_pi;
	while (pstp_SETDATA->st_WaveDir.stf32_pi > 360)
	{
		pstp_SETDATA->st_WaveDir.stf32_pi = pstp_SETDATA->st_WaveDir.stf32_pi - 360;
	}
	while (pstp_SETDATA->st_WaveDir.stf32_pi < 0)
	{
		pstp_SETDATA->st_WaveDir.stf32_pi = pstp_SETDATA->st_WaveDir.stf32_pi + 360;
	}
	pstp_SETDATA->st_WaveDir.stf32_thita = 90 - lf_Ea;
}


/********************************************************
�������ƣ�POLOffset_AEP
�������ܣ�AE������仯������ƫ�Ƽ���
��ڲ�����
stf32_Heading;				//����(0~359.999)
stf32_Roll;					//����(-90.00~90.00)
stf32_Pitch;					//����(-90.00~90.00)
stf32_AZGeoAng;				//��λ����Ƕ�(0~359.999)
stf32_ELGeoAng;				//��������Ƕ�(-180.000~179.999)
stf32_AZDeckAng;			//��λ�װ�Ƕ�(0~359.999)
stf32_ELDeckAng;			//�����װ�Ƕ�(-180.000~179.999)
���ڲ�����
lf32_temp;					//����ƫ��ֵ
��    ע��
********************************************************/
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR)
{
	F32 lf32_x, lf32_y;
	F32 lf32_ELAng, lf32_AZAng, lf32_Pitch, lf32_AZAng2;
	F32 lf32_temp;
	
	//�ȱ任Ϊ����
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
			lf32_x = (F32)(RADMIN);							//0.0  0.01��֮�䣬��Ϊ0.01��
		}
    }
	else
    {
		if (lf32_x > -RADMIN)
		{
			lf32_x = (F32)(-RADMIN);						//0.0  -0.01��֮�䣬��Ϊ-0.01��
		}
    }	
	lf32_temp = (F32)(atan2(lf32_y,lf32_x) * RADTODEG);		//-90.0 - 90.0
	
	return lf32_temp;
}
//===========================================================================
// End of file.
//===========================================================================

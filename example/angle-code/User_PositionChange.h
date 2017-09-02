/********************************************************
�ļ����ƣ�User_ComFun.h
���ߣ�������
�汾��20131105
˵�������ļ�ΪUser_ComFun��ͷ�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/


#ifndef USER_COMFUN_H
#define USER_COMFUN_H

#include <string.h>
#include <math.h>

#include "User_GlobalVar.h"			//�û������ȫ�ֱ���
#include "User_MacroCfg.h"			//�û�����ĺ궨��
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
//------------------------------------���ú궨��--------------------------------------

//��λ�������ò���
typedef	struct _PARAMETER_SatGeoAng_STRU
{
	F32 stf32_SatLong;				//���Ǿ���
	F32 stf32_LocLong;				//��ǰ����
	F32 stf32_LocLat;				//��ǰγ��
	
	U8 stu8_POLMode;				//������ʽ
	
	F32 stf32_AZAng;				//��λ�Ƕ�
	F32 stf32_ELAng;				//�����Ƕ�
	F32 stf32_POLAng;				//�����Ƕ�
		
} PARAMETER_SatGeoAng, *PPARAMETER_SatGeoAng;

//����任���ò���
typedef	struct _PARAMETER_TranCoor_STRU
{
	F32 stf32_Heading;				//����
	F32 stf32_Roll;					//����
	F32 stf32_Pitch;				//����	
	
	F32 stf32_AZGeoAng;				//��λ����Ƕ�
	F32 stf32_ELGeoAng;				//��������Ƕ�
	F32 stf32_POLGeoAng;			//��������Ƕ�
	
	F32 stf32_AZDeckAng;			//��λ�װ�Ƕ�
	F32 stf32_ELDeckAng;			//�����װ�Ƕ�
	F32 stf32_POLDeckAng;			//�����װ�Ƕ�
	F32 stf32_CRSDeckAng;			//����װ�Ƕ�
		
} PARAMETER_TranCoor, *PPARAMETER_TranCoor;

//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------��������----------------------------------------
void CalSatInf(SETDATA * pstp_SETDATA);
void MDL_SatGeoAngCal(PPARAMETER_SatGeoAng pstp_SatGeoAng);
void MDL_TranCoor_GeoToDeck_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);
void MDL_DeckToBall(PPARAMETER_TranCoor pstp_TRANCOOR,SETDATA * pstp_SETDATA);
F32 POLOffset_AEP(PPARAMETER_TranCoor pstp_TRANCOOR);					//AE����仯������ƫ�Ƽ���(���̺���)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

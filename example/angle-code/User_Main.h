/********************************************************
�ļ����ƣ�User_Main.h
���ߣ�������
�汾��20131104
˵�������ļ�Ϊ�������ͷ�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#ifndef USER_MAIN_H
#define USER_MAIN_H

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "User_GlobalVar.h"			//�û������ȫ�ֱ���
#include "User_MacroCfg.h"			//�û�����ĺ궨��

#include "DSP28x_Project.h"			// Device Headerfile and Examples Include File
#include "User_InitSys.h"			//�û�������豸��ʼ��
#include "User_Flash.h"				//�û������ȫ�ֱ���
#include "User_ComFun.h"			//�û�����Ĺ�������
#include "User_ComFunction.h"		//�û�����Ĺ�������
#include "User_PositionChange.h"	//�û�����

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------���ݶ���----------------------------------------
TIMERPAR gst_TimerPar;							//������ʱ
SETDATA gst_CMD;								//����ṹ��
STADATA gst_STUS;								//״̬�ṹ��
//-----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------��������----------------------------------------
void InitGlobalVar(void);																//��ʼ��ȫ�ֱ���
void UartA_SendData(UART *pstp_UARTA,STADATA *pstp_StaData, SETDATA * pstp_SETDATA);	//����A���ͺ���																		//˫����ʱ��������:����״̬������ģʽ�����͹ߵ�����ֵ���Һ���ֵ��Ч������״̬�����޾�����ֵ������ֵ��Ч
void UartB_SendData(UART *pstp_UARTB);													//����B���ͺ���
void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret);						//����C���ͺ���

extern void CalSatInf(SETDATA * pstp_SETDATA);
//-----------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_MAIN_H definition


//===========================================================================
// End of file.
//===========================================================================

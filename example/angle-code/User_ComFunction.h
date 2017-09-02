/********************************************************
�ļ����ƣ�User_ComFun.h
���ߣ�������
�汾��20131105
˵�������ļ�ΪUser_ComFun��ͷ�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/


#ifndef USER_COMFUNCTION_H
#define USER_COMFUNCTION_H

#include <string.h>

#include "User_GlobalVar.h"			//�û������ȫ�ֱ���
#include "User_MacroCfg.h"			//�û�����ĺ궨��
//#include "User_Module.h"			//����ģ��
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
//------------------------------------���ú궨��--------------------------------------


//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------��������----------------------------------------
void UartADataDeal(UART*pstp_Uart, SETDATA* pstp_WrkModSet);//2015��12��10��17:31:58
void UartBDataDeal(UART*pstp_Uart);//2015��12��10��17:31:58
void UartC_RcvDataDeal(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS);//2015��12��10��17:31:58

I32 MonPtrl_OutTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1);
I32 MonPtrl_InTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1);

I32 UartC_SendDataDeal(RCVDATA_CMDDEAL* pstp_CmdFrame, I8* pi8a_Buf);				//���ڷ������ݴ����ɽṹ��������

extern void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret);			//����C���ͺ���
extern U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2);								
void UartC_SetInqCmdSend(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS);//��ѯ����������
void Uart_SetWaveDir(UART*pstp_Uart, SETDATA* pstp_WrkModSet);							//����ָ�������
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

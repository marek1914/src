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

#include "User_GlobalVar.h"			//�û������ȫ�ֱ���
#include "User_MacroCfg.h"			//�û�����ĺ궨��
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
//------------------------------------���ú궨��--------------------------------------


//----------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//-----------------------------------��������----------------------------------------
//CAN���
void ComFun_CANASend(U8 pu8_MsgNum, U16 *pu16p_Buf);						//CANA���ͺ���(��Ϣ����)
void ComFun_CANBSend(U8 pu8_MsgNum, U16 *pu16p_Buf);						//CANB���ͺ���(��Ϣ����)

void ComFun_CANAIDSend(U16 pu16_MsgID, U16 *pu16p_Buf);						//CANA���ͺ���(ID����)
void ComFun_CANBIDSend(U16 pu16_MsgID, U16 *pu16p_Buf);						//CANB���ͺ���(ID����)

//С����
U16 ComFun_U16HToL(U16 pu16_Data);											//U16���ݸߵͰ�λ����
U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2);								//����U8ת��ΪU16

U8 *User_strchr(I8 *pi8p_stringToSearch, 
				I32 pi32_StringMaxLen, I8 pi8_charToFind);					//����������pi8_charToFind��һ�γ��ֵ�λ��
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_COMFUN_H definition

//===========================================================================
// End of file.
//===========================================================================

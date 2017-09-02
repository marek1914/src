/********************************************************
�ļ����ƣ�User_ComFun.c
���ߣ�������
�汾��20131105
˵�������ļ�ΪUser_ComFun��.c�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#include "User_ComFun.h"
#include "User_InitSys.h"			//�û�������豸��ʼ��


//CANA���ͺ���(��Ϣ����)
void ComFun_CANASend(U8 pu8_MsgNum, U16 *pu16p_Buf)
{	
	ComFun_CANAIDSend((U16)gia_CANATXCfg[(pu8_MsgNum - 1) * 4 + 2], pu16p_Buf);					//CANA���ͺ���(ID����)
}

//CANB���ͺ���(��Ϣ����)
void ComFun_CANBSend(U8 pu8_MsgNum, U16 *pu16p_Buf)
{
	ComFun_CANBIDSend((U16)gia_CANBTXCfg[(pu8_MsgNum - 1) * 4 + 2], pu16p_Buf);					//CANB���ͺ���(ID����)
}

//CANA���ͺ���(ID����)
void ComFun_CANAIDSend(U16 pu16_MsgID, U16 *pu16p_Buf)
{
	static U16 lu16s_MailBoxIndex = 0;
	struct ECAN_REGS ECanaShadow;
	struct MBOX *lp_MBOX = (struct MBOX *)&ECanaMboxes.MBOX0;
	
	//���Ƚ�ֹ����ʹ�ܣ������޸�ID��Ȼ���޸���Ϻ��ٴ�����)
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.all &= (~((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex)));
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	//����ID�ͳ���
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGID.all = 0x00000000;
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGID.bit.STDMSGID = pu16_MsgID;
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGCTRL.bit.DLC = 8;						//�̶�����8
	//ʹ������
	ECanaShadow.CANME.all |= ((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex));
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	
	//CAN��������ÿ��IDռ4��I16
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDL.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDL.word.LOW_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDH.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDH.word.LOW_WORD = *(pu16p_Buf++);
	
	//����ʹ��	
	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
	ECanaShadow.CANTRS.all |= ((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex));
	ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;	
	
	//ָ����һ������
	lu16s_MailBoxIndex++;
	if (lu16s_MailBoxIndex >= (CANMSGTOTLALCNT - CANARXMAILBOX))
	{
		lu16s_MailBoxIndex = 0;
	}
}

//CANB���ͺ���(ID����)
void ComFun_CANBIDSend(U16 pu16_MsgID, U16 *pu16p_Buf)
{
	static U16 lu16s_MailBoxIndex = 0;
	struct ECAN_REGS ECanbShadow;
	struct MBOX *lp_MBOX = (struct MBOX *)&ECanbMboxes.MBOX0;

	//���Ƚ�ֹ����ʹ�ܣ������޸�ID��Ȼ���޸���Ϻ��ٴ�����)
	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.all &= (~((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex)));
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;
	//����ID�ͳ���
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGID.all = 0x00000000;
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGID.bit.STDMSGID = pu16_MsgID;
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGCTRL.bit.DLC = 8;						//�̶�����8
	//ʹ������
	ECanbShadow.CANME.all |= ((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex));
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;
		
	//CAN��������ÿ��IDռ4��I16
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDL.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDL.word.LOW_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDH.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDH.word.LOW_WORD = *(pu16p_Buf++);
		
	//����ʹ��	
	ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.all |= ((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex));
	ECanbRegs.CANTRS.all = ECanbShadow.CANTRS.all;	

	//ָ����һ������
	lu16s_MailBoxIndex++;
	if (lu16s_MailBoxIndex >= (CANMSGTOTLALCNT - CANBRXMAILBOX))
	{
		lu16s_MailBoxIndex = 0;
	}
}

///////////////////////////////////////////////////////////////////
//�������ƣ�ComFun_U16HToL
//�������ܣ�U16���ݸߵͰ�λ����
//��ڲ�����pu16_Data = ����
//���ڲ�������
//�� 	ע����
///////////////////////////////////////////////////////////////////
U16 ComFun_U16HToL(U16 pu16_Data)
{
	U16 lu16_Temp;
	
	lu16_Temp = (pu16_Data >> 8) & 0x00FF;
	lu16_Temp += (pu16_Data << 8) & 0xFF00;
	
	return lu16_Temp;
}

///////////////////////////////////////////////////////////////////
//�������ƣ�ComFun_U8ToU16
//�������ܣ�����U8ת��ΪU16
//��ڲ�����pu8_Data1 = ����1��pu8_Data2 = ����2
//���ڲ�������
//�� 	ע����
///////////////////////////////////////////////////////////////////
U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2)
{
	U16 lu16_Temp;
	
	lu16_Temp = ((U16)pu8_Data1 << 8) & 0xFF00;
	lu16_Temp += ((U16)pu8_Data2) & 0x00FF;
	
	return lu16_Temp;
}

///////////////////////////////////////////////////////////////////
//�������ƣ�User_strchr
//�������ܣ��ҵ�������pi8_charToFind��һ�γ��ֵ�λ�ã�������ָ���λ�õ�ָ��
//��ڲ�����pi8p_stringToSearch = ���������飬pi32_StringMaxLen = ������󳤶ȣ�pi8_charToFind = ҪѰ�ҵ��ַ�
//���ڲ�������
//�� 	ע����
///////////////////////////////////////////////////////////////////
U8 *User_strchr(I8 * pi8p_stringToSearch, I32 pi32_StringMaxLen, I8 pi8_charToFind)
{
	I32 li32_i;

	U8 *lu8p_ret = NULL;

	if (NULL == pi8p_stringToSearch)
	{
		return lu8p_ret;
	}
	
	for (li32_i = 0; li32_i < pi32_StringMaxLen; li32_i++)
	{
		if (pi8p_stringToSearch[li32_i] == pi8_charToFind)
		{			
			lu8p_ret= (U8 *)&pi8p_stringToSearch[li32_i];
			return lu8p_ret;
		}
	}
	return lu8p_ret;
}
//===========================================================================
// End of file.
//===========================================================================

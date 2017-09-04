/********************************************************
文件名称：User_ComFun.c
作者：王京城
版本：20131105
说明：本文件为User_ComFun的.c文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#include "User_ComFun.h"
#include "User_InitSys.h"			//用户定义的设备初始化


//CANA发送函数(消息发送)
void ComFun_CANASend(U8 pu8_MsgNum, U16 *pu16p_Buf)
{	
	ComFun_CANAIDSend((U16)gia_CANATXCfg[(pu8_MsgNum - 1) * 4 + 2], pu16p_Buf);					//CANA发送函数(ID发送)
}

//CANB发送函数(消息发送)
void ComFun_CANBSend(U8 pu8_MsgNum, U16 *pu16p_Buf)
{
	ComFun_CANBIDSend((U16)gia_CANBTXCfg[(pu8_MsgNum - 1) * 4 + 2], pu16p_Buf);					//CANB发送函数(ID发送)
}

//CANA发送函数(ID发送)
void ComFun_CANAIDSend(U16 pu16_MsgID, U16 *pu16p_Buf)
{
	static U16 lu16s_MailBoxIndex = 0;
	struct ECAN_REGS ECanaShadow;
	struct MBOX *lp_MBOX = (struct MBOX *)&ECanaMboxes.MBOX0;
	
	//首先禁止邮箱使能，才能修改ID，然后修改完毕后再打开邮箱)
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.all &= (~((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex)));
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	//配置ID和长度
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGID.all = 0x00000000;
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGID.bit.STDMSGID = pu16_MsgID;
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MSGCTRL.bit.DLC = 8;						//固定长度8
	//使能邮箱
	ECanaShadow.CANME.all |= ((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex));
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	
	//CAN缓冲区，每个ID占4个I16
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDL.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDL.word.LOW_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDH.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANARXMAILBOX)->MDH.word.LOW_WORD = *(pu16p_Buf++);
	
	//发送使能	
	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
	ECanaShadow.CANTRS.all |= ((U32)0x00000001 << (CANARXMAILBOX + lu16s_MailBoxIndex));
	ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;	
	
	//指向下一个邮箱
	lu16s_MailBoxIndex++;
	if (lu16s_MailBoxIndex >= (CANMSGTOTLALCNT - CANARXMAILBOX))
	{
		lu16s_MailBoxIndex = 0;
	}
}

//CANB发送函数(ID发送)
void ComFun_CANBIDSend(U16 pu16_MsgID, U16 *pu16p_Buf)
{
	static U16 lu16s_MailBoxIndex = 0;
	struct ECAN_REGS ECanbShadow;
	struct MBOX *lp_MBOX = (struct MBOX *)&ECanbMboxes.MBOX0;

	//首先禁止邮箱使能，才能修改ID，然后修改完毕后再打开邮箱)
	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.all &= (~((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex)));
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;
	//配置ID和长度
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGID.all = 0x00000000;
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGID.bit.STDMSGID = pu16_MsgID;
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MSGCTRL.bit.DLC = 8;						//固定长度8
	//使能邮箱
	ECanbShadow.CANME.all |= ((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex));
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;
		
	//CAN缓冲区，每个ID占4个I16
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDL.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDL.word.LOW_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDH.word.HI_WORD = *(pu16p_Buf++);
	(lp_MBOX + lu16s_MailBoxIndex + CANBRXMAILBOX)->MDH.word.LOW_WORD = *(pu16p_Buf++);
		
	//发送使能	
	ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.all |= ((U32)0x00000001 << (CANBRXMAILBOX + lu16s_MailBoxIndex));
	ECanbRegs.CANTRS.all = ECanbShadow.CANTRS.all;	

	//指向下一个邮箱
	lu16s_MailBoxIndex++;
	if (lu16s_MailBoxIndex >= (CANMSGTOTLALCNT - CANBRXMAILBOX))
	{
		lu16s_MailBoxIndex = 0;
	}
}

///////////////////////////////////////////////////////////////////
//函数名称：ComFun_U16HToL
//函数功能：U16数据高低八位互换
//入口参数：pu16_Data = 数据
//出口参数：无
//备 	注：无
///////////////////////////////////////////////////////////////////
U16 ComFun_U16HToL(U16 pu16_Data)
{
	U16 lu16_Temp;
	
	lu16_Temp = (pu16_Data >> 8) & 0x00FF;
	lu16_Temp += (pu16_Data << 8) & 0xFF00;
	
	return lu16_Temp;
}

///////////////////////////////////////////////////////////////////
//函数名称：ComFun_U8ToU16
//函数功能：两个U8转换为U16
//入口参数：pu8_Data1 = 数据1，pu8_Data2 = 数据2
//出口参数：无
//备 	注：无
///////////////////////////////////////////////////////////////////
U16 ComFun_U8ToU16(U8 pu8_Data1, U8 pu8_Data2)
{
	U16 lu16_Temp;
	
	lu16_Temp = ((U16)pu8_Data1 << 8) & 0xFF00;
	lu16_Temp += ((U16)pu8_Data2) & 0x00FF;
	
	return lu16_Temp;
}

///////////////////////////////////////////////////////////////////
//函数名称：User_strchr
//函数功能：找到数组中pi8_charToFind第一次出现的位置，并返回指向该位置的指针
//入口参数：pi8p_stringToSearch = 待搜索数组，pi32_StringMaxLen = 数组最大长度，pi8_charToFind = 要寻找的字符
//出口参数：无
//备 	注：无
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

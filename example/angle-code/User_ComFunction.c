/********************************************************
文件名称：User_ComFun.c
作者：王京城
版本：20131105
说明：本文件为User_ComFun的.c文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#include "User_ComFunction.h"
#include "User_InitSys.h"			//用户定义的设备初始化
#include "User_GlobalVar.h"			//用户定义的全局变量

//串口A收到接收机发过来的数据的解析
void UartADataDeal(UART*pstp_Uart, SETDATA* pstp_CMD)//2015年12月10日17:31:58
{
	U8	lu8_CheckOut=0;
	U8 lu8_Tmp;
	U16	lu16_Temp;
	I32 li32_Index, li32_RcvLen = 0;
	F32 lf32_Temp;
	if(TRUE == pstp_Uart->stu8_RcvFlag)
	{

		//清标志位
		pstp_Uart->stu8_RcvFlag =  FALSE;

		if (0xEE == pstp_Uart->stu8a_RcvDealBuf1[2])
		{
			li32_RcvLen = UARTA_RCVLEN_A;
		}
		else if (0xDD == pstp_Uart->stu8a_RcvDealBuf1[2])
		{
			li32_RcvLen = UARTA_RCVLEN_B;
		}
		else
		{
			li32_RcvLen = UARTA_RCVLEN_A;
		}

		//校验和

		for (lu8_Tmp = 0; lu8_Tmp < (li32_RcvLen - 2);lu8_Tmp++)
		{
			lu8_CheckOut += pstp_Uart->stu8a_RcvDealBuf1[lu8_Tmp];
		}

		if ( (lu8_CheckOut&0x00FF) == pstp_Uart->stu8a_RcvDealBuf1[li32_RcvLen - 2])//校验和正确
		{
			//pstp_Uart->stu8_CheckFlag=TRUE;

			if (0xEE == pstp_Uart->stu8a_RcvDealBuf1[2])	//控制帧 AA改为EE -cy
			{
				li32_Index = 3;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index], pstp_Uart->stu8a_RcvDealBuf1[li32_Index+1]);
				lf32_Temp = (F32)((I16)lu16_Temp) / 100.0;
				if ((lf32_Temp >= -180.00) && (lf32_Temp < 180.00))
				{
					pstp_CMD->stf32_SatLong = lf32_Temp;
				}

				//发送指令标志置TRUE
				pstp_CMD->stu8_SetFlag = TRUE;
				pstp_CMD->st_SetCmd.stu8_CmdType2 = 0x00;
			}
			else if (0xDD == pstp_Uart->stu8a_RcvDealBuf1[2])	//转发帧 BB改为DD	-cy
			{
				li32_Index = 3;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index + 1], pstp_Uart->stu8a_RcvDealBuf1[li32_Index]);
				pstp_CMD->stf32_Heading = (F32)(lu16_Temp) / 100.0;
				li32_Index += 2;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index + 1], pstp_Uart->stu8a_RcvDealBuf1[li32_Index]);
				pstp_CMD->stf32_Pitch = (F32)((I16)lu16_Temp) / 100.0;
				li32_Index += 2;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index + 1], pstp_Uart->stu8a_RcvDealBuf1[li32_Index]);
				pstp_CMD->stf32_Roll = (F32)((I16)lu16_Temp) / 100.0;
				li32_Index += 2;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index + 1], pstp_Uart->stu8a_RcvDealBuf1[li32_Index]);
				pstp_CMD->stf32_Long = (F32)((I16)lu16_Temp) / 100.0;
				li32_Index += 2;

				lu16_Temp = ComFun_U8ToU16(pstp_Uart->stu8a_RcvDealBuf1[li32_Index + 1], pstp_Uart->stu8a_RcvDealBuf1[li32_Index]);
				pstp_CMD->stf32_Latitude = (F32)((I16)lu16_Temp) / 100.0;
			}
		}
		else if( (lu8_CheckOut&0x00FF) != pstp_Uart->stu8a_RcvDealBuf1[li32_RcvLen - 2])
		{
			pstp_Uart->stu8_CheckFlag=FALSE;
		}

		//pstp_Uart->stu8_RcvFlag =  FALSE;

	}

}

//串口B收到接收机发过来的数据的解析
void UartBDataDeal(UART*pstp_Uart)//2015年12月10日17:31:58
{
	if(TRUE == pstp_Uart->stu8_RcvFlag)
	{
		pstp_Uart->stu8a_RcvDealBuf2[0] = pstp_Uart->stu8a_RcvDealBuf2[0];//测试用
	}
}

//串口C:接收数据解析
/*
void UartC_RcvDataDeal(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS)
{
	U8	lu8_check = 0;			//校验
	I8	li8a_Buf1[BUF_LEN];
	U8 *lu8p_str = NULL;
	U8 lu8_count, lu8_BufLen = 0;
	U8 lu8_Line, lu8_Row;
	U16 lu16_Temp;
	I16 li16_Temp, li16_Temp1;
	U32 lu32_Temp = 0;

	I32  li32_Index;

	if(TRUE == pstp_Uart->stu8_RcvFlag)
	{
		pstp_Uart->stu8_RcvFlag = FALSE;				//清接收标志

		lu8_count = 0;
		memset(li8a_Buf1, 0 ,sizeof(li8a_Buf1));
		
		//判断校验和
		lu8p_str = User_strchr((I8 *)pstp_Uart->stu8a_RcvDealBuf3, 100, 'z');
		lu8_BufLen = lu8p_str - pstp_Uart->stu8a_RcvDealBuf3 + 1;
		//接收转义
		if(UARTC_CMDLEN != MonPtrl_InTransferMean((I8 *)pstp_Uart->stu8a_RcvDealBuf3, lu8_BufLen, li8a_Buf1))
		{
			return;		//接收错误
		}
		
		memcpy((void *)&pstp_WrkModSet->st_CmdDeal, li8a_Buf1, UARTC_CMDLEN);

		//计算校验
		for(lu8_count = CHECKOUT_INDEX1;lu8_count <= CHECKOUT_INDEX2;lu8_count++)
		{
			lu8_check ^=  li8a_Buf1[lu8_count];
		}
		
		//校验正确
		if(lu8_check == pstp_WrkModSet->st_CmdDeal.stu8_checkOut )
		{
			pstp_WrkModSet->sti32_TimerCnt = 0;					//收到返回帧，则清超时计数

			//命令类型
			switch (pstp_WrkModSet->st_CmdDeal.stu8_CmdType1)
			{
				//设置命令回执
			case SET_CMD:
				{
					switch (pstp_WrkModSet->st_CmdDeal.stu8_CmdType2)
					{
					//工作命令回执
					case WRKMOD_CMD:	
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//进行下一条命令的发送
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						

						//设置接收频率回执
					case RCVFREQ_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//进行下一条命令的发送
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						

						//设置发射频率回执
					case SENDFREQ_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//进行下一条命令的发送
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						

						//设置星位、极化回执
					case SATPOL_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//进行下一条命令的发送
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						

						//波束指向回执
					case WAVEDIR_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//进行下一条命令的发送
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 3;
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						
						//设置相位校准回执
					case PHASEADJ_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//没有全部发送完成
							if (pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1] < PHASEADJ_MAXCNT)
							{
								pstp_WrkModSet->st_PhaseAdj.stu8_Index++;
							}
							//全部发送完成
							else
							{
								//进行跳过接收相位校准
								pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 2;
								//下一命令的行、列置0
								pstp_WrkModSet->st_SendUnit.stu8_Line = 0;
								pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
							}							
						}

						//数据接收错误
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						//ID错误
						else if (CMDRCV_IDERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_PhaseAdj.stu8_Index =pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1];
						}					
						break;

						//接收相位校准回执
					case RCVPHASEADJ_CMD:					
						break;
						

						//设置发射振元回执
					case SENDUNIT_CMD:

						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//列增加
							pstp_WrkModSet->st_SendUnit.stu8_Row++;
							//一行一行查询
							if (pstp_WrkModSet->st_SendUnit.stu8_Row >= ROW_MAX)
							{
								if (pstp_WrkModSet->st_SendUnit.stu8_Line  < (LINE_MAX - 1))
								{
									pstp_WrkModSet->st_SendUnit.stu8_Line++;
									pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
								}
								else
								{
									//行、列置0
									pstp_WrkModSet->st_SendUnit.stu8_Line = 0;
									pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
									//下一命令行、列置0
									pstp_WrkModSet->st_RcvUnit.stu8_Line = 0;
									pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
									//进行下一条命令的发送
									pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
								}
							}

						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						
						break;
						

						//设置接收振元回执
					case RCVUNIT_CMD:
						//下位机正确接收
						if (CMDRCV_OK == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							//列增加
							pstp_WrkModSet->st_RcvUnit.stu8_Row++;
							//一行一行查询
							if (pstp_WrkModSet->st_RcvUnit.stu8_Row >= ROW_MAX)
							{
								if (pstp_WrkModSet->st_RcvUnit.stu8_Line  < (LINE_MAX - 1))
								{
									pstp_WrkModSet->st_RcvUnit.stu8_Line++;
									pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
								}
								else
								{
									pstp_WrkModSet->st_RcvUnit.stu8_Line = 0;
									pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
									//进行下一条命令的发送
									pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2 + 1;
								}
							}
						}
						else if (CMDRCV_ERR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = pstp_WrkModSet->st_CmdDeal.stu8_CmdType2; //重新发送本条命令
						}
						break;

					default:
						break;
					}
				}//end of case SET_CMD
		
				break;

			//查询命令
			case INQ_CMD:
				{
					//命令类型
					pstp_WrkModSet->st_InqCmd.stu8_CmdType2= (I8)pstp_WrkModSet->st_CmdDeal.stu8_CmdType2;	
					switch (pstp_WrkModSet->st_InqCmd.stu8_CmdType2)
					{
					//工作命令查询
					case SET_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
						//工作
						if (WRKMOD_NORM == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->stu8_WrkMod = WRKMOD_NORM;
						}
						//待机
						else if (WRKMOD_STANDBY == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->stu8_WrkMod = WRKMOD_STANDBY;
						}
						//其他
						else
						{
							pstp_WrkModSet->st_InqCmd.stu8_CmdType2 = SET_CMD;			//接收错误，重新查询
						}					
						break;

					//设置接收频率
					case RCVFREQ_CMD:
						lu32_Temp = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[7];
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[6]) << 8) & 0x0000ff00);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[5]) << 16) & 0x00ff0000);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[4]) << 24) & 0xff000000);
						pstp_STUS->stf32_RcvFrq = (F32)lu32_Temp;

						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
						break;

					//设置发射频率
					case SENDFREQ_CMD:
						lu32_Temp = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[7];
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[6]) << 8) & 0x0000ff00);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[5]) << 16) & 0x00ff0000);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[4]) << 24) & 0xff000000);
						pstp_STUS->stf32_RcvFrq = (F32)lu32_Temp;

						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
						break;

					//设置星位、极化
					case SATPOL_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
						//天通
						if (SAT_TT == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->st_SatPol.stu8_SatNum = SAT_TT;
						}
						//天链
						else if (SAT_TL == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->st_SatPol.stu8_SatNum = SAT_TL;
						}
						else
						{
							pstp_WrkModSet->st_InqCmd.stu8_CmdType2 = SATPOL_CMD;		//接收错误，重新查询
						}
		
						//左旋
						if (POLMODE_CL == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->st_SatPol.stu8_PolMod = POLMODE_CL;
						}
						//右旋
						else if (POLMODE_CR == pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0])
						{
							pstp_STUS->st_SatPol.stu8_PolMod = POLMODE_CR;
						}
						else
						{
							pstp_WrkModSet->st_InqCmd.stu8_CmdType2 = SATPOL_CMD;		//接收错误，重新查询
						}
						break;

					//波束指向
					case WAVEDIR_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2 += 3;		//跳过相位校准，直接到发射振元

						lu32_Temp = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[7];
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[6]) << 8) & 0x0000ff00);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[5]) << 16) & 0x00ff0000);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[4]) << 24) & 0xff000000);
						pstp_STUS->st_WaveDir.stf32_pi = (F32)lu32_Temp / 10000;

						lu32_Temp = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[3];
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[2]) << 8) & 0x0000ff00);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1]) << 16) & 0x00ff0000);
						lu32_Temp |= ((((U32)pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0]) << 24) & 0xff000000);
						pstp_STUS->st_WaveDir.stf32_thita= (F32)lu32_Temp / 10000;
						break;

					//相位校准查询
					case PHASEADJ_CMD:
						//判断索引范围
						if(pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0] >= 0 &&
							pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0] < PHASEADJ_MAXCNT)
						{
							pstp_STUS->st_PhaseAdj.stu8_Index = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0];
							memcpy((void *)&pstp_STUS->st_PhaseAdj.stu8a_Phase[pstp_WrkModSet->st_PhaseAdj.stu8_Index],
									(void *)&pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1], PHASEADJ_PACKLEN);													
						}
						else
						{
							pstp_WrkModSet->st_InqCmd.stu8_CmdType2 += 2;			//跳过接收相位校准
							//清下一个查询的索引
							pstp_WrkModSet->st_SendUnit.stu8_Line = 0;
							pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
						}
						break;

					//接收相位校准
					case RCVPHASEADJ_CMD:
						break;

					//发射裨檠�
					case SENDUNIT_CMD:
						pstp_WrkModSet->st_SendUnit.stu8_Row++;
						//一行一行查询
						if (pstp_WrkModSet->st_SendUnit.stu8_Row >= ROW_MAX)
						{
							if (pstp_WrkModSet->st_SendUnit.stu8_Line  < (LINE_MAX - 1))
							{
								pstp_WrkModSet->st_SendUnit.stu8_Line++;
								pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
							}
							else
							{
								pstp_WrkModSet->st_SendUnit.stu8_Line = 0;
								pstp_WrkModSet->st_SendUnit.stu8_Row = 0;
								pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
								//清下一个查询的索引
								pstp_WrkModSet->st_RcvUnit.stu8_Line = 0;
								pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
							}
						}

						//行号
						lu8_Line = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0];
						//列号
						lu8_Row = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1];
											
						if ((lu8_Line < LINE_MAX) && (lu8_Row < ROW_MAX))
						{
							pstp_STUS->st_SendUnit.stu8a_Dim[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[2];
							pstp_STUS->st_SendUnit.stu8a_Phase[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[3];
							pstp_STUS->st_SendUnit.stu8a_Power[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[4];						
						}
						break;

					//接收振元查询
					case RCVUNIT_CMD:
						pstp_WrkModSet->st_RcvUnit.stu8_Row++;
						//一行一行查询
						if (pstp_WrkModSet->st_RcvUnit.stu8_Row >= ROW_MAX)
						{
							if (pstp_WrkModSet->st_RcvUnit.stu8_Line  < (LINE_MAX - 1))
							{
								pstp_WrkModSet->st_RcvUnit.stu8_Line++;
								pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
							}
							else
							{
								pstp_WrkModSet->st_RcvUnit.stu8_Line = 0;
								pstp_WrkModSet->st_RcvUnit.stu8_Row = 0;
								pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
								//清下一个查询的索引
								pstp_WrkModSet->st_Temp.stu8_Line = 0;
								pstp_WrkModSet->st_Temp.stu8_Row = 0;
							}
						}
						
						//行号
						lu8_Line = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0];
						//列号
						lu8_Row = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[1];
						
						if ((lu8_Line < LINE_MAX) && (lu8_Row < ROW_MAX))
						{
							pstp_STUS->st_RcvUnit.stu8a_Dim[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[2];
							pstp_STUS->st_RcvUnit.stu8a_Phase[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[3];
							pstp_STUS->st_RcvUnit.stu8a_Power[lu8_Line][lu8_Row] = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[4];						
						}					
						break;
				
						//温度命令
					case TEMP_CMD:
						pstp_WrkModSet->st_Temp.stu8_Row++;
						//一行一行查询
						if (pstp_WrkModSet->st_Temp.stu8_Row >= ROW_MAX)
						{
							if (pstp_WrkModSet->st_Temp.stu8_Line  < (LINE_MAX - 1))
							{
								pstp_WrkModSet->st_Temp.stu8_Line++;
								pstp_WrkModSet->st_Temp.stu8_Row = 0;
							}
							else
							{
								pstp_WrkModSet->st_Temp.stu8_Line = 0;
								pstp_WrkModSet->st_Temp.stu8_Row = 0;
								pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
								//清下一个查询的索引
								pstp_WrkModSet->st_UCW.stu8_Line = 0;
								pstp_WrkModSet->st_UCW.stu8_Row = 0;
							}
						}
	
						li32_Index = 0;						//行号
						lu8_Line = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						//列号
						lu8_Row = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];

						li16_Temp = (I16)ComFun_U8ToU16(pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index], 
													pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index + 1]);

						if ((lu8_Line < LINE_MAX) && (lu8_Row < ROW_MAX))
						{
							pstp_STUS->st_Temp.stf32a_T[lu8_Line][lu8_Row] = (F32)li16_Temp * 0.25;
						}
			
						break;
							
						//电流电压命令
					case VOLCR_CMD:
						pstp_WrkModSet->st_UCW.stu8_Row++;
						//一行一行查询
						if (pstp_WrkModSet->st_UCW.stu8_Row >= ROW_MAX)
						{
							if (pstp_WrkModSet->st_UCW.stu8_Line  < (LINE_MAX - 1) )
							{
								pstp_WrkModSet->st_UCW.stu8_Line++;
								pstp_WrkModSet->st_UCW.stu8_Row = 0;
							}
							else
							{
								pstp_WrkModSet->st_UCW.stu8_Line = 0;
								pstp_WrkModSet->st_UCW.stu8_Row = 0;
								pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
							}
						}

						
						li32_Index = 0;						//行号
						lu8_Line = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						//列号
						lu8_Row = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];

						li16_Temp = (I16)ComFun_U8ToU16(pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index], 
													pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index + 1]);
						li32_Index += 2;
		

						li16_Temp1 = (I16)ComFun_U8ToU16(pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index], 
													pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index + 1]);

						if ((lu8_Line < LINE_MAX) && (lu8_Row < ROW_MAX))
						{
							pstp_STUS->st_UCW.stf32a_Cur[lu8_Line][lu8_Row] = (F32)li16_Temp / 1000;
							pstp_STUS->st_UCW.stf32a_Vol[lu8_Line][lu8_Row] = (F32)li16_Temp1 / 1000;
							pstp_STUS->st_UCW.stf32a_Power[lu8_Line][lu8_Row] = pstp_STUS->st_UCW.stf32a_Vol[lu8_Line][lu8_Row] *
																				pstp_STUS->st_UCW.stf32a_Cur[lu8_Line][lu8_Row];	
						}
						break;
							
						//查询FPGA是否启动
					case BOOT_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;

						pstp_STUS->stu8_Boot = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[0];		//启动状态
						break;
									
					//查询FPGA最后修改日期
					case FPGADATE_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2++;
						
						li32_Index = 0;
						lu16_Temp = (pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index] - 12)*100 +
													pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index + 1] - 6;
						pstp_STUS->st_FPGA.stu16_Year = lu16_Temp;
						li32_Index += 2;

						pstp_STUS->st_FPGA.stu8_Month = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						pstp_STUS->st_FPGA.stu8_Day = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						break;

					//查询DSP最后修改日期
					case DSPDATE_CMD:
						pstp_WrkModSet->st_InqCmd.stu8_CmdType2 = 0;			//重新开始
						
						li32_Index = 0;
						lu16_Temp = (pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index] - 12) * 100 +
									pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index + 1] - 6;

						pstp_STUS->st_DSP.stu16_Year = lu16_Temp;
						li32_Index += 2;
						
						pstp_STUS->st_DSP.stu8_Month = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						pstp_STUS->st_DSP.stu8_Day = pstp_WrkModSet->st_CmdDeal.stu8a_CmdData[li32_Index++];
						break;
							
					default:
						break;
					}
					break;
				}//end of case INQ_CMD
			
			default:
				break;
			}//end of switch (pstp_WrkModSet->st_CmdDeal.stu8_CmdType1)
			
			//接收到回执，可以进行下一帧的发送
			pstp_WrkModSet->stu8_SendFlag = TRUE;
		}//end of 	if(li8a_check == pstp_WrkModSet->st_CmdDeal.stu8_checkOut )
		else
		{
			lu16_Temp = 0;
			return ;
		}
	}//end of if(TRUE == pstp_Uart->stu8_RcvFlag)
}*/

//串口C:发送数据组包
/*
I32 UartC_SendDataDeal(RCVDATA_CMDDEAL* pstp_CmdFrame, I8* pi8a_Buf)
{
	U8	lu8_check = 0,
		lu8_count;
	I32 li32_Len = 0;

	I8 li8a_Buf[BUF_LEN];
	memset(li8a_Buf, 0, BUF_LEN);

	pstp_CmdFrame->stu8_FramHead = FRAMEHEAD;
	pstp_CmdFrame->stu8_FramTail = FRAMETAIL;

	memcpy(li8a_Buf, (void *)pstp_CmdFrame, UARTC_CMDLEN);

	//计算校验
	for(lu8_count = CHECKOUT_INDEX1;lu8_count <= CHECKOUT_INDEX2;lu8_count ++)
	{
		lu8_check ^=  li8a_Buf[lu8_count];
	}
	lu8_count = UARTC_CMDLEN;

	pstp_CmdFrame->stu8_checkOut = lu8_check;

	li8a_Buf[11] = pstp_CmdFrame->stu8_checkOut;

	li32_Len = MonPtrl_OutTransferMean(li8a_Buf,lu8_count, pi8a_Buf);

	return li32_Len;
}
*/
//串口C:设置、查询命令
/*
void UartC_SetInqCmdSend(UART*pstp_Uart, SETDATA* pstp_WrkModSet, STADATA *pstp_STUS)
{
	I32 li32_BufLen = 0;
	
	I8 li8a_Buf[BUF_LEN];
	U32 lu32_Temp;
	U8	lu8_Line , lu8_Row;

	memset(li8a_Buf, 0, BUF_LEN);

	//超时检测，超过50ms未收到回复，则进行重发
	if (pstp_WrkModSet->sti32_TimerCnt > 5)
	{
		pstp_WrkModSet->sti32_TimerCnt = 0;
		pstp_WrkModSet->stu8_SendFlag = TRUE;				//超时重发
	}

	//发送允许
	if (pstp_WrkModSet->stu8_SendFlag)
	{

		//设置命令有效，则进行设置，暂停查询
		if (pstp_WrkModSet->stu8_SetFlag)
		{
			//发送结构体清0
			memset((void *)&pstp_WrkModSet->st_SetCmd.stu8a_CmdData, 0, 8);
			pstp_WrkModSet->st_SetCmd.stu8_CmdType1 = SET_CMD;
			
			switch (pstp_WrkModSet->st_SetCmd.stu8_CmdType2)
			{
				//工作命令
			case WRKMOD_CMD:			
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = pstp_WrkModSet->stu8_WrkMod;
				break;
				
				//接收频率命令
			case RCVFREQ_CMD:
				lu32_Temp = ((U32)pstp_WrkModSet->stf32_RcvFrq) * 1000;

				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[7] = (U8)(lu32_Temp & 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[6] = (U8)((lu32_Temp >> 8)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[5] = (U8)((lu32_Temp >> 16)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = (U8)((lu32_Temp >> 24)& 0x000000ff);
				break;
				
				//发射频率命令
			case SENDFREQ_CMD:
				lu32_Temp = ((U32)pstp_WrkModSet->stf32_SendFrq) * 1000;
				
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[7] = (U8)(lu32_Temp & 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[6] = (U8)((lu32_Temp >> 8)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[5] = (U8)((lu32_Temp >> 16)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = (U8)((lu32_Temp >> 24)& 0x000000ff);							
				break;
				
				//星位、极化命令
			case SATPOL_CMD:				
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = pstp_WrkModSet->st_SatPol.stu8_SatNum;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1] = pstp_WrkModSet->st_SatPol.stu8_PolMod;
				break;
				
				//波束指向命令
			case WAVEDIR_CMD:
				lu32_Temp = ((U32)pstp_WrkModSet->st_WaveDir.stf32_thita) * 10000;
				
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[3] = (U8)(lu32_Temp & 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[2] = (U8)((lu32_Temp >> 8)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1] = (U8)((lu32_Temp >> 16)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = (U8)((lu32_Temp >> 24)& 0x000000ff);

				lu32_Temp = ((U32)pstp_WrkModSet->st_WaveDir.stf32_pi) * 10000;
				
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[7] = (U8)(lu32_Temp & 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[6] = (U8)((lu32_Temp >> 8)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[5] = (U8)((lu32_Temp >> 16)& 0x000000ff);
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = (U8)((lu32_Temp >> 24)& 0x000000ff);
				break;
				
				//相位校准命令
			case PHASEADJ_CMD:
				//发送索引
				if (pstp_WrkModSet->st_PhaseAdj.stu8_Index < (PHASEADJ_MAXCNT - 1))
				{
					memcpy((void *)&pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1],
						(void *)&pstp_WrkModSet->st_PhaseAdj.stu8a_Phase[pstp_WrkModSet->st_PhaseAdj.stu8_Index], PHASEADJ_PACKLEN);
					pstp_WrkModSet->st_PhaseAdj.stu8_Index++;
				}
				else if (pstp_WrkModSet->st_PhaseAdj.stu8_Index == (PHASEADJ_MAXCNT - 1))
				{
					memcpy((void *)&pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1],
						(void *)&pstp_WrkModSet->st_PhaseAdj.stu8a_Phase[pstp_WrkModSet->st_PhaseAdj.stu8_Index], PHASEADJ_PACKLEN);
					pstp_WrkModSet->st_PhaseAdj.stu8_Index++;
				}
				
				break;
				
				//接收相位校准命令
			case RCVPHASEADJ_CMD:

				break;
				
				//发射振元命令
			case SENDUNIT_CMD:
				lu8_Line = pstp_WrkModSet->st_SendUnit.stu8_Line;
				lu8_Row = pstp_WrkModSet->st_SendUnit.stu8_Row;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = lu8_Line;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1] = lu8_Row;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[2] = pstp_WrkModSet->st_SendUnit.stu8a_Dim[lu8_Line][lu8_Row];
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[3] = pstp_STUS->st_SendUnit.stu8a_Dim[lu8_Line][lu8_Row];
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = pstp_STUS->st_SendUnit.stu8a_Dim[lu8_Line][lu8_Row];

				break;
				//接收振元命令
			case RCVUNIT_CMD:
				lu8_Line = pstp_WrkModSet->st_RcvUnit.stu8_Line;
				lu8_Row = pstp_WrkModSet->st_RcvUnit.stu8_Row;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = lu8_Line;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1] = lu8_Row;
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[2] = pstp_WrkModSet->st_RcvUnit.stu8a_Dim[lu8_Line][lu8_Row];
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[3] = pstp_STUS->st_RcvUnit.stu8a_Dim[lu8_Line][lu8_Row];
				pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = pstp_STUS->st_RcvUnit.stu8a_Dim[lu8_Line][lu8_Row];

				pstp_WrkModSet->stu8_SetFlag = FALSE;
				break;

			default:
				break;
			}
			//发送命令转义处理
			li32_BufLen = UartC_SendDataDeal(&pstp_WrkModSet->st_SetCmd, li8a_Buf);
		}
		//进行查询
		else
		{
			//发送结构体清0
			memset((void *)&pstp_WrkModSet->st_InqCmd.stu8a_CmdData, 0, 8);
			//命令类型
			pstp_WrkModSet->st_InqCmd.stu8_CmdType1 = INQ_CMD;	
			switch (pstp_WrkModSet->st_InqCmd.stu8_CmdType2)
			{
				//查询工作模式
			case WRKMOD_CMD:						
				//查询接收频率
			case RCVFREQ_CMD:		
				//查询发射频率
			case SENDFREQ_CMD:
				//查询星位、极化
			case SATPOL_CMD:
				//查询波束指向
			case WAVEDIR_CMD:
				//查询相位校准
			case PHASEADJ_CMD:
				//接收相位校准
			case RCVPHASEADJ_CMD:
				//发送命令转义处理
				break;

				//查询发射振元
			case SENDUNIT_CMD:
				//行号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[0] = pstp_WrkModSet->st_SendUnit.stu8_Line;
				//列号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[1]= pstp_WrkModSet->st_SendUnit.stu8_Row;
				break;

				//查询接收振元
			case RCVUNIT_CMD:
				//行号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[0] = pstp_WrkModSet->st_RcvUnit.stu8_Line;
				//列号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[1]= pstp_WrkModSet->st_RcvUnit.stu8_Row;
				break;
				
				//温度命令
			case TEMP_CMD:
				//行号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[0] = pstp_WrkModSet->st_Temp.stu8_Line;
				//列号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[1]= pstp_WrkModSet->st_Temp.stu8_Row;
				break;
							
				//电流电压命令
			case VOLCR_CMD:
				//行号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[0]= pstp_WrkModSet->st_UCW.stu8_Line;
				//列号
				pstp_WrkModSet->st_InqCmd.stu8a_CmdData[1] = pstp_WrkModSet->st_UCW.stu8_Row;				
				break;
							
				//查询FPGA是否启动
			case BOOT_CMD:	
				//查询FPGA最后修改日期
			case FPGADATE_CMD:
				//查询DSP最后修改日期
			case DSPDATE_CMD:
				break;
													
			default:
				break;
			}
			//发送命令转义处理
			li32_BufLen = UartC_SendDataDeal(&pstp_WrkModSet->st_InqCmd, li8a_Buf);
		}

		if (li32_BufLen >= UARTC_CMDLEN)
		{
			UartC_SendData(pstp_Uart, li8a_Buf, li32_BufLen);
		}

		pstp_WrkModSet->stu8_SendFlag = FALSE;
	}
}

//波束指向角设置，100ms定时设置
void Uart_SetWaveDir(UART*pstp_Uart, SETDATA* pstp_WrkModSet)
{
	U32 lu32_Temp;
	I32 li32_BufLen = 0;
	
	I8 li8a_Buf[BUF_LEN];
	
	memset(li8a_Buf, 0, BUF_LEN);

	//没有设置命令时，100ms进行波束指向角设置
	if (!pstp_WrkModSet->stu8_SetFlag)
	{
		pstp_WrkModSet->st_SetCmd.stu8_CmdType1 = SET_CMD;
		
		pstp_WrkModSet->st_SetCmd.stu8_CmdType2 = WAVEDIR_CMD;
		
		
		lu32_Temp = ((U32)pstp_WrkModSet->st_WaveDir.stf32_thita) * 10000;
		
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[3] = (U8)(lu32_Temp & 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[2] = (U8)((lu32_Temp >> 8)& 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[1] = (U8)((lu32_Temp >> 16)& 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[0] = (U8)((lu32_Temp >> 24)& 0x000000ff);
		
		lu32_Temp = ((U32)pstp_WrkModSet->st_WaveDir.stf32_pi) * 10000;
		
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[7] = (U8)(lu32_Temp & 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[6] = (U8)((lu32_Temp >> 8)& 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[5] = (U8)((lu32_Temp >> 16)& 0x000000ff);
		pstp_WrkModSet->st_SetCmd.stu8a_CmdData[4] = (U8)((lu32_Temp >> 24)& 0x000000ff);	
		
		//发送命令转义处理
		li32_BufLen = UartC_SendDataDeal(&pstp_WrkModSet->st_SetCmd, li8a_Buf);
		
		
		if (li32_BufLen >= UARTC_CMDLEN)
		{
			UartC_SendData(pstp_Uart, li8a_Buf, li32_BufLen);
		}
	}
}

//将发送出去的数据进行转义
I32 MonPtrl_OutTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1)
{	
	I32 li32_Index = 0;
	I32 li32_i;
	
	for ( li32_i = 0;li32_i<pu32_Len;li32_i++)
	{  
		if ((0 == li32_i)||(li32_i == (pu32_Len -1)))
		{
			pi8p_Buf1[li32_Index++] = pi8p_Buf[li32_i];
		}
		else if (0x7E == pi8p_Buf[li32_i])
		{
			pi8p_Buf1[li32_Index++] = 0x5C;
			pi8p_Buf1[li32_Index++] = 0x7E;
		} 
		else if (0x7A == pi8p_Buf[li32_i])
		{
			pi8p_Buf1[li32_Index++] = 0x5C;
			pi8p_Buf1[li32_Index++] = 0x7A;
		}
		else if (0x5C == pi8p_Buf[li32_i])
		{
			pi8p_Buf1[li32_Index++] = 0x5C;
			pi8p_Buf1[li32_Index++] = 0x5C;
		}
		else 
		{
			pi8p_Buf1[li32_Index++] = pi8p_Buf[li32_i];
		}
	}
	
	return li32_Index;	
}


//将接收回来的信息转义
I32 MonPtrl_InTransferMean(I8 *pi8p_Buf,U8 pu32_Len, I8 *pi8p_Buf1)
{	
	I32 li32_Index = 0;
	U32 lu32_i;
	for (lu32_i=0;lu32_i<pu32_Len;lu32_i++)
	{
		if (((U8)0x5C == pi8p_Buf[lu32_i])&&((U8)0x7D == pi8p_Buf[lu32_i+1]))
		{
			pi8p_Buf1[li32_Index++] = 0x7E;
			lu32_i += 2;
			
		}
		else if (((U8)0x5C == pi8p_Buf[lu32_i])&&((U8)0x79 == pi8p_Buf[lu32_i+1]))
		{
			pi8p_Buf1[li32_Index++] = 0x7A;
			lu32_i += 2;
		} 
		else if (((U8)0x5C == pi8p_Buf[lu32_i])&&((U8)0x5C == pi8p_Buf[lu32_i+1]))
		{
			pi8p_Buf1[li32_Index++] = 0x5C;
			lu32_i += 2;
		}
		else
		{
			pi8p_Buf1[li32_Index++] = pi8p_Buf[lu32_i];
		}	
	}
	
	return li32_Index;	
}*/
//===========================================================================
// End of file.
//===========================================================================

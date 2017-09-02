/********************************************************
文件名称：User_Main.c
作者：王京城
版本：V1.00(20131104)
说明：DSP标准程序模版

修改记录：
版本	修改时间		修改人	修改内容
0.0.0.2	2016-10-12		陈扬	参照接口协议F版本修改

*********************************************************/
#include "User_Main.h"					// User_Main头文件

void main()
{
	User_InitSysCtrl();																	//系统初始化
	InitGlobalVar();																	//初始化全局变量

	for (;;)
	{
		ServiceDog();//喂狗
		//UartADataDeal(&gst_UARTA, &gst_CMD);//串口A接收数据解析

		if(TRUE == gst_UARTA.stu8_RcvFlag)
		{
			gst_CMD.stu32_Count++;
			UartADataDeal(&gst_UARTA, &gst_CMD);//串口A接收数据解析
			//gst_CMD.stu32_Count++;
			//if(TRUE == gst_UARTA.stu8_CheckFlag)
			//{
				CalSatInf(&gst_CMD);
				UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
				gst_CMD.stu32_Count1++;
			//}


	      }
       //1ms定时计数
		/*if (gst_TimerPar.stu16_tmr1mscnt)
		{			
			gst_TimerPar.stu16_tmr1mscnt = 0;

			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

	    // 2ms定时计数
		if (gst_TimerPar.stu16_tmr2mscnt)
		{			
			gst_TimerPar.stu16_tmr2mscnt = 0;

			//CalSatInf(&gst_CMD);
						//gst_CMD.sti32_TimerCnt++;
						//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

		//5ms定时计数
		if (gst_TimerPar.stu16_tmr5mscnt)
		{			
			gst_TimerPar.stu16_tmr5mscnt = 0;
			//CalSatInf(&gst_CMD);
			//gst_CMD.sti32_TimerCnt++;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

		//10ms定时计数
		if (gst_TimerPar.stu16_tmr10mscnt)
		{
			gst_TimerPar.stu16_tmr10mscnt = 0;
			CalSatInf(&gst_CMD);
			gst_CMD.sti32_TimerCnt++;			//超时计数器
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//串口A定时上报状态
		}
	
		//20ms定时计数
		if (gst_TimerPar.stu16_tmr20mscnt)
		{
			gst_TimerPar.stu16_tmr20mscnt = 0;
		}

		//50ms定时计数
		if (gst_TimerPar.stu16_tmr50mscnt)
		{
			gst_TimerPar.stu16_tmr50mscnt = 0;	
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
			//状态查询
		}
		//100ms定时计数
		if (gst_TimerPar.stu16_tmr100mscnt)
		{
			gst_TimerPar.stu16_tmr100mscnt = 0;	


			UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//串口A定时上报状态
		}

		//200ms定时计数
		if (gst_TimerPar.stu16_tmr200mscnt)
		{
			gst_TimerPar.stu16_tmr200mscnt = 0;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//串口A定时上报状态
		}

		//1s定时计数
		if (gst_TimerPar.stu16_tmr1scnt)
		{
			gst_TimerPar.stu16_tmr1scnt = 0;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//串口A定时上报状态
		}*/

	}
}


///////////////////////////////////////////////////////////////////
//函数名称：InitGlobalVar
//函数功能：初始化全局变量
//入口参数：无
//出口参数：无
//备 	注：无
///////////////////////////////////////////////////////////////////
void InitGlobalVar(void)
{	

	memset(&gst_TimerPar, 0, sizeof(gst_TimerPar));			//基本定时

	memset(&gst_UARTA, 0, sizeof(gst_UARTA));				//串口A结构体
	memset(&gst_UARTB, 0, sizeof(gst_UARTB));				//串口B结构体
	memset(&gst_UARTC, 0, sizeof(gst_UARTC));				//串口C结构体

	memset(&gst_CMD, 0, sizeof(gst_CMD));					//命令结构体

	gst_CMD.stu8_SendFlag = TRUE;							//置发送标志位TRUE

}

//串口A发送数据
void UartA_SendData(UART *pstp_UARTA, STADATA *pstp_STUS, SETDATA * pstp_SETDATA)
{ 	
	U8 lu8_Index = 0;
	U8 lu8_CheckOut = 0;
	I16 li16_i;

	//清空结构体
	memset((void *)pstp_UARTA->stu8a_TransData, 0, sizeof(pstp_UARTA->stu8a_TransData));

	//帧头
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x7E; //帧头EB改为7E
	// pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x90; 
	
	//帧类型
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x00;  //帧类型BB改为00
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x04;   //添加04

	//波束指向角thita
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 8) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 16) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 24) & 0x000000ff);

	//波束指向角pi
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 8) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 16) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 24) & 0x000000ff);

	//备用 1字节
	//lu8_Index++;

	//校验和
	for (li16_i = 1; li16_i < (UARTA_TRANSLEN - 2); li16_i++)
	{
		lu8_CheckOut ^= pstp_UARTA->stu8a_TransData[li16_i];   //校验和校验
		//
	}
	pstp_UARTA->stu8a_TransData[lu8_Index++] = lu8_CheckOut;
	
	//帧尾
	pstp_UARTA->stu8a_TransData[lu8_Index] = 0x7A;      //帧尾5E改为7A

	pstp_UARTA->stu8_TransIndex = 1;//UART的数据发送序号
	//gst_CMD.stu32_Count1++;
	//数据发送
	SciaRegs.SCITXBUF = pstp_UARTA->stu8a_TransData[0];				//UART的数据发送缓冲区

}

//串口B发送数据
void UartBSendData(UART *pstp_UARTB)
{ 	
	pstp_UARTB->stu8a_TransData[0] = 0xEB;
	pstp_UARTB->stu8a_TransData[1] = 0x90; 
	pstp_UARTB->stu8a_TransData[2] = 0x21;
	pstp_UARTB->stu8a_TransData[3] = 0x11;
	
	//数据发送
	ScibRegs.SCITXBUF = pstp_UARTB->stu8a_TransData[0];				//UART的数据发送缓冲区
	pstp_UARTB->stu8_TransIndex = 1;								//UART的数据发送序号
}

//串口C发送数据
void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret)
{
	U8 lu8_Index;
	static I32 li32_SendCount = 0;
	li32_SendCount++;

	for (lu8_Index = 0;lu8_Index < lu16_Ret;lu8_Index++)
	{
		pstp_UARTC->stu8a_TransData[lu8_Index] = *(pi8a_Buf + lu8_Index);
	}
	//数据发送
	ScicRegs.SCITXBUF = pstp_UARTC->stu8a_TransData[0];				//UART的数据发送缓冲区
	pstp_UARTC->stu8_TransIndex = 1;								//UART的数据发送序号
}

//===========================================================================
// End of file.
//===========================================================================

/********************************************************
�ļ����ƣ�User_Main.c
���ߣ�������
�汾��V1.00(20131104)
˵����DSP��׼����ģ��

�޸ļ�¼��
�汾	�޸�ʱ��		�޸���	�޸�����
0.0.0.2	2016-10-12		����	���սӿ�Э��F�汾�޸�

*********************************************************/
#include "User_Main.h"					// User_Mainͷ�ļ�

void main()
{
	User_InitSysCtrl();																	//ϵͳ��ʼ��
	InitGlobalVar();																	//��ʼ��ȫ�ֱ���

	for (;;)
	{
		ServiceDog();//ι��
		//UartADataDeal(&gst_UARTA, &gst_CMD);//����A�������ݽ���

		if(TRUE == gst_UARTA.stu8_RcvFlag)
		{
			gst_CMD.stu32_Count++;
			UartADataDeal(&gst_UARTA, &gst_CMD);//����A�������ݽ���
			//gst_CMD.stu32_Count++;
			//if(TRUE == gst_UARTA.stu8_CheckFlag)
			//{
				CalSatInf(&gst_CMD);
				UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
				gst_CMD.stu32_Count1++;
			//}


	      }
       //1ms��ʱ����
		/*if (gst_TimerPar.stu16_tmr1mscnt)
		{			
			gst_TimerPar.stu16_tmr1mscnt = 0;

			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

	    // 2ms��ʱ����
		if (gst_TimerPar.stu16_tmr2mscnt)
		{			
			gst_TimerPar.stu16_tmr2mscnt = 0;

			//CalSatInf(&gst_CMD);
						//gst_CMD.sti32_TimerCnt++;
						//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

		//5ms��ʱ����
		if (gst_TimerPar.stu16_tmr5mscnt)
		{			
			gst_TimerPar.stu16_tmr5mscnt = 0;
			//CalSatInf(&gst_CMD);
			//gst_CMD.sti32_TimerCnt++;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
		}

		//10ms��ʱ����
		if (gst_TimerPar.stu16_tmr10mscnt)
		{
			gst_TimerPar.stu16_tmr10mscnt = 0;
			CalSatInf(&gst_CMD);
			gst_CMD.sti32_TimerCnt++;			//��ʱ������
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//����A��ʱ�ϱ�״̬
		}
	
		//20ms��ʱ����
		if (gst_TimerPar.stu16_tmr20mscnt)
		{
			gst_TimerPar.stu16_tmr20mscnt = 0;
		}

		//50ms��ʱ����
		if (gst_TimerPar.stu16_tmr50mscnt)
		{
			gst_TimerPar.stu16_tmr50mscnt = 0;	
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);
			//״̬��ѯ
		}
		//100ms��ʱ����
		if (gst_TimerPar.stu16_tmr100mscnt)
		{
			gst_TimerPar.stu16_tmr100mscnt = 0;	


			UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//����A��ʱ�ϱ�״̬
		}

		//200ms��ʱ����
		if (gst_TimerPar.stu16_tmr200mscnt)
		{
			gst_TimerPar.stu16_tmr200mscnt = 0;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//����A��ʱ�ϱ�״̬
		}

		//1s��ʱ����
		if (gst_TimerPar.stu16_tmr1scnt)
		{
			gst_TimerPar.stu16_tmr1scnt = 0;
			//UartA_SendData(&gst_UARTA, &gst_STUS, &gst_CMD);//����A��ʱ�ϱ�״̬
		}*/

	}
}


///////////////////////////////////////////////////////////////////
//�������ƣ�InitGlobalVar
//�������ܣ���ʼ��ȫ�ֱ���
//��ڲ�������
//���ڲ�������
//�� 	ע����
///////////////////////////////////////////////////////////////////
void InitGlobalVar(void)
{	

	memset(&gst_TimerPar, 0, sizeof(gst_TimerPar));			//������ʱ

	memset(&gst_UARTA, 0, sizeof(gst_UARTA));				//����A�ṹ��
	memset(&gst_UARTB, 0, sizeof(gst_UARTB));				//����B�ṹ��
	memset(&gst_UARTC, 0, sizeof(gst_UARTC));				//����C�ṹ��

	memset(&gst_CMD, 0, sizeof(gst_CMD));					//����ṹ��

	gst_CMD.stu8_SendFlag = TRUE;							//�÷��ͱ�־λTRUE

}

//����A��������
void UartA_SendData(UART *pstp_UARTA, STADATA *pstp_STUS, SETDATA * pstp_SETDATA)
{ 	
	U8 lu8_Index = 0;
	U8 lu8_CheckOut = 0;
	I16 li16_i;

	//��սṹ��
	memset((void *)pstp_UARTA->stu8a_TransData, 0, sizeof(pstp_UARTA->stu8a_TransData));

	//֡ͷ
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x7E; //֡ͷEB��Ϊ7E
	// pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x90; 
	
	//֡����
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x00;  //֡����BB��Ϊ00
	pstp_UARTA->stu8a_TransData[lu8_Index++] = 0x04;   //���04

	//����ָ���thita
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 8) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 16) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_thita * 10000) >> 24) & 0x000000ff);

	//����ָ���pi
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 8) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 16) & 0x000000ff);
	pstp_UARTA->stu8a_TransData[lu8_Index++] = (U8)(((U32)(pstp_SETDATA->st_WaveDir.stf32_pi * 10000) >> 24) & 0x000000ff);

	//���� 1�ֽ�
	//lu8_Index++;

	//У���
	for (li16_i = 1; li16_i < (UARTA_TRANSLEN - 2); li16_i++)
	{
		lu8_CheckOut ^= pstp_UARTA->stu8a_TransData[li16_i];   //У���У��
		//
	}
	pstp_UARTA->stu8a_TransData[lu8_Index++] = lu8_CheckOut;
	
	//֡β
	pstp_UARTA->stu8a_TransData[lu8_Index] = 0x7A;      //֡β5E��Ϊ7A

	pstp_UARTA->stu8_TransIndex = 1;//UART�����ݷ������
	//gst_CMD.stu32_Count1++;
	//���ݷ���
	SciaRegs.SCITXBUF = pstp_UARTA->stu8a_TransData[0];				//UART�����ݷ��ͻ�����

}

//����B��������
void UartBSendData(UART *pstp_UARTB)
{ 	
	pstp_UARTB->stu8a_TransData[0] = 0xEB;
	pstp_UARTB->stu8a_TransData[1] = 0x90; 
	pstp_UARTB->stu8a_TransData[2] = 0x21;
	pstp_UARTB->stu8a_TransData[3] = 0x11;
	
	//���ݷ���
	ScibRegs.SCITXBUF = pstp_UARTB->stu8a_TransData[0];				//UART�����ݷ��ͻ�����
	pstp_UARTB->stu8_TransIndex = 1;								//UART�����ݷ������
}

//����C��������
void UartC_SendData(UART *pstp_UARTC, I8* pi8a_Buf, U16 lu16_Ret)
{
	U8 lu8_Index;
	static I32 li32_SendCount = 0;
	li32_SendCount++;

	for (lu8_Index = 0;lu8_Index < lu16_Ret;lu8_Index++)
	{
		pstp_UARTC->stu8a_TransData[lu8_Index] = *(pi8a_Buf + lu8_Index);
	}
	//���ݷ���
	ScicRegs.SCITXBUF = pstp_UARTC->stu8a_TransData[0];				//UART�����ݷ��ͻ�����
	pstp_UARTC->stu8_TransIndex = 1;								//UART�����ݷ������
}

//===========================================================================
// End of file.
//===========================================================================

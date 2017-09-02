/********************************************************
�ļ����ƣ�User_GlobalVar.h
���ߣ�������
�汾��20131106
˵�������ļ�Ϊ����ĺ�����ͷ�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#ifndef USER_GLOBALVAR_H
#define USER_GLOBALVAR_H

#ifdef __cplusplus
extern "C" {
#endif

//����������е�ϵͳ����
#define SYSTEM_32	0				//32λϵͳ��Windows��Linux�ȣ�
#define SYSTEM_16	1				//16λϵͳ��DSP�ȣ�
#define SYSTEM_8	0				//8λϵͳ����Ƭ���ȣ�

//�û�����������Ͷ���
//32λϵͳ
#if SYSTEM_32
typedef char				I8;
typedef unsigned char		U8;
typedef short				I16;
typedef unsigned short		U16;
typedef int					I32;
typedef unsigned int		U32;
typedef long				I64;
typedef unsigned long		U64;
typedef float				F32;
typedef double				F64;
#endif
//16λϵͳ
#if SYSTEM_16
typedef char				I8;
typedef unsigned char		U8;
typedef int					I16;
typedef unsigned int		U16;
typedef long				I32;
typedef unsigned long		U32;
typedef float				F32;
typedef double				F64;
#endif
//8λϵͳ
#if SYSTEM_8
typedef char				I8;
typedef unsigned char		U8;
typedef int					I16;
typedef unsigned int		U16;
typedef long				I32;
typedef unsigned long		U32;
typedef float				F32;
#endif


	


//------------------------------------------------------------------


//������ʱ�ṹ��
typedef struct _TIMERPAR
{	
	U16 stu16_tmr1mscnt;			//1ms��ʱ����
	U16 stu16_tmr2mscnt;			//2ms��ʱ����
	U16 stu16_tmr5mscnt;			//5ms��ʱ����
	U16 stu16_tmr10mscnt;			//10ms��ʱ����
	U16 stu16_tmr20mscnt;			//20ms��ʱ����
	U16 stu16_tmr50mscnt;			//50ms��ʱ����

	U16 stu16_tmr100mscnt;			//100ms��ʱ����
	U16 stu16_tmr200mscnt;			//200ms��ʱ����

	U16 stu16_tmr1scnt;				//1s��ʱ����	

}TIMERPAR,*pTIMERPAR;

//--------------------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------------
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
//
//-----------------------------------------------------------------

typedef struct _UART_STRU
{
	U8	stu8_ComCnt1;					//ͨ�ż���1
	U8	stu8_ComCnt2;					//ͨ�ż���2
	U8	stu8_IsrCnt;					//�жϼ���
	
	U8	stu8_RcvIndex;					//���ռ���
	U8	stu8_TransIndex;				//���ͼ�
	U8  stu8_RcvFlag;					//���ձ�־
	U8  stu8_CheckFlag;                 //У���־
	U8	stu8a_RcvBuf[100];				//���ջ�����
	U8	stu8a_TransBuf[200];			//���ͻ�����
	U8	stu8a_TransLength;				//���ͱ�־
	U8	stu8a_RcvDealBuf1[100];			//UARTA���մ�����
	U8	stu8a_RcvDealBuf2[100];			//UARTB���մ�����
	U8	stu8a_RcvDealBuf3[100];			//UARTC���մ�����
	U8 stu8_Time;
	U8 stu8_Count;
	U8 stu8_Count1;
	U8 stu8_Count2;
	U8 stu8_Count3;
	U8	stu8a_TransData[200];			//��������
}UART;
extern UART gst_UARTA;
extern UART gst_UARTB;
extern UART gst_UARTC;

//�Ǻż�����ʽ
typedef struct _CMD_SATPOL
{
	U8 stu8_SatNum;					//�Ǻ�		0:��ͨ��1:����
	U8 stu8_PolMod;					//������ʽ	0:������1:����
}CMD_SATPOL;

//����ָ��
typedef struct _CMD_WAVEDIR
{
	F32 stf32_thita;				//thita��
	F32 stf32_pi;					//pi��
}CMD_WAVEDIR;

//��λУ׼
typedef struct _CMD_PHASEADJ
{
	U8 stu8_Index;					//����
	U8 stu8a_Phase[819];			//��λ
}CMD_PHASEADJ;

//��Ԫ����
typedef struct _CMD_UNITSET
{
	U8 stu8_Line;					//��
	U8 stu8_Row;					//��
	U8 stu8a_Dim[4][4];				//˥��
	U8 stu8a_Phase[4][4];			//��λ
	U8 stu8a_Power[4][4];			//��Դ
}CMD_UNITSET;

//��������ṹ��
typedef struct _RCVDATA_CMDDEAL
{
	U8 stu8_FramHead;				//֡ͷ
	U8 stu8_CmdType1;				//֡����1
	U8 stu8_CmdType2;				//֡����2
	U8 stu8a_CmdData[8];			//֡����
	U8 stu8_checkOut;				//У��
	U8 stu8_FramTail;				//֡β
}RCVDATA_CMDDEAL;


//��ѯ����ṹ��
typedef struct _CMD_INQ
{
	I8 sti8_CmdType;				//��������, ��ֵ��Ч�Ĳ�ѯ����
	U8 stu8_Line;					//��
	U8 stu8_Row;					//��
	U8 stu8_Index;					//��λУ׼����
}CMD_INQ;

//�����ѹ������������
typedef struct _CMD_VCW
{
	U8 stu8_Line;					//��
	U8 stu8_Row;					//��
	
	F32 stf32a_Vol[4][4];			//��ѹ
	F32 stf32a_Cur[4][4];			//����
	F32 stf32a_Power[4][4];			//����
}CMD_VCW;

//����¶�
typedef struct _CMD_TEMP
{
	U8 stu8_Line;					//��
	U8 stu8_Row;					//��
	
	F32 stf32a_T[4][4];				//�¶�
}CMD_TEMP;

//���ýṹ��
typedef struct _SETDATA_STRU
{
	U8					stu8_SetFlag;	//���������־��FALSE:������λ���������ã�TRUE:����λ����������
	U8					stu8_SendFlag;	//���ͱ�־��TRUE:��������֡,FALSE:����������֡
	I32					sti32_TimerCnt;	//ʱ������������ڳ�ʱ�ط�,50msδ�յ��ظ�������ط�

	RCVDATA_CMDDEAL		st_CmdDeal;		//�������
	RCVDATA_CMDDEAL		st_SetCmd;		//��������
	RCVDATA_CMDDEAL		st_InqCmd;		//��ѯ����

	U8					stu8_WrkMod;	//����ģʽ
	F32					stf32_RcvFrq;	//����Ƶ��
	F32					stf32_SendFrq;	//����Ƶ��
	CMD_SATPOL			st_SatPol;		//��λ������
	CMD_WAVEDIR			st_WaveDir;		//����ָ��
	CMD_PHASEADJ		st_PhaseAdj;	//��λУ׼
	CMD_UNITSET			st_SendUnit;	//������Ԫ
	CMD_UNITSET			st_RcvUnit;		//������Ԫ
	CMD_VCW				st_UCW;			//��ѹ������������
	CMD_TEMP			st_Temp;		//����¶�
	U32                 stu32_Count;
	U32                 stu32_Count1;
	//��ָͨ��
	F32					stf32_SatLong;	//���Ǿ���
	F32					stf32_PowerDim;	//����˥��

	//��ͨת��֡
	F32					stf32_Heading;	//����
	F32					stf32_Pitch;	//������
	F32					stf32_Roll;		//��ҡ
	F32					stf32_Long;		//����
	F32					stf32_Latitude;	//γ��
} SETDATA;

//�汾����
typedef struct _CMD_VERSION
{
	U16					stu16_Year;
	U8					stu8_Month;
	U8					stu8_Day;
}CMD_VERSION;

//״̬�ṹ��
typedef struct _STADATA_STRU
{
	U8					stu8_WrkMod;	//����ģʽ
	U8					stu8_Boot;		//FPGA����״̬ 1:���� 2:δ����
	U32                 stu32_Count;
	U32                 stu32_Count1;
	F32					stf32_RcvFrq;	//����Ƶ��
	F32					stf32_SendFrq;	//����Ƶ��
	CMD_SATPOL			st_SatPol;		//��λ������
	CMD_WAVEDIR			st_WaveDir;		//����ָ��
	CMD_PHASEADJ		st_PhaseAdj;	//��λУ׼
	CMD_UNITSET			st_SendUnit;	//������Ԫ
	CMD_UNITSET			st_RcvUnit;		//������Ԫ
	CMD_VCW				st_UCW;			//��ѹ������������
	CMD_TEMP			st_Temp;		//����¶�
	CMD_VERSION			st_FPGA;		//FPGA�汾��
	CMD_VERSION			st_DSP;			//DSP�汾��
} STADATA;

//--------------------------------------------------------------------------

#define RADMIN			0.00017				//������Сֵ 0.01���Ӧ�Ļ���ֵ(PI / 180 /100)
#define ChLiW
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_GLOBALVAR_H definition


//===========================================================================
// End of file.
//===========================================================================










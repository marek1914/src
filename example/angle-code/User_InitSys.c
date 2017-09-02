/********************************************************
�ļ����ƣ�User_InitSys.c
���ߣ�������
�汾��20131104
˵�������ļ�ΪUser_InitSys��.c�ļ�
�޸ļ�¼��
�޸�ʱ��		�޸���	�޸�����

*********************************************************/

#include "User_InitSys.h"

//-------------------------------------------------------------------------
//ÿ����Ϣ��������ã��ֱ�Ϊ��Ϣ�š�����'t'/����'r'��ID�����ݳ���

//--------------------------------CANA--------------------------------
//CANA����ID
const U16 gia_CANARXCfg[] = 
{
	CANMSG1,'r',0x000,8,				//��Ϣ�� 1  ����
};

//CANA����ID
const U16 gia_CANATXCfg[] = 
{
	CANMSG1,'t',0x000,8, 				//��Ϣ�� 1  ����
};

//--------------------------------CANB--------------------------------
//CANB����ID
const U16 gia_CANBRXCfg[] = 
{
	CANMSG1,'r',0x000,8, 				//��Ϣ�� 1  ����
};

//CANB����ID
const U16 gia_CANBTXCfg[] = 
{
	CANMSG1,'t',0x000,8, 				//��Ϣ�� 1	����
};

//CANA
U16 gu16a_CANABuf[CANARXMSGCNT*4];				//CAN��������ÿ��IDռ4��I16
I8 gi8a_CANART[CANARXMSGCNT];					//CAN���ա����ͱ�־��'r'���յ������ݣ�'t'�����ݴ�����
U16 gu16a_CANACount[CANARXMSGCNT];				//CAN���߽��ռ���
U16 gu16a_CANACntCache[CANARXMSGCNT];			//CAN���߽��ռ�������

U16	gu16_CANAImptIndex;							//CAN��Ҫ��������
U16 gu16a_CANAImptBuf[100];						//CAN��Ҫ���ݻ�����

U32	gu32_CANARevF;								//CAN���ձ�־
U8	gu8_CANARevIndex;							//CAN��������
U16 gu16a_CANADataBuf[160];						//CAN���н������ݻ�����

//CANB
U16 gu16a_CANBBuf[CANBRXMSGCNT*4];				//CAN��������ÿ��IDռ4��I16
I8 gi8a_CANBRT[CANBRXMSGCNT];					//CAN���ա����ͱ�־��'r'���յ������ݣ�'t'�����ݴ�����
U16 gu16a_CANBCount[CANBRXMSGCNT];				//CAN���߽��ռ���
U16 gu16a_CANBCntCache[CANBRXMSGCNT];			//CAN���߽��ռ�������

U32	gu32_CANBRevF;								//CAN���ձ�־
U8	gu8_CANBRevIndex;							//CAN��������
U16 gu16a_CANBDataBuf[160];						//CAN���н������ݻ�����

//--------------------------

//UART
UART gst_UARTA;
UART gst_UARTB;
UART gst_UARTC;

//-------------------------------------------------------------------------
//ϵͳ��ʼ��
void User_InitSysCtrl(void)
{
//	Step 1. Initialize System Control:
//	PLL, WatchDog, enable Peripheral Clocks
//	This example function is found in the DSP2833x_SysCtrl.c file.
	InitSysCtrl();

//	Step 2. Initalize GPIO:
//	This example function is found in the DSP2833x_Gpio.c file and
//	illustrates how to set the GPIO to it's default state.
//	InitGpio();

//	Step 3. Clear all interrupts and initialize PIE vector table:
//	Disable CPU interrupts
	DINT;

//	Initialize the PIE control registers to their default state.
//	The default state is all PIE interrupts disabled and flags
//	are cleared.
//	This function is found in the DSP2833x_PieCtrl.c file.
	InitPieCtrl();

//	Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

//	Initialize the PIE vector table with pointers to the shell Interrupt
//	Service Routines (ISR).
//	This will populate the entire table, even if the interrupt
//	is not used in this example.  This is useful for debug purposes.
//	The shell ISR routines are found in DSP2833x_DefaultIsr.c.
//	This function is found in DSP2833x_PieVect.c.
	InitPieVectTable();

	//�û���ӳ�ʼ������
	User_InitTimer();					//��ʱ����ʼ��
	User_InitCan();						//CAN��ʼ��	
	User_InitFlash();					//дFlash��ʼ��
	User_InitUart(COMA, B115200, P_NONE, BIT_8, STOP_1);//RS232
	//User_InitUart(COMB, B4800, P_NONE, BIT_8, STOP_1);//(�ŷ����ƺ�ΪRS422)
	User_InitUart(COMC, B9600, P_NONE, BIT_8, STOP_1);	//RS422
	User_InitWatchdog();				//���Ź���ʼ��
	User_InitGpio();					//��ʼ��GPIO���������ô���оƬΪ422ģʽ
//	Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM	
}

//CAN��ʼ��
void User_InitCan(void)
{
	U16 lu16_i;
	struct MBOX *lp_MBOX;
	union CANLAM_REG *lp_LAM;

	/* Create a shadow register structure for the CAN control registers. This is
	needed, since, only 32-bit access is allowed to these registers. 16-bit access
	to these registers could potentially corrupt the register contents. This is
	especially true while writing to a bit (or group of bits) among bits 16 - 31 */

	struct ECAN_REGS ECanaShadow;
	struct ECAN_REGS ECanbShadow;	
	
//	Just initalize eCAN pins for this example
//	This function is in DSP2833x_ECan.c
	InitECanGpio();	

//	Interrupts that are used in this example are re-mapped to
//	ISR functions found within this file.
   	EALLOW;  // This is needed to write to EALLOW protected registers
   	PieVectTable.ECAN0INTA = &ECAN0INTA_ISR;
	PieVectTable.ECAN0INTB = &ECAN0INTB_ISR;
   	EDIS;    // This is needed to disable write to EALLOW protected registers

//	ʹ�ܵ�9���ж�
	IER |= M_INT9;

//	Enable ECAN0INTA in the PIE: Group 9 interrupt 5
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;//AA
	PieCtrlRegs.PIEIER9.bit.INTx7 = 1;//AA

//	In this case just initalize eCAN-A and eCAN-B
//	This function is in DSP2833x_ECan.c
	InitECan();

	EALLOW;
	//-----------------------------------CANA��ʼ��--------------------------------------------------
	//CAN�����Զ���λ
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.all |= 0x00000080;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;	

	//���������ʼID�����ȡ����ؽ�������
	lp_MBOX = (struct MBOX *)&ECanaMboxes.MBOX0;
	lp_LAM = (union CANLAM_REG *)&ECanaLAMRegs.LAM0;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		//����ID��(�̶�ID��Ϊ0x000)
		lp_MBOX->MSGID.all = 0xFFFFFFFF;
		//���ý���/�������ݳ���(�̶�����Ϊ8)
		lp_MBOX->MSGCTRL.bit.DLC = 8;	
		//����
		lp_MBOX++;

		//���ؽ�������
		lp_LAM->all = 0xFFFFFFFF;
		//����
		lp_LAM++;
	}
	
	//д���Ǳ������ƼĴ���
	ECanaShadow.CANOPC.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANOPC.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANOPC.all = ECanaShadow.CANOPC.all;

	//���������շ�
	ECanaShadow.CANMD.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANMD.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;	

	//ʹ������(32����ȫ��ʹ��)
	ECanaRegs.CANME.all = 0xFFFFFFFF;
	
	//�����ж����ȼ�
	ECanaRegs.CANMIL.all = 0x00000000;

	//ʹ�����䷢��/�����ж�
	ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANMIM.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

	//�����ж���·
	ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
	ECanaShadow.CANGIM.bit.I0EN = 1;//AA
//	ECanaShadow.CANGIM.bit.I1EN = 1;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	//-----------------------------------���---------------------------------------------------------

	//-----------------------------------CANB��ʼ��--------------------------------------------------
	//CAN�����Զ���λ
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.all |= 0x00000080;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;	
	
	//���������ʼID�����ȡ����ؽ�������
	lp_MBOX = (struct MBOX *)&ECanbMboxes.MBOX0;
	lp_LAM = (union CANLAM_REG *)&ECanbLAMRegs.LAM0;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		//����ID��(�̶�ID��Ϊ0x000)
		lp_MBOX->MSGID.all = 0xFFFFFFFF;
		//���ý���/�������ݳ���(�̶�����Ϊ8)
		lp_MBOX->MSGCTRL.bit.DLC = 8;	
		//����
		lp_MBOX++;
		
		//���ؽ�������
		lp_LAM->all = 0xFFFFFFFF;
		//����
		lp_LAM++;
	}
	
	//д���Ǳ������ƼĴ���
	ECanbShadow.CANOPC.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANOPC.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANOPC.all = ECanbShadow.CANOPC.all;
	
	//���������շ�
	ECanbShadow.CANMD.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANMD.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;	
	
	//ʹ������(32����ȫ��ʹ��)
	ECanbRegs.CANME.all = 0xFFFFFFFF;
	
	//�����ж����ȼ�
	ECanbRegs.CANMIL.all = 0x00000000;
	
	//ʹ�����䷢��/�����ж�
	ECanbShadow.CANMIM.all = ECanbRegs.CANMIM.all;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANMIM.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANMIM.all = ECanbShadow.CANMIM.all;
	
	//�����ж���·
	ECanbShadow.CANGIM.all = ECanbRegs.CANGIM.all;
	ECanbShadow.CANGIM.bit.I0EN = 1;//AA
//	ECanbShadow.CANGIM.bit.I1EN = 1;
	ECanbRegs.CANGIM.all = ECanbShadow.CANGIM.all;
	//-----------------------------------���---------------------------------------------------------

	EDIS;	
}

//��ʱ����ʼ��
void User_InitTimer(void)
{
//	Interrupts that are used in this example are re-mapped to
//	ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &TINT0_ISR;
	EDIS;    // This is needed to disable write to EALLOW protected registers

//	Initialize the Device Peripheral. This function can be
//	found in DSP2833x_CpuTimers.c
	InitCpuTimers();

//  ���ö�ʱ��ʱ�䣨1000 = 1ms��
	ConfigCpuTimer(&CpuTimer0, 100/*ʱ��Ƶ��*/, 1000);
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

//	ʹ�ܵ�1���ж�
	IER |= M_INT1;

//	Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//AA
}

//���Ź���ʼ��
void User_InitWatchdog(void)
{
	//Reset the watchdog counter
	ServiceDog();

	//Enable the watchdog
	EALLOW;
	SysCtrlRegs.WDCR = 0x002F;
	EDIS;
}

//���ڳ�ʼ��
void User_InitUart(U8 pu8_Com, U32 pu32_BBaudRate,U8 Parity, U8 pu8_DataBits, U8 pu8_StopBits)
{

	struct SCI_REGS SciRegsShadow;
	U16		lu16_BRPvalueSCIA; 
	U32		lu32_ValueSCIA;

	memset(&SciRegsShadow,0,sizeof(SciRegsShadow));		//���״̬�ṹ��

	//����ͨ�ſ�������
	//���ݳ��ȿ���λ����
	SciRegsShadow.SCICCR.bit.SCICHAR = pu8_DataBits;	//����λ
	//ֹͣλ����
	SciRegsShadow.SCICCR.bit.STOPBITS = pu8_StopBits;
	//��żУ��λ����
	switch(Parity)
	{
		//��У��
	case P_NONE:
		SciRegsShadow.SCICCR.bit.PARITYENA = 0;			//��żУ��ʹ��λ0-��żУ���ֹλ��1-��żУ��ʹ��λ
		break;
		//��У��
	case P_ODD:
		SciRegsShadow.SCICCR.bit.PARITYENA = 1;			//��żУ��ʹ��λ0-��żУ���ֹλ��1-��żУ��ʹ��λ
		SciRegsShadow.SCICCR.bit.PARITY = 0;			//��żУ��λѡ�� 0-��У�飬1-żУ��
		break;
		//żУ��
	case P_EVEN:
		SciRegsShadow.SCICCR.bit.PARITYENA = 1;			//��żУ��ʹ��λ0-��żУ���ֹλ��1-��żУ��ʹ��λ
		SciRegsShadow.SCICCR.bit.PARITY = 1;			//��żУ��λѡ�� 0-��У�飬1-żУ��
		break;
	default:
		break;
	}
	//���ò�����
	lu32_ValueSCIA = 25000000 / pu32_BBaudRate/ 8 -1 ;
	lu16_BRPvalueSCIA = (Uint16) lu32_ValueSCIA ;
	SciRegsShadow.SCIHBAUD = (lu16_BRPvalueSCIA >> 8); //�趨������
	SciRegsShadow.SCILBAUD = (lu16_BRPvalueSCIA);

	SciRegsShadow.SCICCR.bit.LOOPBKENA = 0;			//��ֹ�ش�1-ʹ�ܣ�0-��ֹ��һ��ʹ�ã����޸ģ�
	SciRegsShadow.SCICCR.bit.ADDRIDLE_MODE = 0;		//������ģʽ��1-��ַλģʽЭ�飬һ��ദ����ʱʹ�ø�ģʽ��
	//0-������ģʽЭ�飬�������ݿ�ʱ����Ч��
	//���ڿ��ƼĴ���
	SciRegsShadow.SCICTL1.bit.RXENA = 1;			//����ʹ��λ��1-�����ܵ����ַ�������SCIRXBUF,0-��ֹ���յ����ַ�������SCIRXBUF��
	SciRegsShadow.SCICTL1.bit.TXENA = 1;			//������ʹ��λ��1-�ṩSCITXD���ŷ�������,0-��ֹ������������

	SciRegsShadow.SCICTL2.bit.TXINTENA = 1;			//�����ж�ʹ�ܣ�1-ʹ��TXRDY�жϣ�0-��ֹTXRDY�жϣ�
	SciRegsShadow.SCICTL2.bit.RXBKINTENA = 1;		//�����ж�ʹ�ܣ�1-ʹ��RXRDY/BRKDT�жϣ�0-��ֹRXRDY/BRKDT�жϣ�

	SciRegsShadow.SCICTL1.bit.SWRESET = 1;			// �����λ��1-����SCI��0-��ʼ��SCI״̬���Ͳ�����־λ����λ״̬��

	switch(pu8_Com)
	{
	case COMA:
		InitSciaGpio();								//IO�ڳ�ʼ��

		EALLOW; 
		PieVectTable.SCITXINTA = &SCITXINTA_ISR;
		PieVectTable.SCIRXINTA = &SCIRXINTA_ISR;
		EDIS; 

		SciaRegs.SCICCR.all = SciRegsShadow.SCICCR.all;
		SciaRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		SciaRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		SciaRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		SciaRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			//ʹ��PIEģ�� Enable the PIE block
		PieCtrlRegs.PIEIER9.bit.INTx1 = 1;			//ʹ�� PIEģ�鴮��A�����ж�PIE Group 9, int1
		PieCtrlRegs.PIEIER9.bit.INTx2 = 1;			//ʹ�� PIEģ�鴮��A�����ж�PIE Group 9, int1

		IER |= M_INT9;								// ��CPU�ж� Enable CPU INT9
		break;
	case COMB:
		InitScibGpio();								//IO�ڳ�ʼ��

		EALLOW; 
		PieVectTable.SCITXINTB = &SCITXINTB_ISR;
		PieVectTable.SCIRXINTB = &SCIRXINTB_ISR;
		EDIS;

		ScibRegs.SCICCR.all = SciRegsShadow.SCICCR.all;//��Ӧ���ڼĴ�����ֵ
		ScibRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		ScibRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		ScibRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		ScibRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;					

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// ʹ��PIEģ��Enable the PIE block
		PieCtrlRegs.PIEIER9.bit.INTx3 = 1;			// ʹ�� PIEģ�鴮��B�����ж�PIE Group 9, int3
		PieCtrlRegs.PIEIER9.bit.INTx4 = 1;			// ʹ�� PIEģ�鴮��B�����ж�PIE Group 9, int4

		IER |= M_INT9;								//��CPU�ж� Enable CPU INT9
		break;

	case COMC:	
		InitScicGpio();								//IO�ڳ�ʼ��

		EALLOW;  
		PieVectTable.SCITXINTC = &SCITXINTC_ISR;
		PieVectTable.SCIRXINTC = &SCIRXINTC_ISR;
		EDIS; 

		ScicRegs.SCICCR.all = SciRegsShadow.SCICCR.all;	//��Ӧ���ڼĴ�����ֵ
		ScicRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		ScicRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		ScicRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		ScicRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;					
		//��Ӧ���ڼĴ�����ֵ

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			//ʹ��PIEģ�� Enable the PIE block
		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;			//ʹ�� PIEģ�鴮��C�����ж� PIE Group 8, int5
		PieCtrlRegs.PIEIER8.bit.INTx6 = 1;			//ʹ�� PIEģ�鴮��C�����ж� PIE Group 8, int6

		IER |= M_INT8;								//��CPU�ж� Enable CPU INT8
		break;
	default:
		break;

	}	
}


//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void User_InitGpio(void)
{
	DINT;
	
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;	// This is needed to write to EALLOW protected registers

	EDIS;   // This is needed to disable write to EALLOW protected registers
                                    // Enable Global Interrupts
	
	
	EALLOW;   
	//GPIO80��GPIO81��GPIO82��Ϊ0ʱ������оƬΪ422ģʽ
	//��Ӧ�ڴ���оƬ��M0��M1��M2��Ϊ�͵�ƽ
	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;  // XA8
	GpioCtrlRegs.GPCDIR.bit.GPIO80 = 1;		//����Ϊ�����
	GpioDataRegs.GPCDAT.bit.GPIO80 = 0;				//���Ϊ0

	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0;  // XA9
	GpioCtrlRegs.GPCDIR.bit.GPIO81 = 1;		//����Ϊ�����
	GpioDataRegs.GPCDAT.bit.GPIO81 = 0;				//���Ϊ0

    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0;  // XA10
	GpioCtrlRegs.GPCDIR.bit.GPIO82 = 1;		//����Ϊ�����
	GpioDataRegs.GPCDAT.bit.GPIO82 = 0;				//���Ϊ0	
	EDIS;	
}

//===========================================================================
// End of file.
//===========================================================================

/********************************************************
文件名称：User_InitSys.c
作者：王京城
版本：20131104
说明：本文件为User_InitSys的.c文件
修改记录：
修改时间		修改人	修改内容

*********************************************************/

#include "User_InitSys.h"

//-------------------------------------------------------------------------
//每个消息对象的配置，分别为消息号、发送't'/接收'r'、ID、数据长度

//--------------------------------CANA--------------------------------
//CANA接收ID
const U16 gia_CANARXCfg[] = 
{
	CANMSG1,'r',0x000,8,				//消息号 1  接收
};

//CANA发送ID
const U16 gia_CANATXCfg[] = 
{
	CANMSG1,'t',0x000,8, 				//消息号 1  发送
};

//--------------------------------CANB--------------------------------
//CANB接收ID
const U16 gia_CANBRXCfg[] = 
{
	CANMSG1,'r',0x000,8, 				//消息号 1  接收
};

//CANB发送ID
const U16 gia_CANBTXCfg[] = 
{
	CANMSG1,'t',0x000,8, 				//消息号 1	发送
};

//CANA
U16 gu16a_CANABuf[CANARXMSGCNT*4];				//CAN缓冲区，每个ID占4个I16
I8 gi8a_CANART[CANARXMSGCNT];					//CAN接收、发送标志，'r'接收到新数据，'t'新数据待发送
U16 gu16a_CANACount[CANARXMSGCNT];				//CAN总线接收计数
U16 gu16a_CANACntCache[CANARXMSGCNT];			//CAN总线接收计数缓存

U16	gu16_CANAImptIndex;							//CAN重要数据索引
U16 gu16a_CANAImptBuf[100];						//CAN重要数据缓存区

U32	gu32_CANARevF;								//CAN接收标志
U8	gu8_CANARevIndex;							//CAN接收索引
U16 gu16a_CANADataBuf[160];						//CAN所有接收数据缓存区

//CANB
U16 gu16a_CANBBuf[CANBRXMSGCNT*4];				//CAN缓冲区，每个ID占4个I16
I8 gi8a_CANBRT[CANBRXMSGCNT];					//CAN接收、发送标志，'r'接收到新数据，'t'新数据待发送
U16 gu16a_CANBCount[CANBRXMSGCNT];				//CAN总线接收计数
U16 gu16a_CANBCntCache[CANBRXMSGCNT];			//CAN总线接收计数缓存

U32	gu32_CANBRevF;								//CAN接收标志
U8	gu8_CANBRevIndex;							//CAN接收索引
U16 gu16a_CANBDataBuf[160];						//CAN所有接收数据缓存区

//--------------------------

//UART
UART gst_UARTA;
UART gst_UARTB;
UART gst_UARTC;

//-------------------------------------------------------------------------
//系统初始化
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

	//用户添加初始化程序
	User_InitTimer();					//定时器初始化
	User_InitCan();						//CAN初始化	
	User_InitFlash();					//写Flash初始化
	User_InitUart(COMA, B115200, P_NONE, BIT_8, STOP_1);//RS232
	//User_InitUart(COMB, B4800, P_NONE, BIT_8, STOP_1);//(伺服控制盒为RS422)
	User_InitUart(COMC, B9600, P_NONE, BIT_8, STOP_1);	//RS422
	User_InitWatchdog();				//看门狗初始化
	User_InitGpio();					//初始化GPIO，用于设置串口芯片为422模式
//	Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM	
}

//CAN初始化
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

//	使能第9组中断
	IER |= M_INT9;

//	Enable ECAN0INTA in the PIE: Group 9 interrupt 5
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;//AA
	PieCtrlRegs.PIEIER9.bit.INTx7 = 1;//AA

//	In this case just initalize eCAN-A and eCAN-B
//	This function is in DSP2833x_ECan.c
	InitECan();

	EALLOW;
	//-----------------------------------CANA初始化--------------------------------------------------
	//CAN总线自动复位
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.all |= 0x00000080;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;	

	//设置邮箱初始ID、长度、本地接受屏蔽
	lp_MBOX = (struct MBOX *)&ECanaMboxes.MBOX0;
	lp_LAM = (union CANLAM_REG *)&ECanaLAMRegs.LAM0;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		//设置ID号(固定ID号为0x000)
		lp_MBOX->MSGID.all = 0xFFFFFFFF;
		//设置接收/发送数据长度(固定长度为8)
		lp_MBOX->MSGCTRL.bit.DLC = 8;	
		//索引
		lp_MBOX++;

		//本地接受屏蔽
		lp_LAM->all = 0xFFFFFFFF;
		//索引
		lp_LAM++;
	}
	
	//写覆盖保护控制寄存器
	ECanaShadow.CANOPC.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANOPC.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANOPC.all = ECanaShadow.CANOPC.all;

	//设置邮箱收发
	ECanaShadow.CANMD.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANMD.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;	

	//使能邮箱(32邮箱全部使能)
	ECanaRegs.CANME.all = 0xFFFFFFFF;
	
	//设置中断优先级
	ECanaRegs.CANMIL.all = 0x00000000;

	//使能邮箱发送/接收中断
	ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanaShadow.CANMIM.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

	//设置中断线路
	ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
	ECanaShadow.CANGIM.bit.I0EN = 1;//AA
//	ECanaShadow.CANGIM.bit.I1EN = 1;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	//-----------------------------------完成---------------------------------------------------------

	//-----------------------------------CANB初始化--------------------------------------------------
	//CAN总线自动复位
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.all |= 0x00000080;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;	
	
	//设置邮箱初始ID、长度、本地接受屏蔽
	lp_MBOX = (struct MBOX *)&ECanbMboxes.MBOX0;
	lp_LAM = (union CANLAM_REG *)&ECanbLAMRegs.LAM0;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		//设置ID号(固定ID号为0x000)
		lp_MBOX->MSGID.all = 0xFFFFFFFF;
		//设置接收/发送数据长度(固定长度为8)
		lp_MBOX->MSGCTRL.bit.DLC = 8;	
		//索引
		lp_MBOX++;
		
		//本地接受屏蔽
		lp_LAM->all = 0xFFFFFFFF;
		//索引
		lp_LAM++;
	}
	
	//写覆盖保护控制寄存器
	ECanbShadow.CANOPC.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANOPC.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANOPC.all = ECanbShadow.CANOPC.all;
	
	//设置邮箱收发
	ECanbShadow.CANMD.all = 0x00000000;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANMD.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;	
	
	//使能邮箱(32邮箱全部使能)
	ECanbRegs.CANME.all = 0xFFFFFFFF;
	
	//设置中断优先级
	ECanbRegs.CANMIL.all = 0x00000000;
	
	//使能邮箱发送/接收中断
	ECanbShadow.CANMIM.all = ECanbRegs.CANMIM.all;
	for (lu16_i=0;lu16_i<CANARXMAILBOX;lu16_i++)
	{
		ECanbShadow.CANMIM.all |= ((U32)0x00000001 << lu16_i);	
	}
	ECanbRegs.CANMIM.all = ECanbShadow.CANMIM.all;
	
	//设置中断线路
	ECanbShadow.CANGIM.all = ECanbRegs.CANGIM.all;
	ECanbShadow.CANGIM.bit.I0EN = 1;//AA
//	ECanbShadow.CANGIM.bit.I1EN = 1;
	ECanbRegs.CANGIM.all = ECanbShadow.CANGIM.all;
	//-----------------------------------完成---------------------------------------------------------

	EDIS;	
}

//定时器初始化
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

//  配置定时器时间（1000 = 1ms）
	ConfigCpuTimer(&CpuTimer0, 100/*时钟频率*/, 1000);
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

//	使能第1组中断
	IER |= M_INT1;

//	Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//AA
}

//看门狗初始化
void User_InitWatchdog(void)
{
	//Reset the watchdog counter
	ServiceDog();

	//Enable the watchdog
	EALLOW;
	SysCtrlRegs.WDCR = 0x002F;
	EDIS;
}

//串口初始化
void User_InitUart(U8 pu8_Com, U32 pu32_BBaudRate,U8 Parity, U8 pu8_DataBits, U8 pu8_StopBits)
{

	struct SCI_REGS SciRegsShadow;
	U16		lu16_BRPvalueSCIA; 
	U32		lu32_ValueSCIA;

	memset(&SciRegsShadow,0,sizeof(SciRegsShadow));		//清空状态结构体

	//串口通信控制设置
	//数据长度控制位设置
	SciRegsShadow.SCICCR.bit.SCICHAR = pu8_DataBits;	//数据位
	//停止位设置
	SciRegsShadow.SCICCR.bit.STOPBITS = pu8_StopBits;
	//奇偶校验位设置
	switch(Parity)
	{
		//无校验
	case P_NONE:
		SciRegsShadow.SCICCR.bit.PARITYENA = 0;			//奇偶校验使能位0-奇偶校验禁止位，1-奇偶校验使能位
		break;
		//奇校验
	case P_ODD:
		SciRegsShadow.SCICCR.bit.PARITYENA = 1;			//奇偶校验使能位0-奇偶校验禁止位，1-奇偶校验使能位
		SciRegsShadow.SCICCR.bit.PARITY = 0;			//奇偶校验位选择 0-奇校验，1-偶校验
		break;
		//偶校验
	case P_EVEN:
		SciRegsShadow.SCICCR.bit.PARITYENA = 1;			//奇偶校验使能位0-奇偶校验禁止位，1-奇偶校验使能位
		SciRegsShadow.SCICCR.bit.PARITY = 1;			//奇偶校验位选择 0-奇校验，1-偶校验
		break;
	default:
		break;
	}
	//设置波特率
	lu32_ValueSCIA = 25000000 / pu32_BBaudRate/ 8 -1 ;
	lu16_BRPvalueSCIA = (Uint16) lu32_ValueSCIA ;
	SciRegsShadow.SCIHBAUD = (lu16_BRPvalueSCIA >> 8); //设定波特率
	SciRegsShadow.SCILBAUD = (lu16_BRPvalueSCIA);

	SciRegsShadow.SCICCR.bit.LOOPBKENA = 0;			//禁止回传1-使能，0-禁止（一般使用，不修改）
	SciRegsShadow.SCICCR.bit.ADDRIDLE_MODE = 0;		//空闲线模式（1-地址位模式协议，一般多处理器时使用该模式，
	//0-空闲线模式协议，处理数据块时更有效）
	//串口控制寄存器
	SciRegsShadow.SCICTL1.bit.RXENA = 1;			//接收使能位（1-将接受到的字符放置在SCIRXBUF,0-禁止接收到的字符放置在SCIRXBUF）
	SciRegsShadow.SCICTL1.bit.TXENA = 1;			//发送器使能位（1-提供SCITXD引脚发送数据,0-禁止发送器工作）

	SciRegsShadow.SCICTL2.bit.TXINTENA = 1;			//发送中断使能（1-使能TXRDY中断，0-禁止TXRDY中断）
	SciRegsShadow.SCICTL2.bit.RXBKINTENA = 1;		//接收中断使能（1-使能RXRDY/BRKDT中断，0-禁止RXRDY/BRKDT中断）

	SciRegsShadow.SCICTL1.bit.SWRESET = 1;			// 软件复位（1-重启SCI，0-初始化SCI状态机和操作标志位至复位状态）

	switch(pu8_Com)
	{
	case COMA:
		InitSciaGpio();								//IO口初始化

		EALLOW; 
		PieVectTable.SCITXINTA = &SCITXINTA_ISR;
		PieVectTable.SCIRXINTA = &SCIRXINTA_ISR;
		EDIS; 

		SciaRegs.SCICCR.all = SciRegsShadow.SCICCR.all;
		SciaRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		SciaRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		SciaRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		SciaRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			//使能PIE模块 Enable the PIE block
		PieCtrlRegs.PIEIER9.bit.INTx1 = 1;			//使能 PIE模块串口A接收中断PIE Group 9, int1
		PieCtrlRegs.PIEIER9.bit.INTx2 = 1;			//使能 PIE模块串口A发送中断PIE Group 9, int1

		IER |= M_INT9;								// 开CPU中断 Enable CPU INT9
		break;
	case COMB:
		InitScibGpio();								//IO口初始化

		EALLOW; 
		PieVectTable.SCITXINTB = &SCITXINTB_ISR;
		PieVectTable.SCIRXINTB = &SCIRXINTB_ISR;
		EDIS;

		ScibRegs.SCICCR.all = SciRegsShadow.SCICCR.all;//相应串口寄存器赋值
		ScibRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		ScibRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		ScibRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		ScibRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;					

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// 使能PIE模块Enable the PIE block
		PieCtrlRegs.PIEIER9.bit.INTx3 = 1;			// 使能 PIE模块串口B接收中断PIE Group 9, int3
		PieCtrlRegs.PIEIER9.bit.INTx4 = 1;			// 使能 PIE模块串口B发送中断PIE Group 9, int4

		IER |= M_INT9;								//开CPU中断 Enable CPU INT9
		break;

	case COMC:	
		InitScicGpio();								//IO口初始化

		EALLOW;  
		PieVectTable.SCITXINTC = &SCITXINTC_ISR;
		PieVectTable.SCIRXINTC = &SCIRXINTC_ISR;
		EDIS; 

		ScicRegs.SCICCR.all = SciRegsShadow.SCICCR.all;	//相应串口寄存器赋值
		ScicRegs.SCIHBAUD = SciRegsShadow.SCIHBAUD;
		ScicRegs.SCILBAUD = SciRegsShadow.SCILBAUD;
		ScicRegs.SCICTL2.all = SciRegsShadow.SCICTL2.all;
		ScicRegs.SCICTL1.all = SciRegsShadow.SCICTL1.all;					
		//相应串口寄存器赋值

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			//使能PIE模块 Enable the PIE block
		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;			//使能 PIE模块串口C接收中断 PIE Group 8, int5
		PieCtrlRegs.PIEIER8.bit.INTx6 = 1;			//使能 PIE模块串口C发送中断 PIE Group 8, int6

		IER |= M_INT8;								//开CPU中断 Enable CPU INT8
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
	//GPIO80、GPIO81、GPIO82均为0时，串口芯片为422模式
	//对应于串口芯片的M0、M1、M2均为低电平
	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;  // XA8
	GpioCtrlRegs.GPCDIR.bit.GPIO80 = 1;		//设置为输出口
	GpioDataRegs.GPCDAT.bit.GPIO80 = 0;				//输出为0

	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0;  // XA9
	GpioCtrlRegs.GPCDIR.bit.GPIO81 = 1;		//设置为输出口
	GpioDataRegs.GPCDAT.bit.GPIO81 = 0;				//输出为0

    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0;  // XA10
	GpioCtrlRegs.GPCDIR.bit.GPIO82 = 1;		//设置为输出口
	GpioDataRegs.GPCDAT.bit.GPIO82 = 0;				//输出为0	
	EDIS;	
}

//===========================================================================
// End of file.
//===========================================================================

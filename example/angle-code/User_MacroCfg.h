/********************************************************
文件名称：MacroCfg.h
作者：王京城
版本：20131106
说明：本文件为程序的宏配置头文件
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
修改记录：
修改时间		修改人		程序版本	
2015年06月19日	王京城		"0.2.0.4"
修改内容：
1、加入指向跟踪功能及相关（跟踪及航向修正等）
2、修改CAN中断接收及处理功能
3、加入Ku圆扫跟踪故障后直接转入指向跟踪功能
4、增加CANB中0x01E4处理功能，包括经纬度、UTC时间、北斗航向、北斗速度、数据有效标志

修改时间		修改人		程序版本	
2015年08月31日	王京城		"0.2.0.5"
修改内容：
1、参数上报时间间隔由2ms变为5ms
2、轴角码盘模式，轴角初始化时：方位速度由0.25变为0.15、俯仰速度由0.25变为0.1、交叉速度由0.25变为0.1

修改时间		修改人		程序版本	
2015年10月28日	王京城		"0.2.0.6"
修改内容：
1、指向跟踪环路中，方位、俯仰、交叉轴加速度限制去掉
2、增加导航盒串口通讯状态上报

修改时间		修改人		程序版本	
2015年12月8日	王京城		"0.2.0.7"
修改内容：
1、将罗经、自动惯导作为两种独立的工作方式，不在使用罗经修正惯导等；
2、双天线时，不再将本天线的航向数据送往对方；
3、使用指向跟踪时，处理航向上报（变为真实航向）；
4、状态上报增加2ms上报，5个数据包，增加惯导姿态速度等；
5、轴角初始化速度修改为缓慢增加，避免阶跃
6、指向跟踪扫主面加入位置环，时延调整使用两个参数。

*********************************************************/

#ifndef USER_MACROCFG_H
#define USER_MACROCFG_H

#ifdef __cplusplus
extern "C" {
#endif

//宏定义
#define VERSIONS				"0.0.0.1"		//DSP程序版本
#define DEVTYPE					"ZK"			//设备类型(上报程序版本使用)(主控程序)

//CAN波特率
#define B_250K					9				//波特率为250K
	
//CAN消息号
#define CANMSG1					1
#define CANMSG2					2
#define CANMSG3					3
#define CANMSG4					4
#define CANMSG5					5
#define CANMSG6					6
#define CANMSG7					7
#define CANMSG8					8
#define CANMSG9					9
#define CANMSG10				10
#define CANMSG11				11
#define CANMSG12				12
#define CANMSG13				13
#define CANMSG14				14
#define CANMSG15				15
#define CANMSG16				16
#define CANMSG17				17
#define CANMSG18				18
#define CANMSG19				19
#define CANMSG20				20
#define CANMSG21				21
#define CANMSG22				22
#define CANMSG23				23
#define CANMSG24				24
#define CANMSG25				25
#define CANMSG26				26
#define CANMSG27				27
#define CANMSG28				28
#define CANMSG29				29
#define CANMSG30				30
#define CANMSG31				31
#define CANMSG32				32

//用户自定义宏//
#define ERROR					0.0001			//小数
#define PI						3.1415926		//PI
#define DEGTORAD				0.01745329		//角度转换到弧度
#define RADTODEG				57.2957805		//弧度转换到角度

#define FALSE					0				//错误
#define TRUE					1				//正确

#define HISIG					1				//高电平
#define LOWSIG					0				//低电平

#define ZERO					0				//0
#define ASCIIZERO				'0'				//0x30

#define ANGLE360				360.0			//角度基数
#define AngRng360				0				//角度范围(0~359.999)
#define AngRng180				1				//角度范围(-180.000~179.999)

#define STARTSEND				0x01			//开始发送
#define STOPSEND 				0x02			//停止发送

#define TIME1S					200				//1秒计时

//DSP程序在线刷新宏定义
#define DSPMAIN                 0x01            //DSP程序刷新指令(测试为0x01)
#define Jumpgxcx				(void(*)(void))  0x33FFF6

//
#define CANTOCANCLOSE			0				//CANA到CANB中转关闭
#define CANTOCANOPENSENDDATA	1				//CANA到CANB中转打开（不关闭正常程序，直接转发数据）			
#define CANTOCANOPENTIMERSEND	2				//CANA到CANB中转打开（不关闭正常程序，2ms循环上报数据）
#define CANTOCANOPENDEBUG		3				//CANA到CANB中转打开（关闭正常程序，直接转发数据，用于程序调试，
												//不能再进行其它任何操作，不能保存FLASH，必须断电才可恢复正常）
#define CANTOCANOPENDEBUGOPEN	4				//CANA到CANB中转打开（关闭正常程序，直接转发数据，用于程序调试，）
												//可以进行参数修改，读写FLASH等操作）
//UART
#define UARTA_RCVFIRST			0xEB			//串口A接收第一个字节
#define UARTA_RCVSECOND			0x90			//串口A接收第二个字节
#define	UARTA_RCVLEN_A			8				//串口A接收数据长度 帧类型0xEE 改为8个长度-cy
#define	UARTA_RCVLEN_B			15				//串口A接收数据长度 帧类型0xBB
#define	UARTA_TRANSLEN			13				//串口A发送数据长度-cy-改为13   //10.27改14为13

#define UARTB_RCVFIRST			0xEB			//串口B接收第一个字节
#define UARTB_RCVSECOND			0x90			//串口B接收第二个字节
#define	UARTB_RCVLEN			4				//串口B接收数据长度
#define	UARTB_TRANSLEN			4				//串口B发送数据长度

#define UARTC_RCVFIRST			0xEB			//串口C接收第一个字节
#define UARTC_RCVSECOND			0x90			//串口C接收第二个字节
#define UARTC_CMDLEN			13				//串口C接收数据长度	
#define	UARTC_TRANSLEN			13				//串口C发送数据长度

#define CHECKOUT_INDEX1			0x01			//校验开始位置
#define CHECKOUT_INDEX2			0x0A			//校验结束位置

//帧头、帧尾
#define FRAMEHEAD				0x7E			//帧头
#define FRAMETAIL				0x7A			//帧尾

#define SET_CMD				0x00				//设置命令
#define INQ_CMD				0x01				//查询命令

#define WRKMOD_CMD			0x00				//工作模式命令
#define	RCVFREQ_CMD			0x01				//接收频率命令
#define SENDFREQ_CMD		0x02				//发射频率命令
#define SATPOL_CMD			0x03				//星位、极化命令
#define WAVEDIR_CMD			0x04				//波束指向命令
#define PHASEADJ_CMD		0x05				//相位校准命令
#define	RCVPHASEADJ_CMD		0x06				//接收相位校准命令
#define SENDUNIT_CMD		0x07				//发射振元命令
#define RCVUNIT_CMD			0x08				//接收振元命令
#define TEMP_CMD			0x09				//温度命令
#define VOLCR_CMD			0x0A				//电流电压命令
#define BOOT_CMD			0x0B				//查询FPGA是否启动
#define FPGADATE_CMD		0x0C				//查询FPGA最后修改日期
#define DSPDATE_CMD			0x0D				//查询DSP最后日期

#define WRKMOD_NORM			0x01				//正常模式
#define WRKMOD_STANDBY		0x02				//待机模式

#define SAT_TT				0x00				//天通
#define SAT_TL				0x01				//天链

#define POLMODE_CL			0x00				//左旋圆极化
#define POLMODE_CR			0x01				//右旋圆极化

#define PHASEADJ_PACKLEN	0x07				//每包有效数据个数
#define PHASEADJ_MAXCNT		117					//相位校准数据包长度

#define UNITSET_PACKLEN		0x05				//振元设置数据长度

#define CMDRCV_OK			0x01				//命令接收正确
#define CMDRCV_ERR			0x02				//命令接收错误
#define CMDRCV_IDERR		0x03				//命令ID索引错误

#define INQ_INVALID			-1					//查询命令无效

#define BUF_LEN				20					//临时缓冲区长度

#define LINE_MAX			4					//行
#define ROW_MAX				4					//列	
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_MACROCFG_H definition


//===========================================================================
// End of file.
//===========================================================================

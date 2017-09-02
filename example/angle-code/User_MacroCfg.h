/********************************************************
�ļ����ƣ�MacroCfg.h
���ߣ�������
�汾��20131106
˵�������ļ�Ϊ����ĺ�����ͷ�ļ�
//										o o        
//										.-----.-----.
//										|     |     |
//										|  '  | | | |
//										'-----'-'-'-'
//
�޸ļ�¼��
�޸�ʱ��		�޸���		����汾	
2015��06��19��	������		"0.2.0.4"
�޸����ݣ�
1������ָ����ٹ��ܼ���أ����ټ����������ȣ�
2���޸�CAN�жϽ��ռ�������
3������KuԲɨ���ٹ��Ϻ�ֱ��ת��ָ����ٹ���
4������CANB��0x01E4�����ܣ�������γ�ȡ�UTCʱ�䡢�������򡢱����ٶȡ�������Ч��־

�޸�ʱ��		�޸���		����汾	
2015��08��31��	������		"0.2.0.5"
�޸����ݣ�
1�������ϱ�ʱ������2ms��Ϊ5ms
2���������ģʽ����ǳ�ʼ��ʱ����λ�ٶ���0.25��Ϊ0.15�������ٶ���0.25��Ϊ0.1�������ٶ���0.25��Ϊ0.1

�޸�ʱ��		�޸���		����汾	
2015��10��28��	������		"0.2.0.6"
�޸����ݣ�
1��ָ����ٻ�·�У���λ����������������ٶ�����ȥ��
2�����ӵ����д���ͨѶ״̬�ϱ�

�޸�ʱ��		�޸���		����汾	
2015��12��8��	������		"0.2.0.7"
�޸����ݣ�
1�����޾����Զ��ߵ���Ϊ���ֶ����Ĺ�����ʽ������ʹ���޾������ߵ��ȣ�
2��˫����ʱ�����ٽ������ߵĺ������������Է���
3��ʹ��ָ�����ʱ���������ϱ�����Ϊ��ʵ���򣩣�
4��״̬�ϱ�����2ms�ϱ���5�����ݰ������ӹߵ���̬�ٶȵȣ�
5����ǳ�ʼ���ٶ��޸�Ϊ�������ӣ������Ծ
6��ָ�����ɨ�������λ�û���ʱ�ӵ���ʹ������������

*********************************************************/

#ifndef USER_MACROCFG_H
#define USER_MACROCFG_H

#ifdef __cplusplus
extern "C" {
#endif

//�궨��
#define VERSIONS				"0.0.0.1"		//DSP����汾
#define DEVTYPE					"ZK"			//�豸����(�ϱ�����汾ʹ��)(���س���)

//CAN������
#define B_250K					9				//������Ϊ250K
	
//CAN��Ϣ��
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

//�û��Զ����//
#define ERROR					0.0001			//С��
#define PI						3.1415926		//PI
#define DEGTORAD				0.01745329		//�Ƕ�ת��������
#define RADTODEG				57.2957805		//����ת�����Ƕ�

#define FALSE					0				//����
#define TRUE					1				//��ȷ

#define HISIG					1				//�ߵ�ƽ
#define LOWSIG					0				//�͵�ƽ

#define ZERO					0				//0
#define ASCIIZERO				'0'				//0x30

#define ANGLE360				360.0			//�ǶȻ���
#define AngRng360				0				//�Ƕȷ�Χ(0~359.999)
#define AngRng180				1				//�Ƕȷ�Χ(-180.000~179.999)

#define STARTSEND				0x01			//��ʼ����
#define STOPSEND 				0x02			//ֹͣ����

#define TIME1S					200				//1���ʱ

//DSP��������ˢ�º궨��
#define DSPMAIN                 0x01            //DSP����ˢ��ָ��(����Ϊ0x01)
#define Jumpgxcx				(void(*)(void))  0x33FFF6

//
#define CANTOCANCLOSE			0				//CANA��CANB��ת�ر�
#define CANTOCANOPENSENDDATA	1				//CANA��CANB��ת�򿪣����ر���������ֱ��ת�����ݣ�			
#define CANTOCANOPENTIMERSEND	2				//CANA��CANB��ת�򿪣����ر���������2msѭ���ϱ����ݣ�
#define CANTOCANOPENDEBUG		3				//CANA��CANB��ת�򿪣��ر���������ֱ��ת�����ݣ����ڳ�����ԣ�
												//�����ٽ��������κβ��������ܱ���FLASH������ϵ�ſɻָ�������
#define CANTOCANOPENDEBUGOPEN	4				//CANA��CANB��ת�򿪣��ر���������ֱ��ת�����ݣ����ڳ�����ԣ���
												//���Խ��в����޸ģ���дFLASH�Ȳ�����
//UART
#define UARTA_RCVFIRST			0xEB			//����A���յ�һ���ֽ�
#define UARTA_RCVSECOND			0x90			//����A���յڶ����ֽ�
#define	UARTA_RCVLEN_A			8				//����A�������ݳ��� ֡����0xEE ��Ϊ8������-cy
#define	UARTA_RCVLEN_B			15				//����A�������ݳ��� ֡����0xBB
#define	UARTA_TRANSLEN			13				//����A�������ݳ���-cy-��Ϊ13   //10.27��14Ϊ13

#define UARTB_RCVFIRST			0xEB			//����B���յ�һ���ֽ�
#define UARTB_RCVSECOND			0x90			//����B���յڶ����ֽ�
#define	UARTB_RCVLEN			4				//����B�������ݳ���
#define	UARTB_TRANSLEN			4				//����B�������ݳ���

#define UARTC_RCVFIRST			0xEB			//����C���յ�һ���ֽ�
#define UARTC_RCVSECOND			0x90			//����C���յڶ����ֽ�
#define UARTC_CMDLEN			13				//����C�������ݳ���	
#define	UARTC_TRANSLEN			13				//����C�������ݳ���

#define CHECKOUT_INDEX1			0x01			//У�鿪ʼλ��
#define CHECKOUT_INDEX2			0x0A			//У�����λ��

//֡ͷ��֡β
#define FRAMEHEAD				0x7E			//֡ͷ
#define FRAMETAIL				0x7A			//֡β

#define SET_CMD				0x00				//��������
#define INQ_CMD				0x01				//��ѯ����

#define WRKMOD_CMD			0x00				//����ģʽ����
#define	RCVFREQ_CMD			0x01				//����Ƶ������
#define SENDFREQ_CMD		0x02				//����Ƶ������
#define SATPOL_CMD			0x03				//��λ����������
#define WAVEDIR_CMD			0x04				//����ָ������
#define PHASEADJ_CMD		0x05				//��λУ׼����
#define	RCVPHASEADJ_CMD		0x06				//������λУ׼����
#define SENDUNIT_CMD		0x07				//������Ԫ����
#define RCVUNIT_CMD			0x08				//������Ԫ����
#define TEMP_CMD			0x09				//�¶�����
#define VOLCR_CMD			0x0A				//������ѹ����
#define BOOT_CMD			0x0B				//��ѯFPGA�Ƿ�����
#define FPGADATE_CMD		0x0C				//��ѯFPGA����޸�����
#define DSPDATE_CMD			0x0D				//��ѯDSP�������

#define WRKMOD_NORM			0x01				//����ģʽ
#define WRKMOD_STANDBY		0x02				//����ģʽ

#define SAT_TT				0x00				//��ͨ
#define SAT_TL				0x01				//����

#define POLMODE_CL			0x00				//����Բ����
#define POLMODE_CR			0x01				//����Բ����

#define PHASEADJ_PACKLEN	0x07				//ÿ����Ч���ݸ���
#define PHASEADJ_MAXCNT		117					//��λУ׼���ݰ�����

#define UNITSET_PACKLEN		0x05				//��Ԫ�������ݳ���

#define CMDRCV_OK			0x01				//���������ȷ
#define CMDRCV_ERR			0x02				//������մ���
#define CMDRCV_IDERR		0x03				//����ID��������

#define INQ_INVALID			-1					//��ѯ������Ч

#define BUF_LEN				20					//��ʱ����������

#define LINE_MAX			4					//��
#define ROW_MAX				4					//��	
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of USER_MACROCFG_H definition


//===========================================================================
// End of file.
//===========================================================================

#include "CSystem_Local.h"
#include "CSystem_IIC.h"
#include "CSystem_WatchDog.h"

#include "stm32f0xx.h"


#define GPIO_SCL	GPIO_Pin_13
#define GPIO_SDA	GPIO_Pin_14
#define GPIO_PORT	GPIOB

#define SCL(v)		{if (v){GPIO_PORT->BSRR = GPIO_SCL;}else{GPIO_PORT->BRR = GPIO_SCL;}}
#define SDA(v)		{if (v){GPIO_PORT->BSRR = GPIO_SDA;}else{GPIO_PORT->BRR = GPIO_SDA;}}
#define SDA_read()	((GPIO_PORT->IDR & GPIO_SDA) ? m_true : m_false)
#define SCL_read()	((GPIO_PORT->IDR & GPIO_SCL) ? m_true : m_false)

#define I2C_Lock()		__disable_irq()	
#define I2C_Unlick()	__enable_irq()

/////////////////////////////////////////////////////////////////////////////////
static void I2C_delay(void)
{
	__NOP();
    __NOP();
	__NOP();
    __NOP();
	__NOP();
	//670KHz
}
/////////////////////////////////////////////////////////////////////////////////
static t_bool I2C_CheckIdle(void)
{
	t_u8 i = 255;
	while(!SCL_read() && (--i))
	{
	}
	return i;
}
/////////////////////////////////////////////////////////////////////////////////
static t_u8 I2C_Start(void)
{
	t_u8 i = 30;
	I2C_Lock();
    SDA(1);
    SCL(1);
	I2C_delay();
	while(!SDA_read() && (--i))
	{
		SCL(0);
		I2C_delay();
		SCL(1);
		I2C_delay();
	}
	if (i)
	{
		SDA(0);
		I2C_delay();
		SCL(0);
		I2C_Unlick();		
	}
    return i;
}
/////////////////////////////////////////////////////////////////////////////////
static void I2C_Stop(void)
{
	I2C_Lock();
    SDA(0);
    I2C_delay();
    SCL(1);
	I2C_CheckIdle();
	I2C_delay();
    SDA(1);
	I2C_Unlick();
}
/////////////////////////////////////////////////////////////////////////////////
static void I2C_Ack(void)
{
	I2C_Lock();
    SDA(0);
    I2C_delay();
    SCL(1);
	I2C_CheckIdle();
    I2C_delay();
    SCL(0);
    I2C_Unlick();
}
/////////////////////////////////////////////////////////////////////////////////
static void I2C_NoAck(void)
{
	I2C_Lock();
    SDA(1);
    I2C_delay();
    SCL(1);
	I2C_CheckIdle();
    I2C_delay();
    SCL(0);
    I2C_Unlick();
}
/////////////////////////////////////////////////////////////////////////////////
static t_u8 I2C_WaitAck(t_bool bNoAck)
{
	t_u8 i=0;
	SDA(1);
	I2C_delay();
	SCL(1);	
	if (I2C_CheckIdle())
	{
		I2C_delay();
		if (!bNoAck){
			i = 30;
			while(SDA_read() && (--i)){
				I2C_delay();
			}
		}else{
			i = 1;
		}
	}
	SCL(0);
	SDA(0);
    return i;
}
/////////////////////////////////////////////////////////////////////////////////
static t_bool I2C_SendByte(t_u8 byte, t_bool bNoAck = m_false)
{
    t_u8 i = 7;
	
	I2C_Lock();
	SDA(byte & 0x80);
	byte <<= 1;
	I2C_delay();
	SCL(1);
	if (!I2C_CheckIdle())
	{
		SCL(0);
		return m_false;
	}
	I2C_delay();
	SCL(0);
    while (i--) 
	{
        SDA(byte & 0x80);
		byte <<= 1;
		
        I2C_delay();
        SCL(1);
		I2C_delay();
		SCL(0);
		
    }
	I2C_Unlick();
	
	return I2C_WaitAck(bNoAck);
}
/////////////////////////////////////////////////////////////////////////////////
static t_u8 I2C_ReceiveByte(void)
{
    t_u8 i = 7;
    t_u8 byte = 0;
	
	I2C_Lock();
    SDA(1);
	I2C_delay();
	SCL(1);
    if (I2C_CheckIdle())
	{
		I2C_delay();
		if (SDA_read()) {
			byte |= 0x01;
		}
		SCL(0);
		while (i--) {
			byte <<= 1;
			I2C_delay();
			SCL(1);
			I2C_delay();
			if (SDA_read()) {
				byte |= 0x01;
			}
			SCL(0);
		}
	}
	I2C_Unlick();
    return byte;
}
/////////////////////////////////////////////////////////////////////////////////
void Dev_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_SDA | GPIO_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
}
/////////////////////////////////////////////////////////////////////////////////
t_bool Dev_I2C_Write(t_u8 addr, t_u8 len, t_u8* buf, t_bool bStop)
{
	int i;
	
    if (!I2C_Start())
    {
        return m_false;
    }

	if (!I2C_SendByte(addr))
	{
        I2C_Stop();
        return m_false;
    }
	
    for (i = 0; i < len; i++) 
	{
		if (!I2C_SendByte(buf[i], (i+1)>=len))
		{
            I2C_Stop();
            return m_false;
        }
    }
	if (bStop)
	{
		I2C_Stop();
	}
    return m_true;
}
/////////////////////////////////////////////////////////////////////////////////
t_bool Dev_I2C_Read(t_u8 addr, t_u8 len, t_u8* buf, t_bool bDelay)
{
	if (!I2C_Start())
    {
		return m_false;
	}

	if (!I2C_SendByte(addr | 0x01)) 
	{
        I2C_Stop();
        return m_false;
    }

    while (len) 
	{
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return m_true;
}


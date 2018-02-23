#include "pub_math.h"
#include "pub_protocol.h"
#include "hal_local.h"
#include "rad_include.h"


/*****************************************************************/
#define CFG_EN_CRC(v)	(v<<3)
#define CFG_CRCO(v)		(v<<2)
#define CFG_PWRUP(v)	(v<<1)
#define CFG_PRIM_RX(v)	(v<<0)
/*****************************************************************/
#define RX_DR			0x40
#define	TX_DS			0x20
#define MAX_RT			0x10

#define CNF_TX			CFG_EN_CRC(1)|CFG_CRCO(1)|CFG_PWRUP(1)|CFG_PRIM_RX(0)|0x40
#define CNF_RX			CFG_EN_CRC(1)|CFG_CRCO(1)|CFG_PWRUP(1)|CFG_PRIM_RX(1)|0x30
#define CNF_OFF			CFG_EN_CRC(1)|CFG_CRCO(1)|CFG_PWRUP(0)|CFG_PRIM_RX(0)
//****************************************************************//
// SPI(nRF24L01) commands
#define RF_READ_REG     0x00  // Define read command to register
#define RF_WRITE_REG    0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register
#define ACTIVATE		0x50  // 
//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define DYNPD			0x1C
#define FEATURE			0x1D

#define EVT_INT		0
#define EVT_TIMOUT	1

enum{eModeIdle=0,eModeRevc,eModeSend};

static t_u8 			m_mode		= eModeIdle;
static t_u8 			m_freq_set	= 0;
static t_u8 			m_rx_buffer[32]	= {0};
static t_bool 			m_rx_test	= m_false;
static t_rad_send_fn	m_send_fn	= m_null;
static t_rad_timout_fn	m_timout_fn	= m_null;
static t_rad_receive_fn	m_receive_fn= m_null;

/************************************************************************
**
************************************************************************/
static t_void default_send_callback(void)
{
}
/************************************************************************
**
************************************************************************/
static t_void default_timout_callback(void)
{
}
/************************************************************************
**
************************************************************************/
static t_void rad_set_receive_fn(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static void nrf24_w_reg(t_u8 reg, t_u8 value)
{
	hal_spi_csn(m_true);        // CSN low, init SPI transaction
  	hal_spi_rw(reg);      // select register
  	hal_spi_rw(value);              // ..and write value to it..
  	hal_spi_csn(m_false);                   // CSN high again
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static t_u8 nrf24_r_reg(t_u8 reg)
{
	t_u8 status;
	
  	hal_spi_csn(m_true);        // CSN low, init SPI transaction
  	hal_spi_rw(reg);      // select register
	status = hal_spi_rw(0x00);
  	hal_spi_csn(m_false);       // CSN high again

  	return(status);              // return nRF24L01 status t_u8
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
static t_u8 nrf24_read_buf(t_u8 reg, t_u8 *pBuf, t_u8 len)
{
	t_u8 status;

  	hal_spi_csn(m_true);                 		// Set CSN low, init SPI tranaction
  	status = hal_spi_rw(reg);       		// Select register to write to and read status t_u8
	hal_spi_r_buf(pBuf, len);
  	hal_spi_csn(m_false);                           // Set CSN high again

  	return(status);                    // return nRF24L01 status t_u8
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static t_u8 nrf24_write_buf(t_u8 reg, t_u8 *pBuf, t_u8 len)
{
	t_u8 status;

  	hal_spi_csn(m_true);                // Set CSN low, init SPI tranaction
  	status = hal_spi_rw(reg);    // Select register to write to and read status t_u8
	hal_spi_w_buf(pBuf, len);
  	hal_spi_csn(m_false);                 // Set CSN high again
	
  	return(status);          // return nRF24L01 status t_u8
}
/************************************************************************
Function: nRF24L01_FlushTX();

  Description:
 ************************************************************************/
static void nrf24_flush_tx(void)
{
	nrf24_w_reg(FLUSH_TX, 0xff);
}
/************************************************************************
Function: nRF24L01_FlushRX();

  Description:
 ************************************************************************/
static void nrf24_flush_rx(void)
{
	nrf24_w_reg(FLUSH_RX, 0xff);
}
/************************************************************************
Function: nRF24L01_GetStatus();

  Description:
 ************************************************************************/
static t_u8 nrf24_get_status(void)
{
	return nrf24_r_reg(STATUS);
}
/************************************************************************
Function: nRF24L01_GetStatus();

  Description:
 ************************************************************************/
static void nrf24_clear_irq(unsigned mask)
{
	nrf24_w_reg(RF_WRITE_REG + STATUS, mask);
}
/************************************************************************
Function: nRF24L01_NextTxPacket();

  Description:
 ************************************************************************/
static void nrf24_rx_packet(void)
{
	nrf24_read_buf(RD_RX_PLOAD, m_rx_buffer, PLOAD_WIDTH);
}
/************************************************************************
Function: nrf24_activate();

  Description:
 ************************************************************************/
static void nrf24_activate(t_u8 code)
{
	nrf24_w_reg(ACTIVATE, code);
}
/************************************************************************
Function: set_channle();
  Description:
 ************************************************************************/
void rad_set_freq(t_u8 ch)
{
	m_freq_set = ch;
}
/************************************************************************
Function: rad_rx_test();
  Description:
 ************************************************************************/
t_void rad_rx_test(t_bool bEn)
{
	m_rx_test=bEn;
}
/************************************************************************
Function: rad_rx_buffer();
  Description:
 ************************************************************************/
t_u8* rad_rx_buffer(void)
{
	return m_rx_buffer;
}
/************************************************************************
Function: set_rx_mode();
  Description:
 ************************************************************************/
void rad_set_rx_mode(t_u32 addr)
{
	if (m_mode != eModeRevc)
	{
		hal_radio_enable(0);
		nrf24_w_reg(RF_WRITE_REG + RF_CH,  m_freq_set);
		// RX_Addr0 same as TX_Adr for Auto.Ack
		nrf24_write_buf(RF_WRITE_REG + RX_ADDR_P0, (t_u8*)&addr, ADRR_WIDTH); 	
		nrf24_w_reg(RF_WRITE_REG + CONFIG,	CNF_RX); 

		hal_radio_enable(1);
		
		m_mode = eModeRevc;
	}
}
/************************************************************************
Function: set_tx_data();
  Description:
 ************************************************************************/
void rad_set_tx_mode(t_u32 addr, t_u8 *pbuf, t_u8 len)
{
	if (m_mode == eModeSend)
	{
		nrf24_write_buf(WR_TX_PLOAD, pbuf, b_min(PLOAD_WIDTH,len));
	}
	else
	{
		hal_radio_enable(0);
		nrf24_w_reg(RF_WRITE_REG + RF_CH,  m_freq_set);
		nrf24_write_buf(RF_WRITE_REG + TX_ADDR, (t_u8*)&addr, ADRR_WIDTH);
		nrf24_write_buf(WR_TX_PLOAD, pbuf, b_min(PLOAD_WIDTH,len));
		nrf24_w_reg(RF_WRITE_REG + CONFIG, CNF_TX);

		m_mode = eModeSend;
	}
}
/************************************************************************
Function: enable_send();
  Description:
 ************************************************************************/
void rad_enable_send(void)
{
	if (m_mode == eModeSend)
	{
		hal_radio_enable(1);
		hal_radio_enable_delay();
		hal_radio_enable(0);
	}
}
/************************************************************************
Function: rad_disable();
  Description:
 ************************************************************************/
void rad_disable(void)
{
	m_mode = eModeIdle;
	hal_radio_enable(0);
}
/************************************************************************
**
************************************************************************/
t_void rad_timout(void)
{
	m_timout_fn();
}
/************************************************************************
**
************************************************************************/
t_void rad_interrupt(void)
{
	t_u8 status = nrf24_get_status();
	if (status & TX_DS)
	{
		nrf24_clear_irq(TX_DS);	
		if (m_mode == eModeSend)
		{
			m_send_fn();
		}
	}
	else if (status & RX_DR)
	{
		if (m_rx_test)
		{
			hal_led_onoff(1,1);
		}
		if (m_mode == eModeRevc)
		{
			nrf24_rx_packet();
			nrf24_clear_irq(RX_DR);
			if (nrf24_get_status() == 0x00)
			{
				nrf24_flush_rx();
			}
			m_receive_fn();
		}
		else
		{
			nrf24_flush_rx();
			nrf24_clear_irq(RX_DR);
		}
	}
	else if (status & MAX_RT)
	{
		
	}
}
/************************************************************************
**
************************************************************************/
void rad_init(void)
{
	m_mode 		= eModeIdle;
	m_send_fn	= default_send_callback;
	m_timout_fn	= default_timout_callback;
	m_receive_fn= rad_set_receive_fn;
	
	hal_dly_callback(rad_timout);
	hal_radio_callback(rad_interrupt);
	
	hal_radio_enable(0);
	nrf24_w_reg(RF_WRITE_REG + CONFIG,0x00);
	nrf24_flush_tx();
	nrf24_flush_tx();
	nrf24_flush_tx();
	nrf24_flush_rx();
	nrf24_flush_rx();
	nrf24_flush_rx();

	nrf24_clear_irq(TX_DS | RX_DR | MAX_RT);

	nrf24_w_reg(RF_WRITE_REG + EN_AA,     0x00); // Enable Auto.Ack:Pipe0
	nrf24_w_reg(RF_WRITE_REG + EN_RXADDR, 0x01); // Enable Pipe0
	nrf24_w_reg(RF_WRITE_REG + RF_CH,     0x0A); // Select RF channel 0
	nrf24_w_reg(RF_WRITE_REG + RF_SETUP,  0x0F); // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
#if (ADRR_WIDTH==3)
	nrf24_w_reg(RF_WRITE_REG + SETUP_AW,  0x01); // RX/TX Address 3 width
#elif (ADRR_WIDTH==4)
	nrf24_w_reg(RF_WRITE_REG + SETUP_AW,  0x02); // RX/TX Address 4 width
#else
	# error SETUP_AW
#endif
	nrf24_w_reg(RF_WRITE_REG + SETUP_RETR,0x00); // Re-Transmit Enable
	nrf24_w_reg(RF_WRITE_REG + RX_PW_P0,  PLOAD_WIDTH); // Select same RX payload width as TX Payload width

	nrf24_activate(0x73);
	nrf24_w_reg(RF_WRITE_REG + FEATURE,   0x04); // the ack payload for the pipe 0 
	nrf24_w_reg(RF_WRITE_REG + DYNPD, 	  0x01); // Enable the dynamic payload size 
	nrf24_activate(0x73);
}
/************************************************************************
**
************************************************************************/
t_void rad_set_send_fn(t_rad_send_fn pFn)
{
	m_send_fn = pFn;
}
/************************************************************************
**
************************************************************************/
t_void rad_set_timeout_fn(t_rad_timout_fn pFn)
{
	m_timout_fn = pFn;
}
/************************************************************************
**
************************************************************************/
t_void rad_set_receivce_fn(t_rad_receive_fn pFn)
{
	m_receive_fn = pFn;
}
/************************************************************************
** 返回频率通道个数
************************************************************************/
t_u8 rad_freq_num(void)
{
	return 125;
}


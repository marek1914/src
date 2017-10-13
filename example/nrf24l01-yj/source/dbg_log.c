#include "pub_math.h"
#include "pub_protocol.h"
#include "hal_local.h"
#include "dbg_log.h"

static char   m_buffer[1024];
static t_u16  m_front = 0;
static t_u16  m_rear  = 0;

/************************************************************************/
/*                                                                      */
/************************************************************************/
static t_bool dbg_getchar(t_u8* nChar)
{
	if( m_rear != m_front )
	{
		t_u16 front = m_front;
		if ((++front)>=sizeof(m_buffer))
		{
			front = 0;
		}
		*nChar = m_buffer[front];
		m_front = front;
		return m_true;
	}
	return m_false;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static void dbg_putchar(char chr)
{
	if ((++m_rear)>=sizeof(m_buffer))
	{
		m_rear = 0;
	}
	m_buffer[m_rear] = chr;
	if( m_rear == m_front )
	{
		if ((++m_front)>=sizeof(m_buffer))
		{
			m_front = 0;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static void dbg_putstr(char * str, t_s32 len)
{
	t_s32 i;
	for(i=0;i<len;i++)
	{
		if ((++m_rear)>=sizeof(m_buffer))
		{
			m_rear = 0;
		}
		m_buffer[m_rear] = str[i];
		if( m_rear == m_front )
		{
			if ((++m_front)>=sizeof(m_buffer))
			{
				m_front = 0;
			}
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void dbg_init(void)
{
	hal_uart_callback(dbg_getchar);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void dbg_print(const char rw, t_u32 addr)
{
	char i,buf[6],chr;
	for(i=0;i<6;i++){
		chr = addr & 0x0F;
		addr= addr >> 4;
		if (chr < 10){
			buf[5-i] = '0' + chr;
		}else{
			buf[5-i] = 'A' + (chr - 10);
		}
	}
	
	dbg_putchar(rw);
	dbg_putchar(':');
	dbg_putstr(buf,6);
	dbg_putstr("\r\n", 2);
	
	hal_uart_request();
}

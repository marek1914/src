#include <stdarg.h>
#include <string.h>

#include "CTypes.h"
#include "COuart.h"
#include "CSystem_Terminal_Lcd.h"
#include "CSystem_Terminal_Delay.h"

#include "stm32f10x.h"
const t_u8 initreg_tab_hx8347[][5] = {
	0x00,0xEA,0x00,0x00, 0,
	0x00,0xEB,0x00,0x20, 0,
	0x00,0xEC,0x00,0x3C, 0,
	0x00,0xED,0x00,0xC4, 0,
	0x00,0xE8,0x00,0x48, 0,
	0x00,0xE9,0x00,0x38, 0,
	0x00,0xF1,0x00,0x01, 0,
	0x00,0xF2,0x00,0x08, 0,
	0x00,0x40,0x00,0x01, 0,
	0x00,0x41,0x00,0x07, 0,
	0x00,0x42,0x00,0x09, 0,
	0x00,0x43,0x00,0x19, 0,
	0x00,0x44,0x00,0x17, 0,
	0x00,0x45,0x00,0x20, 0,
	0x00,0x46,0x00,0x18, 0,
	0x00,0x47,0x00,0x61, 0,
	0x00,0x48,0x00,0x00, 0,
	0x00,0x49,0x00,0x10, 0,
	0x00,0x4A,0x00,0x17, 0,
	0x00,0x4B,0x00,0x19, 0,
	0x00,0x4C,0x00,0x14, 0,
	0x00,0x50,0x00,0x1F, 0,
	0x00,0x51,0x00,0x28, 0,
	0x00,0x52,0x00,0x26, 0,
	0x00,0x53,0x00,0x36, 0,
	0x00,0x54,0x00,0x38, 0,
	0x00,0x55,0x00,0x3E, 0,
	0x00,0x56,0x00,0x1E, 0,
	0x00,0x57,0x00,0x67, 0,
	0x00,0x58,0x00,0x0B, 0,
	0x00,0x59,0x00,0x06, 0,
	0x00,0x5A,0x00,0x08, 0,
	0x00,0x5B,0x00,0x0F, 0,
	0x00,0x5C,0x00,0x1F, 0,
	0x00,0x5D,0x00,0xCC, 0,
	0x00,0x1B,0x00,0x1B, 0,
	0x00,0x1A,0x00,0x01, 0,
	0x00,0x24,0x00,0x70, 0,
	0x00,0x25,0x00,0x58, 0,
	0x00,0x23,0x00,0x6C, 0,
	0x00,0x18,0x00,0x39, 0,
	0x00,0x19,0x00,0x01, 0,
	0x00,0x01,0x00,0x00, 0,
	0x00,0x1F,0x00,0x88, 5,
	0x00,0x1F,0x00,0x80, 5,
	0x00,0x1F,0x00,0x90, 5,
	0x00,0x1F,0x00,0xD0, 5,
	0x00,0x17,0x00,0x05, 0,
	0x00,0x36,0x00,0x09, 0,
	0x00,0x28,0x00,0x38,40,
	0x00,0x28,0x00,0x3F, 0,
	0x00,0x02,0x00,0x00, 0,
	0x00,0x03,0x00,0x00, 0,
	0x00,0x04,0x00,0x00, 0,
	0x00,0x05,0x00,0xEF, 0,
	0x00,0x06,0x00,0x00, 0,
	0x00,0x07,0x00,0x00, 0,
	0x00,0x08,0x00,0x01, 0,
	0x00,0x09,0x00,0x3F, 0
};

const t_u8 initreg_tab_ili9347[] = {
#if 0
	0x11，0, 120,
	0x28, 0,   0,
	0xEF, 3,   0,0x03, 0x80, 0x02,
	0xCF, 3,   0,0x00, 0xAA, 0xB0,
	0xED, 4,   0,0x67, 0x03, 0x12, 0x81,
	0xCB, 5,   0,0x39, 0x2C, 0x00, 0x34, 0x02,
	0xEA, 2,   0,0x00, 0x00,
	0xE8, 3,   0,0x85, 0x0A, 0x78,
	0xB1, 2,   0,0x00, 0x18,
	0xC0, 1,   0,0x26,						//VRH[5:0]
	0xC1, 1,   0,0x0C,						//SAP[2:0];BT[3:0]
	0xC2, 1,   0,0x22,
	0xC5, 2,   0,0x34, 0x3C,					//VCM control
	0xC7, 1,   0,0xbd,						//ad
	0x3A, 1,   0,0x55,
	0x36, 1,   0,0x08,						// Memory Access Control
	0xB1, 2,   0,0x00, 0x1B,					// Frame Rate Control
	0xB6, 2,   0,0x8a, 0xa2,					// Display Function Control
	0xF2, 1,   0,0x00,						// 3Gamma Function Disable
	0xF7, 1,   0,0x20,
	0x26, 1,   0,0x01,						//Gamma curve selected
											//Set Gamma
	0xE0,15,   0,0x1f, 0x26, 0x24, 0x0c, 0x12, 0x0a, 0x53, 0xed, 0x3c, 0x01, 0x10, 0x08, 0x08, 0x07, 0x00,
	0xE1 15,   0,0x00, 0x19, 0x1b, 0x03, 0x0d, 0x05, 0x2c, 0x14, 0x43, 0x0e, 0x0f, 0x07, 0x37, 0x38, 0x1F,
	
	0x29, 0,   0,
#endif
	0x11, 0,120,
	0xCF, 3,  0,0x00,0x81,0X30,
	0xED, 4,  0,0x64,0x03,0X12,0X81,
	0xE8, 3,  0,0x85,0x10,0x7A,
	0xCB, 5,  0,0x39,0x2C,0x00,0x34,0x02,
	0xF7, 1,  0,0x20,
	0xEA, 2,  0,0x00,0x00,
	0xC0, 1,  0,0x21,
	0xC1, 1,  0,0x11,
	0xC5, 2,  0,0x3F,0x3C,
	0xC7, 1,  0,0xA8,
	0x36, 1,  0,0x08,
	0xB1, 2,  0,0x00,0x1B,
	0xB6, 2,  0,0x0A,0xA2,
	0xF2, 1,  0,0x00,
	0x26, 1,  0,0x01,
	0x3a, 1,  0,0x55,
	0xF6, 3,  0,0x01,0x00,0x01,
	0xB0, 1,  0,0x60,
	0xE0,15,  0,0x0F,0x23,0x1F,0x0B,0x0E,0x08,0x4B,0XA8,0x3B,0x0A,0x14,0x06,0x10,0x09,0x00,
 	0xE1,15,  0,0x00,0x1C,0x20,0x04,0x10,0x08,0x34,0x47,0x44,0x05,0x0B,0x09,0x2F,0x36,0x0F,
	0x29, 0,
	0x2C, 0
};






//LCD_RST
#define LCD_RST_H()		GPIOA->BSRR = GPIO_Pin_8
#define LCD_RST_L()		GPIOA->BRR  = GPIO_Pin_8
//LCD_RS
#define LCD_RS_H() 		GPIOB->BSRR = GPIO_Pin_15
#define LCD_RS_L() 		GPIOB->BRR  = GPIO_Pin_15
//LCD_RW
#define LCD_RW_H() 		GPIOB->BSRR = GPIO_Pin_14
#define LCD_RW_L() 		GPIOB->BRR  = GPIO_Pin_14
//LCD_RD
#define LCD_RD_H() 		GPIOB->BSRR = GPIO_Pin_13
#define LCD_RD_L() 		GPIOB->BRR  = GPIO_Pin_13
//LCD_CS
#define LCD_CS_H() 		GPIOB->BSRR = GPIO_Pin_12
#define LCD_CS_L() 		GPIOB->BRR  = GPIO_Pin_12
//LCD_DB_W
#define LCD_DB_W(v)			{\
	GPIOA->BSRR = ( v) & 0xFF;	\
	GPIOA->BRR	= (~v) & 0xFF;	\
}
#define LCD_DB_R()		(GPIOA->IDR&0xFF)
#define LCD_DB_OUT()	GPIOA->CRL = 0x33333333
#define LCD_DB_IN()		GPIOA->CRL = 0x44444444

static u16    last_x = 0xFFFE;
static u16    last_y = 0xFFFE;
static t_bool hx8347_alive = m_false;

void Dev_LCD_GPIO_init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
   	GPIO_InitStructure.GPIO_Pin = 0x01FF;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
   	GPIO_InitStructure.GPIO_Pin = 0xF000;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	LCD_RS_H();
	LCD_RW_H();
	LCD_RD_H();
	LCD_CS_H();

}
void Dev_LCD_BL_init()
{
	uint16_t PrescalerValue = 0;
	RCC_ClocksTypeDef clk;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_GetClocksFreq(&clk);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (clk.PCLK1_Frequency / (12000*100)) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 100;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1.2.3.4 */
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3 , &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM3 , TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3 , ENABLE);
		
	/* TIM3 enable counter */
	TIM_Cmd(TIM3 , ENABLE);
}


//*************write register command************//
void dev_lcd_hx8347_write_com(t_u8 com1,t_u8 com2)
{
//	LCD_RD_H();
	LCD_RS_L();
	
//	LCD_CS_L();
	LCD_DB_W(com1);
	LCD_RW_L();
	LCD_RW_H();

	LCD_DB_W(com2);		
	LCD_RW_L();
	LCD_RW_H();
//	LCD_CS_H();		
}

//*************write  data************//
void dev_lcd_hx8347_write_data(t_u8 dat1,t_u8 dat2)
{
//	LCD_RD_H();
	LCD_RS_H();
	
//	LCD_CS_L();
	LCD_DB_W(dat1);
	LCD_RW_L();
	LCD_RW_H();

	LCD_DB_W(dat2);		
	LCD_RW_L();
	LCD_RW_H();
//	LCD_CS_H();			
}

void dev_lcd_hx8347_init(void)
{
	int i;
	t_u8* pDat;

	hx8347_alive = m_true;
	
	LCD_RD_H();
	
	for(i = 0; i < b_arraysize(initreg_tab_hx8347); i++)
	{
		pDat = (t_u8*)initreg_tab_hx8347[i];
		dev_lcd_hx8347_write_com(pDat[0],pDat[1]);
		dev_lcd_hx8347_write_data(pDat[2],pDat[3]);
		if (pDat[4])
		{
			Dev_Delay_Delay(pDat[4]);
		}
	}

	dev_lcd_hx8347_write_com(0x00,0x22);                      //Start GRAM write

	for(i=0;i<320*240;i++)
	{
		dev_lcd_hx8347_write_data(0x00,0x00); 
	}
}

void dev_lcd_hx8347_set_point(t_u16 x,t_u16 y)
{
	dev_lcd_hx8347_write_com(0x00,0x03);
	dev_lcd_hx8347_write_data(0x00,x);
	dev_lcd_hx8347_write_com(0x00,0x06);
	dev_lcd_hx8347_write_data(0x00,y>>8);
	dev_lcd_hx8347_write_com(0x00,0x07);
	dev_lcd_hx8347_write_data(0x00,y);
	dev_lcd_hx8347_write_com(0x00,0x22);
}

void dev_lcd_hx8347_draw_point(t_u16 x,t_u16 y, t_u16 nColor)
{
	if (last_x != x || y != last_y)
	{
		dev_lcd_hx8347_set_point(x,y);
	}
	last_x = x + 1;
	last_y = y;
	dev_lcd_hx8347_write_data(nColor>>8,nColor); 
} 


void dev_lcd_hx8347_draw_hline(t_u16 x,t_u16 y,t_u16 w, t_u16  nColor)
{
	last_y = 0xffff;
	dev_lcd_hx8347_set_point(x,y);
	
	t_u16 i;
	for(i=0;i<w;i++){
		dev_lcd_hx8347_write_data(nColor>>8,nColor); 
	}	
}

void dev_lcd_hx8347_draw_buffer(t_u16 x,t_u16 y,t_u16 w, t_u16* pBuf)
{
	last_y = 0xffff;
	dev_lcd_hx8347_set_point(x,y);
	
	t_u16 i,color;
	for(i=0;i<w;i++){
		color = pBuf[i];
		dev_lcd_hx8347_write_data(color>>8,color); 
	}	
}

void dev_lcd_ili9341_write_comm(t_u8 com)
{
   	LCD_RS_L();

   	LCD_DB_W(com)
 	LCD_RW_L();
	LCD_RW_H();
}

void dev_lcd_ili9341_write_data(t_u8 com1)
{
   	LCD_RS_H();

   	LCD_DB_W(com1)
 	LCD_RW_L();
	LCD_RW_H();
}

void dev_lcd_ili9341_write_data(t_u8 com1, t_u8 com2)
{
 	LCD_RS_H();

   	LCD_DB_W(com1)
 	LCD_RW_L();
	LCD_RW_H();

	LCD_DB_W(com2)
 	LCD_RW_L();
	LCD_RW_H();
}


void dev_lcd_ili9341_read_data(const t_u8 com, t_u8 * pbuf, t_u8 len)
{
	t_u8 i;
	
	//Command
	LCD_RS_L();

   	LCD_DB_W(com)
 	LCD_RW_L();
	LCD_RW_H();

	//Data
   	LCD_DB_IN();		//数据输入
	LCD_RS_H();
	Dev_Delay_Delay(1);
	for(i=0;i<len;i++)
	{
	 	LCD_RD_L();
		LCD_RD_L();
		LCD_RD_L();
		LCD_RD_L();
	 	pbuf[i] = LCD_DB_R();
		LCD_RD_H();
	}

	LCD_DB_OUT();		//数据输出
}

void dev_lcd_ili9341_init(void)
{
	int offset, i, len;
	
	//************* Start Initial Sequence **********//
	for (offset = 0; offset < b_arraysize(initreg_tab_ili9347); )
	{
		len = initreg_tab_ili9347[offset+1];
	
		dev_lcd_ili9341_write_comm(initreg_tab_ili9347[offset+0]);

		for (i = 0; i < len; i++)
		{
			dev_lcd_ili9341_write_data(initreg_tab_ili9347[offset+3+i]);
		}
		if (initreg_tab_ili9347[offset+2])
		{
			Dev_Delay_Delay(initreg_tab_ili9347[offset+2]);
		}
		offset += len + 3;
	}
}


void dev_lcd_ili9341_set_point(t_u16 x,t_u16 y)
{
	dev_lcd_ili9341_write_comm (0x2A);
	dev_lcd_ili9341_write_data(x>>8,x);

	dev_lcd_ili9341_write_comm (0x2B);
	dev_lcd_ili9341_write_data(y>>8,y);
	
	dev_lcd_ili9341_write_comm (0x2C);
}

void dev_lcd_ili9341_draw_point(t_u16 x,t_u16 y, t_u16 nColor)
{
	if (last_x != x || y != last_y)
	{
		dev_lcd_ili9341_set_point(x,y);
	}
	last_x = x + 1;
	last_y = y;
	dev_lcd_ili9341_write_data(nColor>>8,nColor); 
} 


void dev_lcd_ili9341_draw_hline(t_u16 x,t_u16 y,t_u16 w, t_u16  nColor)
{
	last_y = 0xffff;
	dev_lcd_ili9341_set_point(x,y);
	
	t_u16 i;
	for(i=0;i<w;i++){
		dev_lcd_ili9341_write_data(nColor>>8,nColor); 
	}	
}

void dev_lcd_ili9341_draw_buffer(t_u16 x,t_u16 y,t_u16 w, t_u16* pBuf)
{
	last_y = 0xffff;
	dev_lcd_ili9341_set_point(x,y);
	
	t_u16 i,color;
	for(i=0;i<w;i++){
		color = pBuf[i];
		dev_lcd_ili9341_write_data(color>>8,color); 
	}	
}


void Dev_Lcd_init(void)
{
	t_u8 id;
	Dev_LCD_GPIO_init();
	Dev_LCD_BL_init();

	
	LCD_RST_H();
	Dev_Delay_Delay(1);
	LCD_RST_L();
	Dev_Delay_Delay(20);
	LCD_RST_H();
	Dev_Delay_Delay(200);

	LCD_CS_L();

	dev_lcd_ili9341_read_data(0x00, &id, 1);
	if (id == 0x75){
		dev_lcd_hx8347_init();
	}else{
		dev_lcd_ili9341_init();
	}
}


void Dev_Lcd_Draw_Point(t_u16 x,t_u16 y, t_u16 nColor)
{
	if (hx8347_alive){
		dev_lcd_hx8347_draw_point(x,y,nColor);
	}else{
		dev_lcd_ili9341_draw_point(x,y,nColor);
	}
} 

void Dev_Lcd_Draw_HLine(t_u16 x,t_u16 y,t_u16 w, t_u16  nColor)
{
	if (hx8347_alive){
		dev_lcd_hx8347_draw_hline(x,y,w,nColor);
	}else{
		dev_lcd_ili9341_draw_hline(x,y,w,nColor);
	}
}

void Dev_Lcd_Draw_Buffer(t_u16 x,t_u16 y,t_u16 w, t_u16* pBuf)
{
	if (hx8347_alive){
		dev_lcd_hx8347_draw_buffer( x, y, w,  pBuf);
	}else{
		dev_lcd_ili9341_draw_buffer( x, y, w,  pBuf);
	}
}

void Dev_Lcd_EnableBl(t_bool bEn)
{
	if (bEn)
	{
		TIM3->CCR4 = 80;
	}
	else
	{
		TIM3->CCR4 = 0;
	}
}


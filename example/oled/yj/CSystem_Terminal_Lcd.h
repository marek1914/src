#ifndef __C_BSTONE_SYSTEM_HAL_LCD_H
#define __C_BSTONE_SYSTEM_HAL_LCD_H

void Dev_Lcd_init(void);
void Dev_Lcd_Update(t_u8 *pBuf, t_u16 sx, t_u16 ex, t_u16 y);
void Dev_Lcd_EnableBl(t_bool bEn);
void Dev_Lcd_Draw_Point(t_u16 x,t_u16 y, t_u16 nColor);
void Dev_Lcd_Draw_HLine(t_u16 x,t_u16 y,t_u16 w,t_u16  nColor);
void Dev_Lcd_Draw_Buffer(t_u16 x,t_u16 y,t_u16 w, t_u16* pBuf);

#endif


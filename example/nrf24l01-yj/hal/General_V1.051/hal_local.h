#ifndef __C_HAL_LOCAL_H
#define __C_HAL_LOCAL_H

#include "pub_types.h"

typedef t_void (*t_hal_timout_fn)(void);   
typedef t_void (*t_hal_interrupt_fn)(void);
typedef t_bool (*t_hal_uart_fn)(t_u8*);

extern t_void hal_init(void);
extern t_void hal_standby(void);
extern t_void hal_unlock(void);
extern t_void hal_lock(void);
extern t_void hal_read_uid(t_u8 *uid, t_u8 len);
extern t_void hal_system_reset(void);

extern t_u32  hal_get_ms_32(void);
extern t_u16  hal_get_us_16(void);

extern t_void hal_uart_callback(t_hal_uart_fn pFn);
extern t_void hal_uart_request(void);

extern t_void hal_dly_callback(t_hal_timout_fn pFn);
extern t_void hal_dly_start(t_u16 us);
extern t_void hal_dly_stop(void);
extern t_void hal_dly_next(t_u16 us);
extern t_void hal_dly_sync(t_u16 us);

extern t_void hal_spi_csn(t_bool en);
extern t_u8   hal_spi_rw(t_u8 val);
extern t_void hal_spi_w_buf(t_u8* buf, t_u8 len);
extern t_void hal_spi_r_buf(t_u8* buf, t_u8 len);

extern t_void hal_radio_callback(t_hal_interrupt_fn pFn);
extern t_void hal_radio_enable(t_u8 en);
extern t_void hal_radio_enable_delay(void);

extern t_void hal_led_onoff(t_u8 cha, t_u8 on);
extern t_u8   hal_key_mask(void);

extern t_void hal_param_load(void* pbuf, t_u8 len);
extern t_void hal_param_save(void* pbuf, t_u8 len);

#endif



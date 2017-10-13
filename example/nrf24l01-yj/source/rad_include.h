#ifndef __C_RADIOCHIP_H
#define __C_RADIOCHIP_H

typedef t_void (*t_rad_timout_fn)(void);   
typedef t_void (*t_rad_receive_fn)(void);
typedef t_void (*t_rad_send_fn)(void);

extern t_void rad_init(void);
extern t_void rad_set_send_fn(t_rad_send_fn pFn);
extern t_void rad_set_timeout_fn(t_rad_timout_fn pFn);
extern t_void rad_set_receivce_fn(t_rad_receive_fn pFn);

extern t_void rad_rx_test(t_bool bEn);
extern t_void rad_set_freq(t_u8 ch);
extern t_void rad_set_rx_mode(t_u32 addr);
extern t_void rad_set_tx_mode(t_u32 addr, t_u8 *pbuf, t_u8 len);
extern t_void rad_enable_send(void);
extern t_void rad_disable(void);
extern t_u8*  rad_rx_buffer(void);
extern t_u8   rad_freq_num(void);

#endif


/*
 * Copyright (c) 2016 gj. All Rights Reserved.
 * register define
 */

#include <stdint.h>

#define _IO	volatile
#define FPGA_VER_REG		*(_IO uint32_t*)0x30000000
#define FPGA_VER_TEST		*(_IO uint32_t*)0x30000004

/* uart */
#define UART_RX_FIFO_STATUS	*(_IO uint8_t*)0x30000300
#define UART_RX_FIFO_CTRL	*(_IO uint8_t*)0x30000304
#define UART_RX_FIFO_DATA	*(_IO uint8_t*)0x30000308
#define UART_TX_FIFO_STATUS	*(_IO uint8_t*)0x3000030c
#define UART_TX_FIFO_CTRL	*(_IO uint8_t*)0x30000310
#define UART_TX_FIFO_DATA	*(_IO uint8_t*)0x30000314

/* rs485 */
#define RS485_CRC_en		*(_IO uint8_t*) 0x30000318
#define RS485_CRC_valid		*(_IO uint8_t*) 0x3000031c
#define RS485_rx_CRC_data	*(_IO uint16_t*)0x30000320
#define RS485_tx_CRC_data	*(_IO uint16_t*)0x30000324

/* flash */
#define FLASH_sector_addr	*(_IO uint16_t*)0x30000200
#define FLASH_other_addr	*(_IO uint16_t*)0x30000204
#define FLASH_Txdata		*(_IO uint16_t*)0x30000208
#define FLASH_Rxdata		*(_IO uint16_t*)0x3000020c
#define FLASH_rdy_by		*(_IO uint8_t*) 0x30000210
#define FLASH_wb_code		*(_IO uint8_t*) 0x30000214
#define FLASH_WORD_BY_TEn	*(_IO uint8_t*) 0x30000218

/* SRAM */
#define SRAM_TEST_REG		*(_IO uint8_t*) 0x30000400
#define SRAM_E_REG			*(_IO uint8_t*) 0x30000404
#define SRAM_RD				*(_IO uint8_t*) 0x30000408
#define SRAM_WR				*(_IO uint8_t*) 0x3000040c
#define SRAM_TEST_ADDR		*(_IO uint32_t*)0x30000410
#define SRAM_WR_DATA		*(_IO uint32_t*)0x30000414
#define SRAM_RD_DATA		*(_IO uint32_t*)0x30000418

#define Image_send_crl_reg	*(_IO uint8_t*)0x30000600
#define Image_send_finish	*(_IO uint8_t*)0x30000604

#define light_threshold		*(_IO uint8_t*) 0x30070000
#define light_pixel_rfd		*(_IO uint8_t*) 0x30070004
#define light_pixel_raddr	*(_IO uint8_t*) 0x30070008
#define light_pixel_data	*(_IO uint32_t*)0x3007000c

#define zone_ram_dv			*(_IO uint8_t*) 0x30000800
#define zone_ram_raddr		*(_IO uint16_t*)0x30000804
#define zone_ram_rdata		*(_IO uint8_t*) 0x30000808


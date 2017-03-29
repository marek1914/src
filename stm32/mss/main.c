/*
 * Copyright (c) 2016 gj. All Rights Reserved.
 */

#include <m2sxxx.h>
#include <string.h>
#include "core.h"


//static void delay(void);

/* check start/middle/end addr : 0x00000000 / 0x0003fff0 / 0x0007fff0 */
int sram_check(void)
{
	int i;
	uint32_t test_addr[3] = {0x00000000, 0x0003fff0, 0x0007fff0};
	uint32_t dst[16];
	uint32_t src[16] = {
		0x11111111, 0x22222222, 0x33333333, 0x44444444,
		0x55555555, 0x66666666, 0x77777777, 0x88888888,
		0x99999999, 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc,
		0xdddddddd, 0xeeeeeeee, 0xffffffff, 0x55aa55aa
	};

	for (i = 0; i < 3; i++) {
		sram_write(src, 16, test_addr[i]);
		sram_read(dst, 16, test_addr[i]);
		if ( memcmp(src, dst, 16*4))
			return 0;
	}
	return 1;
}

/* test last sector */
int flash_check(void)
{
	uint16_t dst[16];
	uint16_t src[16] = {
		0x1111, 0x2222, 0x3333, 0x4444,
		0x5555, 0x6666, 0x7777, 0x8888,
		0x9999, 0xaaaa, 0xbbbb, 0xcccc,
		0xdddd, 0xeeee, 0xffff, 0x55aa
	};

	return 1;
	flash_erase_sector(0x1ff);

	/* very slow!! TODO */
	flash_progam(src, 16, 0x1ff000);
	flash_read(dst, 16, 0x1ff000);
	return 1;
}

int cmos_check(void)
{
	return 1;
}

/* handle 3 messages in big looooooop */
void message_handler(void)
{
	uint8_t cmd[7];
	/* 60-2 Byte */
	uint8_t msg[58]; 
	struct self_check *self_check;
	struct bad_pixel *bad_pixel;
	struct light_pixel_info lpi;

	for(;;) {
		if (getevent()) {
			light_pixel_check(&lpi);
		} else {
			receivemsg7(cmd);
			switch (cmd[0]) {
			case 0x10:
				memset(msg, 0, 58);
				self_check = (struct self_check*)msg;
				self_check->token = 0x10;
				self_check->self = 0x55;
				self_check->sram = sram_check()?0x55:0xaa;
				self_check->flash = flash_check()?0x55:0xaa;
				self_check->fpga = fpga_check()?0x55:0xaa;
				self_check->reserve[0] = 0x55;
				self_check->reserve[1] = 0x55;
				self_check->cmos = cmos_check()?0x55:0xaa;
				self_check->reserve1 = 0x55;
				sendmsg60(msg);
				break;
			case 0x20:
				memset(msg, 0,58);
				bad_pixel = (struct bad_pixel*)msg;
				bad_pixel->token = 0x20;
				sendmsg60(msg);
				break;
			case 0x30:
				image_send_crl();
				break;
			default:
				break;
			}
		}
	}
}


int main(void) {
	uint32_t fpga_ver;

	fpga_ver = fpga_get_version();

	/* power on clear */
	uart_init();
	flash_init();

	message_handler();
	return 0;
}


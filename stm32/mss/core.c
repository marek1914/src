#include "core.h"

/* set light pixel threshold */
int light_pixel_thd_wr(uint8_t thd)
{
	light_threshold = thd;
	return 1;
}

void image_send_crl(void)
{
	Image_send_crl_reg = 0x01;
	while (!Image_send_finish);
	Image_send_crl_reg = 0x00;
}

uint32_t fpga_get_version(void)
{
	return FPGA_VER_REG;
}

int fpga_check(void)
{
	FPGA_VER_TEST = 0x55aa;
	if(FPGA_VER_TEST == 0x55aa)
		return 1;
	else
		return 0;
}


int sram_read(uint32_t *buf, int size, uint32_t addr)
{
	int i;

	if (addr > SRAM_MAX) {
		return -1;
	}
	SRAM_TEST_REG = 0x01;
	for (i = 0; i < size; i++) {
		SRAM_TEST_ADDR = addr+i;
		SRAM_RD = 0x01;
		SRAM_RD = 0x00;
		buf[i] = SRAM_RD_DATA;
		SRAM_RD = 0x01;
	}
	SRAM_TEST_REG = 0x00;
	return 1;
}


int sram_write(const uint32_t *buf, int size, uint32_t addr)
{
	int i;

	if (addr > SRAM_MAX) {
		return -1;
	}

	SRAM_TEST_REG = 0x01;

	for (i = 0; i < size; i++) {
		SRAM_TEST_ADDR = addr+i;
		SRAM_WR_DATA = buf[i];
		SRAM_WR = 0x0F;
		SRAM_WR = 0x00;
		SRAM_WR = 0x0F;
	}	
	SRAM_TEST_REG = 0x00;
	return 1;
}



uint8_t cmv_read(uint8_t addr)
{

	cmv_cfg_addr =addr;
	cmv_control  =0x00;
	cmv_control  =0x04;  //4: READ 8:WRITE
	cmv_control  =0x00;

	while(cmv_cfg_read_ok == 0x00){
	}
	return cmv_cfg_datrd;

}

void cmv_write(uint8_t addr,uint8_t data)
{

   //int i;
	//if (addr > 128)
	//	printf("addr is greater than 128, error");

	cmv_cfg_addr =addr;
	cmv_cfg_datwr =data;
	cmv_control  =0x00;
	cmv_control  =0x08;  //4: READ 8:WRITE
	cmv_control  =0x00;

	while(cmv_ctrl_state !=0x02)
	{};
}

void cmv_reset_only (void)
{
	cmv_control  =0x00;
	cmv_control  =0x01;  //1:rst_only 2:s_SPI_CONFIG_only 4: READ 8:WRITE 0x10:s_TRAINING,0x20:s_GRAB 0x40:s_EXT_INTE,0x80:rst , config,trainning
	cmv_control  =0x00;
}

void cmv_fpga_config_only (void)
{
	cmv_control  =0x00;
	cmv_control  =0x02;  //1:rst_only 2:s_SPI_CONFIG_only 4: READ 8:WRITE 0x10:s_TRAINING,0x20:s_GRAB 0x40:s_EXT_INTE,0x80:rst , config,trainning
	cmv_control  =0x00;
}


void cmv_training_only (void)
{
	cmv_control  =0x00;
	cmv_control  =0x10;  //1:rst_only 2:s_SPI_CONFIG_only 4: READ 8:WRITE 0x10:s_TRAINING, 0x40:s_EXT_INTE,0x80:rst , config,trainning
	cmv_control  =0x00;
}

void cmv_soft_inter (void)
{
	cmv_control  =0x00;
	cmv_control  =0x40;  //1:rst_only 2:s_SPI_CONFIG_only 4: READ 8:WRITE 0x10:s_TRAINING, 0x40:s_EXT_INTE,0x80:rst , config,trainning
	cmv_control  =0x00;
}

void cmv_fpga_initial (void)
{
	cmv_control  =0x00;
	cmv_control  =0x40;  //1:rst_only 2:s_SPI_CONFIG_only 4: READ 8:WRITE 0x10:s_TRAINING, 0x40:s_EXT_INTE,0x80:rst , config,trainning
	cmv_control  =0x00;
}

void cmv_soft_config (void)
{
	cmv_write(41,0x05);
	cmv_write(72,0x02);
	cmv_write(74,0x00);
	cmv_write(77,0x00);
	cmv_write(80,0x11);
	cmv_write(81,0x11);
	cmv_write(84,0x04);
	cmv_write(85,0x01);
	cmv_write(86,0x0e);
	cmv_write(87,0x0c);
	cmv_write(88,0x40);

	cmv_write(91,0x40);

	cmv_write(94,0x65);
	cmv_write(95,0x6a);
	cmv_write(98,0x6d);
	cmv_write(99,0x6d);
	cmv_write(100,0xa0);

	cmv_write(102,0x01);
	cmv_write(103,0x3d);

	cmv_write(114,0x03);
	cmv_write(116,0x09);

	cmv_write(118,0x01);
	cmv_write(121,0x02);
	cmv_write(123,0x62);
}


void uart_init(void)
{
	uint32_t i;
	/* clean tx/rx fifo */
	UART_TX_FIFO_CTRL = 0;
	UART_TX_FIFO_CTRL = 1;
	UART_TX_FIFO_CTRL = 0;

	UART_RX_FIFO_CTRL = 0;
	UART_RX_FIFO_CTRL = 1;
	UART_RX_FIFO_CTRL = 0;

	for (i = 0; i < 60000; i++) ;

}

uint8_t get_char(void)
{
	while ((UART_RX_FIFO_STATUS)&0x01);
	UART_RX_FIFO_CTRL = 0;
	UART_RX_FIFO_CTRL = 2;
	UART_RX_FIFO_CTRL = 0;
	return UART_RX_FIFO_DATA;
}

void put_char(uint8_t c)
{
	uint8_t status;
	//status = UART_TX_FIFO_STATUS;
	while ((UART_TX_FIFO_STATUS)&0x01);
	UART_TX_FIFO_DATA = c;
	UART_TX_FIFO_CTRL = 0;
	UART_TX_FIFO_CTRL = 2;
	UART_TX_FIFO_CTRL = 0;
}

void send_data(const char* buf)
{
	while(*buf) put_char(*buf++);
}

/* 
 * Flash 32Mbit(4M x 8-Bit/2M x 16-Bit)
 * word mode addr: 9bit(sector addr)+12bit(addr range)
 * Top Boot: 63(32k word) + 8(4k word) = 71 sector
 * Bottom Boot: 8(4k word)+63(32k word) = 71 sector 
 */

void flash_init(void)
{
	/* reset */
	FLASH_wb_code = 0x01;
	/* Word mode , must */
	FLASH_WORD_BY_TEn = 0x01;
}

/* caution! all data lose */
void flash_erase_chip(void)
{
	FLASH_wb_code = 0x04;
	while(!FLASH_rdy_by);
}

/* sector_addr 9bit */
int flash_erase_sector(uint16_t sector_addr)
{
	if (sector_addr > 0x1ff) {
		/* out of range */
		return -1;
	}
	FLASH_sector_addr = sector_addr;
	FLASH_other_addr = 0x0000;
	FLASH_wb_code = 0x03;

	while(!FLASH_rdy_by);
	return 1;
}

int flash_read(uint16_t *buf, int32_t size, uint32_t addr)
{
	int i;
	
	if (addr > 0x1fffff) {
		/* out of range */
		return -1;
	}
	FLASH_sector_addr = 0x0000;
	for (i = 0; i < 256; i++) {
		FLASH_other_addr = i;
		FLASH_wb_code = 0x05;
		buf[i] = FLASH_Rxdata;
	}
	return 1;
}

int flash_progam(const uint16_t *buf, int32_t size, uint32_t addr)
{
	int i;

	if (addr > 0x1fffff) {
		/* out of range */
		return -1;
	}

	FLASH_sector_addr = addr >> 12;
	FLASH_other_addr = addr & 0x0fff;

	for (i = 0; i < 256; i++) {
		FLASH_other_addr = i;
		FLASH_wb_code = 0x06;
		FLASH_Txdata = i;
		while(!FLASH_rdy_by);
	}

	return 1;
}

/* end flash */


void light_pixel_check(struct light_pixel_info *lpi)
{
	uint8_t n, i, zone;
	uint32_t tmp;

	/* line 0:even line 1:odd */
	static uint8_t is_odd = 0;

	lpi->number = 0;
	for (zone = 0; zone < 4; zone++) {
		uint8_t base_addr;

		base_addr = is_odd*128 + zone*32;
		/* data valid */
		//while (!light_pixel_rfd);
		light_pixel_rfd = 0;
		light_pixel_raddr = base_addr;

		n = light_pixel_data;
		if ( n ) {
			if (n>31) n = 31;
			for (i = 0; i < n; i++) {
				light_pixel_raddr = base_addr + 1 + i;
				tmp = light_pixel_data;
				/* 2bit(null)+11bit(x)+11bit(y)+8bit(value) */
				lpi->value[lpi->number + i] = (uint8_t)tmp;
				lpi->coord[lpi->number + i].y = tmp>>8 & 0x000007ff;
				lpi->coord[lpi->number + i].x = tmp>>19;
			}
			lpi->number += n;
		}
	}
	is_odd = !is_odd;
}

/* reset tx/rx CRC */
static void rs485_crc_reset(void)
{
	RS485_CRC_en = 0x00;
	RS485_CRC_en = 0x03;
}

/**
 * is_send = 1, get send crc
 * is_send = 0, get receive crc
 */
uint16_t get_crc(uint8_t is_send)
{
	uint16_t crc;
	if (is_send)
		crc = RS485_tx_CRC_data;
	else
		crc = RS485_rx_CRC_data;
	return crc;
}

/**
 * Send 60 byte message
 * return 0:ok  -1:fail
 */
int sendmsg60(const uint8_t *msg)
{
	int i;
	uint16_t crc;
	rs485_crc_reset();

	for (i = 0; i < 58; i++) {
		put_char(msg[i]);
	}
	crc = get_crc(1);
	/* low 8bit */
	put_char((uint8_t)crc);
	/* high 8bit */
	put_char(crc>>8);
	return 0;
}

/*
 * 10 03 e8 00 00   crc 16 96
 */
int receivemsg7(uint8_t *msg)
{
	int i;
	uint16_t crc, crc1;

	do {
		rs485_crc_reset();
		msg[0] = get_char();
	} while(msg[0]!=0x10 && msg[0]!=0x20 && msg[0]!=0x30);

	for (i = 1; i < 5; i++) {
		msg[i] = get_char();
	}
	crc = get_crc(0);
	msg[5] = get_char();
	msg[6] = get_char();	
	crc1 = msg[6]<<8 | msg[5];
	if (crc == crc1)
		return 1;
	else 
		return 0;
}

uint16_t get_temperature(void)
{
	uint8_t temp_h_addr;
	uint8_t temp_l_addr;

	uint16_t temperature_h;
	uint16_t temperature_l;
	uint16_t temperature;

	temp_h_addr =127;
	temperature_h = cmv_read(temp_h_addr);
	temp_l_addr =126;
	temperature_l = cmv_read(temp_l_addr);

	temperature =temperature_h *128 + temperature_l;

	return temperature;
}

int getevent(void)
{
	while ((UART_RX_FIFO_STATUS)&0x01 && !light_pixel_rfd);

	if (!light_pixel_rfd) {
		return 1;
	} else {
		return 0;
	}
}

/* 27*27 byte buf */
void get_image_zone(uint8_t *buf)
{
	int i, j;

	while(!zone_ram_dv);
	for (i = 0; i < 27; i++) {
		for (j = 0; j < 27; j++) {
			zone_ram_raddr = i<<5 | j;
			buf[i*27+j] = zone_ram_rdata;
		}
	}
	zone_ram_dv = 0;
}


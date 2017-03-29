#include "reg.h"

#define SRAM_MAX 512*1024-1

struct light_pixel_info {
	/* max 124 */
	uint8_t number;
	uint8_t value[124];
	struct {
		uint16_t x;
		uint16_t y;
	} coord[124];
};

/* 7Byte */
struct self_check_cmd {

	uint8_t token;
	uint16_t sync_cycle;
	uint8_t reserve[2];
	uint16_t crc;
};

/* not use */
struct bad_pixel_cmd {
	/* 0x20 */
	uint8_t token;
	uint8_t reserve[4];
	uint16_t crc;	
};

/* not use */
struct image_info_cmd {
	/* 0x30 */
	uint8_t token;
	uint8_t R0[4];
	uint16_t crc;
};

/* (60-2)Byte */
struct self_check {
	/* 0x10 */
	uint8_t token;
	/* ok:0x55, fail:0xaa */
	uint8_t self;
	uint8_t sram;
	uint8_t flash;
	uint8_t fpga;
	uint8_t reserve[2];
	uint8_t cmos;
	uint8_t reserve1;
	uint16_t integration_time;
	uint8_t pad[47];
};

struct bad_pixel {
	/* 0x20 */
	uint8_t token;
	uint8_t bad_number;
	struct {
		int16_t Y;
		int16_t Z;
	} coordinate[14];
};


int light_pixel_thd_wr(uint8_t thd);
void image_send_crl(void);
uint32_t fpga_get_version(void);
int fpga_check(void);

int sram_read(uint32_t *buf, int size, uint32_t addr);
int sram_write(const uint32_t *buf, int size, uint32_t addr);

void uart_init(void);
uint8_t get_char(void);
void send_data(const char* buf);

static void rs485_crc_reset(void);

void flash_init(void);
void flash_erase_chip(void);
int flash_erase_sector(uint16_t sector_addr);
int flash_read(uint16_t *buf, int32_t size, uint32_t addr);
int flash_progam(const uint16_t *buf, int32_t size, uint32_t addr);

void light_pixel_check(struct light_pixel_info *lpi);
int receivemsg7(uint8_t *msg);
int sendmsg60(const uint8_t *msg);

int fpga_check(void);

#include "stm32f1xx_hal.h"

GPIO_InitTypeDef iic_gpio;

void iic_init(void)
{
	iic_gpio.Mode = GPIO_MODE_OUTPUT_OD;
	iic_gpio.Pull = GPIO_NOPULL;
	iic_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	/* scl */
	iic_gpio.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &iic_gpio);
	/* sda */
	iic_gpio.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &iic_gpio);
}

void sda_out(void)
{
	iic_gpio.Mode = GPIO_MODE_OUTPUT_OD;
	HAL_GPIO_Init(GPIOB, &iic_gpio);
}

void sda_in(void)
{
	iic_gpio.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &iic_gpio);
}

void scl(unsigned char set)
{
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, set);
	HAL_Delay(1);
}
	
void sda(unsigned char set)
{
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, set);
	HAL_Delay(1);
}

unsigned char sda_read(void)
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
}

void _start(void)
{
    sda(1); scl(1); sda(0); scl(0);
}

void _stop(void)
{
    sda_out();
    sda(0); scl(1); sda(1);
}

void read_ack(void)
{
	/* wait sda */
	scl(1);
	scl(0); 
}

void write_ack(void)
{
	sda_out();
	sda(0); scl(1); scl(0);
}

void write_noack(void)
{
	sda_out();
	sda(1); scl(1); scl(0);
}

void send_byte(unsigned char byte)
{
    char i;

    for (i = 0; i < 8; i++) {
        /* bitbang... */
        sda(byte & 0x80);
        byte <<= 1;
        scl(1);
				scl(0); 
    }
}

unsigned char read_byte(void)
{
	unsigned char i, byte = 0;
	
	sda_in();
	for (i = 0; i < 8; i++) {
			scl(1);
			byte <<= 1;
			byte |= sda_read();
			scl(0);
	}
	return byte;
}

void iic_write_byte(unsigned short addr, unsigned char data)
{
	_start();
	send_byte(0xa0);
	read_ack();
	send_byte(addr>>8);
	read_ack();
	send_byte(addr);
	read_ack();
	send_byte(data);
	read_ack();
	_stop();
}

void iic_read_byte(unsigned short addr, unsigned char *data, unsigned short len)
{
	unsigned short i;
	
	_start();
	send_byte(0xa0);
	read_ack();
	send_byte(addr>>8);
	read_ack();
	send_byte(addr);
	read_ack();
	_start();
	send_byte(0xa1);
	read_ack();
	
	for (i = 0; i < len; i++) {
		data[i] = read_byte();
		if (i == len-1) {
			write_noack();
		} else {
			write_ack();
		}
	}
	_stop();
}
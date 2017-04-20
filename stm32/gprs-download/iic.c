/* Standard-mode max 100Hz */

static void _sendbyte(UI8 byte);
static UI8 _readack(void);
void _start(void);
void _stop(void);
void send_data(UI8 Send_Len, UI8 *Send_Data);
void rec_data(short int Length, char *Data);
void rec_data_noack(char *Data);

void delay(void)
{
}

void scl_high(void) 
{ delay(); }

void sda_high(void) 
{ delay(); }

void scl_low(void) 
{ delay(); }

void sda_low(void) 
{ delay(); }

void SDA_In(void) {}
void SDA_Out(void) {}
UI8 SDA_DATA(void) { return Bitkeydata; }

void send_rec_data(UI8 Send_Len, UI8 Receive_Len, UI8 *Send_Data, UI8 *Receive_Data)
{
	UI8 count = 0;
	UI8 cur_len = 0;

	send_data(Send_Len, Send_Data);
	if (Receive_Len != 0) {
		count = Receive_Len - 1;
		do {
			if (count <= 32)
				cur_len = count;
			else
				cur_len = 32;
			rec_data(cur_len, (char *)(Receive_Data + Receive_Len - 1 - count));
			count = count - cur_len;
		} while (count != 0);
		rec_data_noack((char *)(Receive_Data + Receive_Len - 1));
	}
	_stop();
}


void send_byte(unsigned char byte)
{
	char i;

	for (i = 0; i < 8; i++) {
		/* bitbang... */
		scl_low();
		if (byte & 0x80)
			sda_high();
		else
			sda_low();
		byte <<= 1;
		scl_high();
	}
}

unsigned char read_byte(void)
{
	unsigned char i, byte = 0;

	for (i = 0; i < 8; i++) {
		scl_high();
		byte <<= 1;
		byte |= SDA_DATA();
		scl_low();
	}
	return byte;
}


UI8 read_ack(void)
{
	UI8 ack = 1;
	UI8 ix = 100;

	ack = 1;

	SDA_HIGH();
	SDA_In();
	SCL_HIGH();

	while (ack && ix) {
		ack = SDA_DATA();
		ix--;
	}
	SCL_LOW();
	SDA_Out();
	return ack;
}


static void sendAck(UI8 status)
{
	SDA_Out();
	if (status)
		SDA_LOW();
	else
		SDA_HIGH();
	SCL_HIGH();
	SCL_LOW();
}

static UI8 I2C_GetByte(UI8 more)
{
	UI16 byte;
	SDA_In();
	byte = I2C_ReadByte();
	sendAck(more);
	return byte;
}

void _start(void)
{
	sda_high();
	scl_high();
	sda_low();
	scl_low();
}

void _stop(void)
{
	SDA_Out();
	sda_low();
	scl_high();
	sda_high();
}

void send_data(UI8 Send_Len, UI8 *Send_Data)
{
	UI8 i;
	_start();
	for (i = 0; i < Send_Len; i++, Send_Data++) {
		_sendbyte(*Send_Data);
		_readack();
	}
}

void rec_data(short int Length, char *Data)
{
	UI8 i;
	for (i = 0; i < Length; i++, Data++) 
		*Data = I2C_GetByte(1);
}

void rec_data_noack(char *Data) 
{ 
	*Data = I2C_GetByte(0); 
}


void main(unsigned long argc, void *argv)
{
	int i;
	RESULT TestStatus;
	UI8 Random[8], SK[8];
	UI8 Keys[8];
	UI8 ReadCheckSum[2];
	MMAC_GPIO_PIN_CONFIG pinCfg;
	MMAC_GPIO_CALLBACK callback;
	emmaBHIF.BHIF_ALTERNATE = 0x00000000; /* Set EMMA2SL Pin Configuration */
	MMAC_RTOS_Initialise();
	MMAC_GPIO_Initialise();
	// emmaZPIO.ZPIO_DIR0 |= 0x00000003;//SDA SCL Output
	// emmaZPIO.ZPIO_DIR0 |= 0x000c0000;
	// emmaZPIO.ZPIO_DATA0 = 0x00080000;

	DSI_init(1);
	for (i = 0; i < 2000; i++) DSI_printf(1, "gaojiegaojiegaojie\r\n");
	pinCfg.interruptMode = MMAC_GPIO_DISABLED;
	pinCfg.interruptStatus = 0;         /* not used */
	pinCfg.direction = MMAC_GPIO_WRITE; /*dir = out*/
	callback = NULL;

	MMAC_GPIO_PinConfigure(25, &pinCfg, callback);  // vcc
	MMAC_GPIO_PinConfigure(18, &pinCfg, callback);  // clk
	MMAC_GPIO_PinConfigure(23, &pinCfg, callback);  // sda

	MMAC_GPIO_SetBit(25, MMAC_GPIO_PIN_MODE_HIGH);
	MMAC_GPIO_SetBit(18, MMAC_GPIO_PIN_MODE_HIGH);
	MMAC_GPIO_SetBit(23, MMAC_GPIO_PIN_MODE_HIGH);

	AT0104_Init();
	DRV_AT0104_VerifyPassword(0x07, SecureCode, Write);
	DRV_AT0104_ReadConfigZone(0x0, 240, Rddata);

	DRV_AT0104_ReadConfigZone(0x0, 240, Rddata);
/*自定义F1算法*/


	DRV_AT0104_VerifyPassword(0x03, PassWord3 + 5, Read);
	DRV_AT0104_ReadUserZone(0, 12, ReadUserZone);
	DRV_AT0104_ReadChecksum(ReadCheckSum);

	DRV_AT0104_VerifyPassword(0x03, PassWord3 + 1, Write);
	DRV_AT0104_WriteUserZone(0, 12, 0, UserTest);
	DRV_AT0104_SendChecksum();

	DRV_AT0104_ReadUserZone(0, 12, ReadUserZone);
	DRV_AT0104_ReadUserZone(0, 12, ReadUserZone);

	for (i = 0; i < 32; i++) DSI_printf(1, "0x%02x ", ReadUserZone[i]);
#endif
}


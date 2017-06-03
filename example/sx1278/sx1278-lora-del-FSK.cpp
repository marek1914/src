/* http://wirelessopensource.com */

#include "SPI.h"
#include "SX1278.h"

#define Serial.println()
#define Serial.print()

SX1278::SX1278()
{
	_bandwidth = BW_125;
	_codingRate = CR_5;
	_spreadingFactor = SF_7;
	_channel = CH_1;
	_header = HEADER_ON;
	_CRC = CRC_ON;
	_modem = LORA;
	_power = 15;
	_packetNumber = 0;
	_reception = CORRECT_PACKET;
	_retries = 0;
	_maxRetries = 3;
	packet_sent.retry = _retries;
};

uint8_t SX1278::ON()
{
	uint8_t state = 2;

	// Powering the module
	pinMode(SX1278_SS, OUTPUT);
	digitalWrite(SX1278_SS, HIGH);
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);
	setMaxCurrent(0x1B);
	setLORA();
	return state;
}

void SX1278::OFF()
{
	SPI.end();
	// Powering the module
	pinMode(SX1278_SS, OUTPUT);
	digitalWrite(SX1278_SS, LOW);
}

uint8_t SX1278::readRegister(uint8_t address)
{
	uint8_t value = 0x00;
	digitalWrite(SX1278_SS, LOW);
	delay(1);
	bitClear(address, 7);
	SPI.transfer(address);
	value = SPI.transfer(0x00);
	digitalWrite(SX1278_SS, HIGH);
	return value;
}

void SX1278::writeRegister(uint8_t address, uint8_t data)
{
	digitalWrite(SX1278_SS, LOW);
	delay(1);
	bitSet(address, 7); 
	SPI.transfer(address);
	SPI.transfer(data);
	digitalWrite(SX1278_SS, HIGH);
}

/* Clears the interruption flags */
void SX1278::clearFlags()
{
	uint8_t st0;
	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	writeRegister(REG_IRQ_FLAGS, 0xFF);
	writeRegister(REG_OP_MODE, st0);
}

/* Sets the module in LoRa mode */
uint8_t SX1278::setLORA()
{
	uint8_t state = 2;
	uint8_t st0;

	writeRegister(REG_OP_MODE, FSK_SLEEP_MODE);
	writeRegister(REG_OP_MODE, LORA_SLEEP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	writeRegister(REG_MAX_PAYLOAD_LENGTH, MAX_LENGTH);

	writeRegister(REG_MODEM_CONFIG1, 0x72);
	writeRegister(REG_MODEM_CONFIG2, 0x70);
	writeRegister(REG_MODEM_CONFIG3, 0x00);

	_modem = LORA;
	return state;
}

uint8_t SX1278::getMode()
{
	uint8_t value;

	value = readRegister(REG_MODEM_CONFIG1);
	_bandwidth = (value >> 4);
	_codingRate = (value >> 1) & 0x07;
	value = readRegister(REG_MODEM_CONFIG2);
	_spreadingFactor = (value >> 4) & 0x0F;

	return 0;
}

/* Sets bandwidth, coding rate ,spreading factor */
int8_t SX1278::setMode(uint8_t mode)
{
	int8_t state = 2;
	uint8_t st0;
	uint8_t config1 = 0x00;
	uint8_t config2 = 0x00;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	switch (mode) {
		case 1:// better reach, medium time on air
			setCR(CR_5);
			setSF(SF_12);
			setBW(BW_125);
			break;
		case 2:// medium reach, less time on air
			setCR(CR_5);
			setSF(SF_12);
			setBW(BW_250);
			break;
		case 3:// worst reach, less time on air
			setCR(CR_5);
			setSF(SF_10);
			setBW(BW_125);
			break;
		case 4:// better reach, low time on air
			setCR(CR_5);
			setSF(SF_12);
			setBW(BW_500);
			break;
		case 5:// better reach, medium time on air
			setCR(CR_5);
			setSF(SF_10);
			setBW(BW_250);
			break;
		// better reach, worst time-on-air
		case 6:
			setCR(CR_5);
			setSF(SF_11);
			setBW(BW_500);
			break;
		// medium-high reach, medium-low time-on-air
		case 7:
			setCR(CR_5);
			setSF(SF_9);
			setBW(BW_250);
			break;
		// medium reach, medium time-on-air
		case 8:
			setCR(CR_5);
			setSF(SF_9);
			setBW(BW_500);
			break;
		// medium-low reach, medium-high time-on-air
		case 9:
			setCR(CR_5);
			setSF(SF_8);
			setBW(BW_500);
			break;
		// worst reach, less time_on_air
		case 10:
			setCR(CR_5);
			setSF(SF_7);
			setBW(BW_500);
			break;
		default:
			state = -1;
	};
	writeRegister(REG_OP_MODE, st0);
	return state;
}

/* implicit or explicit header mode */
uint8_t SX1278::getHeader()
{
	int8_t state = 2;

	// take out bit 2 from REG_MODEM_CONFIG1 indicates ImplicitHeaderModeOn
	if (bitRead(REG_MODEM_CONFIG1, 0) == 0) {  // explicit header mode (ON)
		_header = HEADER_ON;
		state = 1;
	} else {  // implicit header mode (OFF)
		_header = HEADER_OFF;
		state = 1;
	}

	state = 0;
	return state;
}

/* Sets the module in explicit header mode (header is sent) */
int8_t SX1278::setHeaderON()
{
	int8_t state = 2;
	uint8_t config1;

	config1 = readRegister(REG_MODEM_CONFIG1);
	if (_spreadingFactor == 6) {
		state = -1;  // Mandatory headerOFF with SF = 6
		Serial.println(F("Mandatory implicit header mode with spreading factor = 6 "));
	} else {
		config1 = config1 & B11111110;
		writeRegister(REG_MODEM_CONFIG1, config1);
	}
	if (_spreadingFactor != 6) {  // checking headerON taking out bit 2 from REG_MODEM_CONFIG1
		config1 = readRegister(REG_MODEM_CONFIG1);
		if (bitRead(config1, 0) == HEADER_ON) {
			state = 0;
			_header = HEADER_ON;
			Serial.println(F("## Header has been activated ##"));
		} else {
			state = 1;
		}
	}
	return state;
}

/* Sets the module in implicit header mode (header is not sent) */
int8_t SX1278::setHeaderOFF()
{
	uint8_t state = 2;
	uint8_t config1;

	// Read config1 to modify only the header bit
	config1 = readRegister(REG_MODEM_CONFIG1);
	// sets bit 2 from REG_MODEM_CONFIG1 = headerOFF
	config1 = config1 | B00000001;
	writeRegister(REG_MODEM_CONFIG1, config1);
	config1 = readRegister(REG_MODEM_CONFIG1);
	if (bitRead(config1, 2) == HEADER_OFF) {
		// checking headerOFF taking out bit 2 from REG_MODEM_CONFIG1
		state = 0;
		_header = HEADER_OFF;
		Serial.println(F("## Header has been desactivated ##"));
	} else {
		state = 1;
		Serial.println(F("** Header hasn't been desactivated ##"));
	}
	return state;
}

/* Indicates if module is configured with or without checking CRC */
uint8_t SX1278::getCRC()
{
	uint8_t value;

	value = readRegister(REG_MODEM_CONFIG2);
	if (bitRead(value, 2) == CRC_OFF) {  // CRCoff
		_CRC = CRC_OFF;
	} else {  // CRCon
		_CRC = CRC_ON;
	}
	return 0;
}

/* Sets the module with CRC on */
uint8_t SX1278::setCRC_ON()
{
	uint8_t config;
	config = readRegister(REG_MODEM_CONFIG2);
	config = config | B00000100;
	writeRegister(REG_MODEM_CONFIG2, config);

	return 0;
}

uint8_t SX1278::setCRC_OFF()
{
	int8_t state = 2;
	uint8_t config;

	config = readRegister(REG_MODEM_CONFIG2);
	config = config & B11111011;
	writeRegister(REG_MODEM_CONFIG2, config);

	return state;
}

int8_t SX1278::getSF()
{
	uint8_t config2;

	config2 = (readRegister(REG_MODEM_CONFIG2)) >> 4;
	_spreadingFactor = config2;

	return 0;
}

/* Sets SF spr: spreading factor value to set in LoRa */
uint8_t SX1278::setSF(uint8_t spr)
{
	uint8_t st0;
	int8_t state = 2;
	uint8_t config2;
	uint8_t config3;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	/* 4-7 */
	config2 = readRegister(REG_MODEM_CONFIG2);
	// Read config3 to modify only the LowDataRateOptimize
	config3 = readRegister(REG_MODEM_CONFIG3);

	switch (spr) {
		case SF_6:
			config2 = config2 & 0x0f | 6 << 4;
			break;
		case SF_7:
			config2 = config2 & 0x0f | 7 << 4;
			break;

		case SF_8:
			config2 = config2 & 0x0f | 8 << 4;
			break;

		case SF_9:
			config2 = config2 & 0x0f | 9 << 4;
			break;

		case SF_10:
			config2 = config2 & 0x0f | 10 << 4;
			break;

		case SF_11:
			config2 = config2 & 0x0f | 11 << 4;
			getBW();
			if (_bandwidth == BW_125) {  // LowDataRateOptimize
				config3 = config3 | 0x08;
			}
			break;

		case SF_12:
			config2 = config2 & 0x0f | 12 << 4;

			getBW();
			if (_bandwidth == BW_125) {  // LowDataRateOptimize
				config3 = config3 | 0x08;
			}
			break;
	}

	// Check if it is neccesary to set special settings for SF=6
	if (spr == SF_6) {
		// Mandatory headerOFF with SF = 6 (Implicit mode)
		setHeaderOFF();

		// Set the bit field DetectionOptimize of
		// register RegLoRaDetectOptimize to value "0b101".
		writeRegister(REG_DETECT_OPTIMIZE, 0x05);
		// Write 0x0C in the register RegDetectionThreshold.
		writeRegister(REG_DETECTION_THRESHOLD, 0x0C);
	} else {
		// LoRa detection Optimize: 0x03 --> SF7 to SF12
		writeRegister(REG_DETECT_OPTIMIZE, 0x03);
		// LoRa detection threshold: 0x0A --> SF7 to SF12
		writeRegister(REG_DETECTION_THRESHOLD, 0x0A);
	}

	// sets bit 1-0 of REG_MODEM_CONFIG2 (SymbTimout) and bit 2 of
	// REG_MODEM_CONFIG3 (AgcAutoOn) for any SF value
	config2 = config2 | B00000011;
	config3 = config3 | B00000100;

	writeRegister(REG_MODEM_CONFIG2, config2);
	writeRegister(REG_MODEM_CONFIG3, config3);
	writeRegister(REG_OP_MODE, st0);

	return 0;
}

int8_t SX1278::getBW()
{
	_bandwidth = readRegister(REG_MODEM_CONFIG1) >> 4;
	return 0;
}

/* bandwith */
int8_t SX1278::setBW(uint16_t band)
{
	uint8_t st0;
	uint8_t config1;
	uint8_t config3;

	st0 = readRegister(REG_OP_MODE);

	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	config1 = readRegister(REG_MODEM_CONFIG1); 
	config3 = readRegister(REG_MODEM_CONFIG3);

	getSF();
	config1 &= 0x0f;

	config1 |= band << 4;

	if (band < 8 && (_spreadingFactor == 11 || _spreadingFactor == 12 )) {
		config3 = config3 | 0x08;
	}

	writeRegister(REG_MODEM_CONFIG1, config1);
	writeRegister(REG_MODEM_CONFIG3, config3);

	_bandwidth = band;

	writeRegister(REG_OP_MODE, st0);
	return 0;
}

/* coding rate */
int8_t SX1278::getCR()
{
	uint8_t config1;
	config1 = readRegister(REG_MODEM_CONFIG1) >> 1;
	config1 = config1 & B00000111;
	_codingRate = config1;
	return 0;
}

/* coding rate */
int8_t SX1278::setCR(uint8_t cod)
{
	uint8_t st0, i;
	uint8_t config1;

	st0 = readRegister(REG_OP_MODE);

	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	config1 = readRegister(REG_MODEM_CONFIG1);

	for (i = 1; i <= 4; i++) {
		config1 = config1 & 0xf1 | i;
	}

	writeRegister(REG_MODEM_CONFIG1, config1);
	_codingRate = cod;

	writeRegister(REG_OP_MODE, st0);
	return 0;
}

uint8_t SX1278::getChannel()
{
	uint32_t ch;
	uint8_t freq3;
	uint8_t freq2;
	uint8_t freq1;

	freq3 = readRegister(REG_FRF_MSB);  // MSB
	freq2 = readRegister(REG_FRF_MID);  // MID
	freq1 = readRegister(REG_FRF_LSB);  // LSB
	ch = ((uint32_t)freq3 << 16) + ((uint32_t)freq2 << 8) + (uint32_t)freq1;
	_channel = ch;

	return 0;
}

int8_t SX1278::setChannel(uint32_t ch)
{
	uint8_t st0;
	int8_t state = 2;
	unsigned int freq3;
	unsigned int freq2;
	uint8_t freq1;
	uint32_t freq;

	st0 = readRegister(REG_OP_MODE);
	
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	freq3 = ((ch >> 16) & 0x0FF);  // MSB
	freq2 = ((ch >> 8) & 0x0FF);   // MIB
	freq1 = (ch & 0xFF);           // LSB

	writeRegister(REG_FRF_MSB, freq3);
	writeRegister(REG_FRF_MID, freq2);
	writeRegister(REG_FRF_LSB, freq1);

	_channel = ch;

	writeRegister(REG_OP_MODE, st0);
	return state;
}

uint8_t SX1278::getPower()
{
	uint8_t value = 0x00;

	value = readRegister(REG_PA_CONFIG);
	value = value & B00001111;
	// Pout= 17-(15-OutputPower) = OutputPower+2
	value = value + 2;
	_power = value;
	if ((value >= 2) && (value <= 20)) {
	}
	return 0;
}

int8_t SX1278::setPower(char p)
{
	uint8_t st0;
	uint8_t value = 0x00;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	writeRegister(REG_PA_DAC, 0x84);

	switch (p) {
		case 'M': //max 20dbm
			_power = 0xFF;
			writeRegister(REG_PA_DAC, 0x87);
			break;
		case 'H': //high 14dbm
			_power = 0xFC;
			break;
		case 'I': //intermediate 8dbm
			_power = 0xF6;
			break;
		case 'L': //low 2dbm
			_power = 0xF0;
			break;
		default:
			state = -1;
			break;
	}
	writeRegister(REG_PA_CONFIG, _power);
	writeRegister(REG_OP_MODE, st0);
	return 0;
}

/* Sets the signal power indicated as input to the module pow: 0 to 14 dBm */
int8_t SX1278::setPowerNum(uint8_t pow)
{
	uint8_t st0;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	if (pow<2 || pow>20) {
		return -1;
	}
	if (pow <= 17) {// Pout= 17-(15-OutputPower) = OutputPower+2
		writeRegister(REG_PA_DAC, 0x84);
		pow = pow - 2;
	} else {  // Power > 17dbm -> Power = 20dbm
		writeRegister(REG_PA_DAC, 0x87);
		pow = 15;
	}
	_power = pow;

	writeRegister(REG_PA_CONFIG, _power);
	writeRegister(REG_OP_MODE, st0);
	return state;
}

/* Gets preamble length */
uint8_t SX1278::getPreambleLength()
{
	uint8_t p_length;
	p_length = readRegister(REG_PREAMBLE_MSB_LORA);
	_preamblelength = (p_length << 8) & 0xFFFF;
	p_length = readRegister(REG_PREAMBLE_LSB_LORA);
	_preamblelength = _preamblelength + (p_length & 0xFFFF);
	return 0;
}

/* Sets preamble length */
uint8_t SX1278::setPreambleLength(uint16_t l)
{
	uint8_t st0;
	uint8_t p_length;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	p_length = ((l >> 8) & 0x0FF);
	writeRegister(REG_PREAMBLE_MSB_LORA, p_length);
	p_length = (l & 0x0FF);
	writeRegister(REG_PREAMBLE_LSB_LORA, p_length);
	writeRegister(REG_OP_MODE, st0);
	return 0;
}

uint8_t SX1278::getPayloadLength()
{
	_payloadlength = readRegister(REG_PAYLOAD_LENGTH_LORA);
	return 0;
}

int8_t SX1278::setPacketLength()
{
	uint16_t length;

	length = _payloadlength + OFFSET_PAYLOADLENGTH;
	return setPacketLength(length);
}

int8_t SX1278::setPacketLength(uint8_t l)
{
	uint8_t st0;
	uint8_t value = 0x00;
	int8_t state = 2;

	st0 = readRegister(REG_OP_MODE);
	packet_sent.length = l;
	
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	writeRegister(REG_PAYLOAD_LENGTH_LORA, packet_sent.length);

	writeRegister(REG_OP_MODE, st0);
	return state;
}

uint8_t SX1278::getNodeAddress()
{
	return 0;
}

/* add<255  */
int8_t SX1278::setNodeAddress(uint8_t addr)
{
	_nodeAddress = addr;
	// in LoRa mode, address is SW controlled
	return state;
}

/* Gets the SNR value */
int8_t SX1278::getSNR()
{ 
	uint8_t value;

	value = readRegister(REG_PKT_SNR_VALUE);
	if (value & 0x80) {
		value = ((~value + 1) & 0xFF) >> 2;
		_SNR = -value;
	} else {
		_SNR = (value & 0xFF) >> 2;
	}
	return 0;
}

uint8_t SX1278::getRSSI()
{
	int rssi_mean = 0;

	for (int i = 0; i < 5; i++) {
		_RSSI = -OFFSET_RSSI + readRegister(REG_RSSI_VALUE_LORA);
		rssi_mean += _RSSI;
	}
	rssi_mean = rssi_mean / 5;
	_RSSI = rssi_mean;

	return 0;
}

/* Gets the RSSI of the last packet received */
int16_t SX1278::getRSSIpacket()
{  // RSSIpacket only exists in LoRa
	int8_t state = 2;

	state = getSNR();
	if (state == 0) {
		if (_SNR < 0) {
			_RSSIpacket = -NOISE_ABSOLUTE_ZERO +
						  10.0 * SignalBwLog[_bandwidth] + NOISE_FIGURE +
						  (double)_SNR;
			state = 0;
		} else {
			_RSSIpacket = readRegister(REG_PKT_RSSI_VALUE);
			_RSSIpacket = -OFFSET_RSSI + (double)_RSSIpacket;
			state = 0;
		}
	}
	return state;
}

uint8_t SX1278::setRetries(uint8_t ret)
{
	_maxRetries = ret;
	return 0;
}

uint8_t SX1278::getMaxCurrent()
{
	int8_t state = 2;
	uint8_t value;

	state = 1;
	_maxCurrent = readRegister(REG_OCP);

	// extract only the OcpTrim value from the OCP register
	_maxCurrent &= B00011111;

	if (_maxCurrent <= 15) {
		value = (45 + (5 * _maxCurrent));
	} else if (_maxCurrent <= 27) {
		value = (-30 + (10 * _maxCurrent));
	} else {
		value = 240;
	}
	_maxCurrent = value;
	Serial.print(F("## Maximum current supply configured is "));
	Serial.print(value, DEC);
	Serial.println(F(" mA ##"));
	state = 0;
	return state;
}

/* Limits current of the power amplifier */
int8_t SX1278::setMaxCurrent(uint8_t rate)
{
	uint8_t st0;

	if (rate > 0x1B)
		rate = 0x1B;

	/* Enable Over Current Protection */
	rate |= 0x20;

	st0 = readRegister(REG_OP_MODE);
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	writeRegister(REG_OCP, rate);
	writeRegister(REG_OP_MODE, st0);
	return 0;
}

uint8_t SX1278::getRegs()
{
	state = getMode();  // Stores the BW, CR and SF.
	state = getPower();  // Stores the power.
	state = getChannel();  // Stores the channel.
	state = getCRC();  // Stores the CRC configuration.
	state = getHeader();  // Stores the header configuration.
	state = getPreambleLength();  // Stores the preamble length.
	state = getPayloadLength();  // Stores the payload length.
	state = getNodeAddress();  // Stores the node address.
	state = getMaxCurrent();  // Stores the maximum current supply.
	state_f = getTemp();  // Stores the module temperature.
	return 0;
}

uint8_t SX1278::truncPayload(uint16_t length16)
{
	if (length16 > MAX_PAYLOAD) { //251
		_payloadlength = MAX_PAYLOAD;
	return 0;
}

uint8_t SX1278::setACK()
{
	uint8_t state = 2;

	clearFlags();

	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	// Setting ACK length in order to send it
	state = setPacketLength(ACK_LENGTH);
	if (state == 0) {
		// Setting ACK
		memset(&ACK, 0x00, sizeof(ACK));
		ACK.dst = packet_received.src;  // ACK destination is packet source
		ACK.src = packet_received.dst;  // ACK source is packet destination
		ACK.packnum = packet_received.packnum;
		ACK.length = 0;  // length = 0 to show that's an ACK
		ACK.data[0] = _reception;  // CRC of the received packet

		// Setting address pointer in FIFO data buffer
		writeRegister(REG_FIFO_ADDR_PTR, 0x00);
		writeRegister(REG_FIFO_TX_BASE_ADDR, 0x00);

		state = 1;

		// Writing ACK to send in FIFO
		writeRegister(REG_FIFO, ACK.dst);  // Writing the destination in FIFO
		writeRegister(REG_FIFO, ACK.src);  // Writing the source in FIFO
		writeRegister(REG_FIFO, ACK.packnum);  // Writing the packet number in FIFO
		writeRegister(REG_FIFO, ACK.length);  // Writing the packet length in FIFO
		writeRegister(REG_FIFO, ACK.data[0]);  // Writing the ACK in FIFO

		Serial.println(F("## ACK set and written in FIFO ##"));
		// Print the complete ACK if debug_mode
		Serial.println(F("## ACK to send:"));
		Serial.print(ACK.dst, HEX);
		Serial.print("|");
		Serial.print(ACK.src, HEX);
		Serial.print("|");
		Serial.print(ACK.packnum, HEX);
		Serial.print("|");
		Serial.print(ACK.length, HEX);
		Serial.print("|");
		Serial.print(ACK.data[0], HEX);
		Serial.println(F(" ##"));

		state = 0;
		_reception = CORRECT_PACKET;  // Updating value to next packet

		delay(500);
	}
	return state;
}

uint8_t SX1278::receive()
{
	uint8_t state = 1;

	// Initializing packet_received struct
	memset(&packet_received, 0x00, sizeof(packet_received));

	writeRegister(0x31, 0x43);
	writeRegister(REG_PA_RAMP, 0x09);
	writeRegister(REG_LNA, 0x23);
	writeRegister(REG_FIFO_ADDR_PTR, 0x00);
	writeRegister(REG_SYMB_TIMEOUT_LSB, 0xFF);
	writeRegister(REG_FIFO_RX_uint8_t_ADDR, 0x00);

	// Proceed depending on the protocol selected
	// With MAX_LENGTH gets all packets with length < MAX_LENGTH
	state = setPacketLength(MAX_LENGTH);
	writeRegister(REG_OP_MODE, LORA_RX_MODE);

// showRxRegisters();
	return state;
}

/* Configures the module to receive information */
uint8_t SX1278::receivePacketMAXTimeout()
{
	return receivePacketTimeout(MAX_TIMEOUT);
}

/* Configures the module to receive information */
uint8_t SX1278::receivePacketTimeout()
{
	setTimeout();
	return receivePacketTimeout(_sendTime);
}

/*
 Function: Configures the module to receive information.
 Returns: Integer that determines if there has been any error
*/
uint8_t SX1278::receivePacketTimeout(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	// set RX mode
	state = receive();

	// if RX mode is set correctly then wait for data
	if (state == 0) {
		// Wait for a new packet for 'wait' time
		if (availableData(wait)) {
			// If packet received, getPacket
			state_f = getPacket();
		} else {
			state_f = 1;
		}
	} else {
		state_f = state;
	}
	return state_f;
}

/*
 Function: Configures the module to receive information and send an ACK.
 Returns: Integer that determines if there has been any error
*/
uint8_t SX1278::receivePacketMAXTimeoutACK()
{
	return receivePacketTimeoutACK(MAX_TIMEOUT);
}

/*
 Function: Configures the module to receive information and send an ACK.
 Returns: Integer that determines if there has been any error
*/
uint8_t SX1278::receivePacketTimeoutACK()
{
	setTimeout();
	return receivePacketTimeoutACK(_sendTime);
}

/* Configures the module to receive information and send an ACK */
uint8_t SX1278::receivePacketTimeoutACK(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	// set RX mode
	state = receive();

	// if RX mode is set correctly then wait for data
	if (state == 0) {
		// Wait for a new packet for 'wait' time
		if (availableData(wait)) {
			// If packet received, getPacket
			state = getPacket();
		} else {
			state = 1;
			state_f = 3;  // There is no packet received
		}
	} else {
		state = 1;
		state_f = 1;  // There has been an error with the 'receive' function
	}

	if ((state == 0) || (state == 3)) {
		if (_reception == INCORRECT_PACKET) {
			state_f = 4;  // The packet has been incorrectly received
		} else {
			state_f = 1;  // The packet has been correctly received
		}
		state = setACK();
		if (state == 0) {
			state = sendWithTimeout();
			if (state == 0) {
				state_f = 0;
				Serial.println(F("This last packet was an ACK, so ..."));
				Serial.println(F("ACK successfully sent"));
			} else {
				state_f = 1;  // There has been an error with the
				              // 'sendWithTimeout' function
			}
		} else {
			state_f = 1;  // There has been an error with the 'setACK' function
		}
	} else {
		state_f = 1;
	}
	return state_f;
}

/*
 Function: Configures the module to receive all the information on air, before
 MAX_TIMEOUT expires.
 Returns: Integer that determines if there has been any error
*/
uint8_t SX1278::receiveAll() { return receiveAll(MAX_TIMEOUT); }
/*
 Configures the module to receive all the information on air.
*/
uint8_t SX1278::receiveAll(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t config1;

	state = receive();

	if (state == 0) {
		state = getPacket(wait);
	}
	return state;
}

/* checks its destination */
boolean SX1278::availableData() 
{ 
	return availableData(MAX_TIMEOUT); 
}
 
boolean SX1278::availableData(uint32_t wait)
{
	uint8_t value;
	uint8_t header = 0;
	boolean forme = false;
	unsigned long previous;

	_hreceived = false;

	previous = millis();

	value = readRegister(REG_IRQ_FLAGS);

	while ((bitRead(value, 4) == 0) && (millis() - previous < wait)) {
		value = readRegister(REG_IRQ_FLAGS);
		// avoid overflow
		if (millis() < previous) {
			previous = millis();
		}
	}

	// Check if Valid Header was received
	if (bitRead(value, 4) == 1) {
		_hreceived = true;
		while ((header == 0) && (millis() - previous < wait)) {
			header = readRegister(REG_FIFO_RX_BYTE_ADDR);
			// avoid overflow
			if (millis() < previous) {
				previous = millis();
			}
		}
		// Read first byte of the received packet
		if (header != 0) {
			_destination = readRegister(REG_FIFO);
		}
	} else {
		forme = false;
		_hreceived = false;
		Serial.println(F("** The timeout has expired **"));
	}

	/* use '_hreceived' because we need to ensure that '_destination' 
	 * is updated and is not the '_destination' from the previously packet
	 */
	if (_hreceived == true) {
		Serial.println(F("## Checking destination ##"));

		// Checking destination
		if ((_destination == _nodeAddress) ||
		    (_destination == BROADCAST_0)) {  // LoRa or FSK mode
			forme = true;
			Serial.println(F("## Packet received is for me ##"));
		} else {
			forme = false;
			Serial.println(F("## Packet received is not for me ##"));
			Serial.println(millis());

			// If it is not a correct destination address, change to
			// STANDBY to minimize power 
			writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
			}
		}
	} else {
		// If timeout then change to STANDBY 
		// writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);
	}
	return forme;
}

/* It gets and stores a packet if it is received before MAX_TIMEOUT expires */
uint8_t SX1278::getPacketMAXTimeout() { return getPacket(MAX_TIMEOUT); }

/* It gets and stores a packet if it is received */
int8_t SX1278::getPacket() { return getPacket(MAX_TIMEOUT); }

/*It gets and stores a packet if it is received before ending 'wait' time */
int8_t SX1278::getPacket(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;
	uint8_t value = 0x00;
	unsigned long previous;
	boolean p_received = false;

	previous = millis();

	/// LoRa mode
	value = readRegister(REG_IRQ_FLAGS);

	// Wait until the packet is received (RxDone flag) or the timeout expires
	while ((bitRead(value, 6) == 0) && (millis() - previous < wait)) {
		value = readRegister(REG_IRQ_FLAGS);
		// avoid overflow
		if (millis() < previous) {
			previous = millis();
		}
	}

	// Check if 'RxDone' is true and 'PayloadCrcError' is correct
	if ((bitRead(value, 6) == 1) && (bitRead(value, 5) == 0)) {
		// packet received & CRC correct
		p_received = true;  // packet correctly received
		_reception = CORRECT_PACKET;
		Serial.println(F("## Packet correctly received in LoRa mode ##"));
	} else {
		if (bitRead(value, 6) != 1) {
			Serial.println(F("NOT 'RxDone' flag"));
		}

		if (_CRC != CRC_ON) {
			Serial.println(F("NOT 'CRC_ON' enabled"));
		}

		if ((bitRead(value, 5) == 0) && (_CRC == CRC_ON)) {
			// CRC is correct
			_reception = CORRECT_PACKET;
		} else {
			// CRC incorrect
			_reception = INCORRECT_PACKET;
			state = 3;
			Serial.println(F("** The CRC is incorrect **"));
		}
	}

	/* If a new packet was received correctly, now the information must be
	 * filled inside the structures of the class
	 */
	if (p_received == true) {
		// Store the packet
		// LoRa
		writeRegister(REG_FIFO_ADDR_PTR, 0x00);
		packet_received.dst = readRegister(REG_FIFO);

		// Reading second byte of the received packet
		// Reading third byte of the received packet
		// Reading fourth byte of the received packet
		packet_received.src = readRegister(REG_FIFO);
		packet_received.packnum = readRegister(REG_FIFO);
		packet_received.length = readRegister(REG_FIFO);

		// calculate the payload length
		_payloadlength = packet_received.length - OFFSET_PAYLOADLENGTH;

		// check if length is incorrect
		if (packet_received.length > (MAX_LENGTH + 1)) {
			Serial.println(F("Corrupted packet, length must be less than 256"));
		} else {
			// Store payload in 'data'
			for (unsigned int i = 0; i < _payloadlength; i++) {
				packet_received.data[i] = readRegister(REG_FIFO);
			}
			// Store 'retry'
			packet_received.retry = readRegister(REG_FIFO);

// Print the packet if debug_mode
			Serial.println(F("## Packet received:"));
			Serial.print(packet_received.dst, HEX);
			Serial.print("|");
			Serial.print(packet_received.src, HEX);
			Serial.print("|");
			Serial.print(packet_received.packnum, HEX);
			Serial.print("|");
			Serial.print(packet_received.length, HEX);
			Serial.print("|");
			for (unsigned int i = 0; i < _payloadlength; i++) {
				Serial.print(packet_received.data[i], HEX);
				Serial.print("|");
			}
			Serial.print(packet_received.retry, HEX);
			Serial.println(F(" ##"));
			Serial.println();
			state_f = 0;
		}
	} else {
		// if packet was NOT received
		state_f = 1;
		if ((_reception == INCORRECT_PACKET) && (_retries < _maxRetries) &&
		    (state != 3)) {
			_retries++;
			Serial.println(F("## Retrying to send the last packet ##"));
		}
	}
	// Setting address pointer in FIFO data buffer to 0x00 again
	writeRegister(REG_FIFO_ADDR_PTR, 0x00);
	clearFlags();

	if (wait > MAX_WAIT) {
		state_f = -1;
		Serial.println(F("The timeout must be smaller than 12.5 seconds"));
	}
	return state_f;
}

/* sets the packet destination */
int8_t SX1278::setDestination(uint8_t dest)
{
	_destination = dest;
	packet_sent.dst = dest;
	packet_sent.src = _nodeAddress;
	packet_sent.packnum =_packetNumber;
	_packetNumber++;

	return 0;
}

uint8_t SX1278::setTimeout()
{
	uint16_t delay;

	delay = ((0.1 * _sendTime) + 1);

	float Tpacket = timeOnAir();
	_sendTime = (uint16_t)Tpacket + (rand() % delay) + 1000;

	return 0;
}

/*
 It gets the theoretical value of the time-on-air of the packet
 Float that determines the time-on-air
*/
float SX1278::timeOnAir() 
{ 
	return timeOnAir(_payloadlength); 
}

/* It gets the theoretical value of the time-on-air of the packet */
float SX1278::timeOnAir(uint16_t payloadlength)
{
	float BW;
	float DE = 0;
	float SF = _spreadingFactor;
	float PL = payloadlength + OFFSET_PAYLOADLENGTH;
	float H = _header;
	float CR = _codingRate;

	// Dara rate optimization enabled if SF is 11 or 12
	if (SF > 10)
		DE = 1.0;
	else
		DE = 0.0;

	// payload correction
	if (payloadlength == 0) PL = 255;

	// Bandwidth value setting
	if (_bandwidth == BW_125)
		BW = 125.0;
	else if (_bandwidth == BW_250)
		BW = 250.0;
	else if (_bandwidth == BW_500)
		BW = 500.0;
	else
		BW = 125.0;

	// Calculation steps:
	float Tsym = pow(2, SF) / (BW);       // ms
	float Tpreamble = (8 + 4.25) * Tsym;  // ms
	float argument1 = ceil((8.0 * PL - 4.0 * SF + 28.0 + 16.0 - 20.0 * H) /
	                       (4.0 * (SF - 2.0 * DE))) * (CR + 4.0);
	float argument2 = 0;
	float payloadSymbNb = 8 + max(argument1, argument2);
	float Tpayload = payloadSymbNb * Tsym;
	float Tpacket = Tpreamble + Tpayload;

	return Tpacket;
}

/* It sets a char array payload packet in a packet struct */
uint8_t SX1278::setPayload(char *payload)
{
	uint8_t state = 2;
	uint8_t state_f = 2;
	uint16_t length16;

	state = 1;
	length16 = (uint16_t)strlen(payload);
	truncPayload(length16);
	for (int i = 0; i < _payloadlength; i++) {
		packet_sent.data[i] = payload[i];
	}

	// Set length with the actual counter value
	// Setting packet length in packet structure
	state_f = setPacketLength();
	return state_f;
}

/* It sets an uint8_t array payload packet in a packet struct */
uint8_t SX1278::setPayload(uint8_t *payload)
{
	uint8_t state = 2;

	state = 1;
	for (unsigned int i = 0; i < _payloadlength; i++) {
		packet_sent.data[i] =
		    payload[i];  // Storing payload in packet structure
	}
	// set length with the actual counter value
	state = setPacketLength();  // Setting packet length in packet structure
	return state;
}

uint8_t SX1278::setPacket(uint8_t dest, char *payload)
{
	int8_t state = 2;
	uint8_t st0;

	st0 = readRegister(REG_OP_MODE);
	clearFlags();

	_reception = CORRECT_PACKET;

	if (_retries == 0) {
		// Updating these values only if it is the first try
		// Setting destination in packet structure
		state = setDestination(dest);
		if (state == 0) {
			state = setPayload(payload);
		}
	} else {
		state = setPacketLength();
		packet_sent.retry = _retries;
		Serial.print(F("** Retrying to send last packet "));
		Serial.print(_retries, DEC);
		Serial.println(F(" time **"));
	}

	// Setting address pointer in FIFO data buffer
	writeRegister(REG_FIFO_TX_BASE_ADDR, 0x00);
	writeRegister(REG_FIFO_ADDR_PTR, 0x00);
	if (state == 0) {
		state = 1;
		writeRegister(REG_FIFO, packet_sent.dst);
		writeRegister(REG_FIFO, packet_sent.src);
		writeRegister(REG_FIFO, packet_sent.packnum);
		writeRegister(REG_FIFO, packet_sent.length);
		for (uint16_t i = 0; i < _payloadlength; i++) {
			writeRegister(REG_FIFO, packet_sent.data[i]);
		}
		writeRegister(REG_FIFO, packet_sent.retry);
		state = 0;
		for (uint16_t i = 0; i < _payloadlength; i++) {
			Serial.print(packet_sent.data[i], HEX);
			Serial.print("|");
		}
		Serial.print(packet_sent.retry, HEX);
		Serial.println(F(" ##"));
	}
	writeRegister(REG_OP_MODE, st0);
	return state;
}

/* It sets a packet struct in FIFO in order to sent it */
uint8_t SX1278::setPacket(uint8_t dest, uint8_t *payload)
{
	int8_t state = 2;
	uint8_t st0;

	st0 = readRegister(REG_OP_MODE);
	clearFlags();

	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	_reception = CORRECT_PACKET;  // Updating incorrect value to send a packet
	                              // (old or new)
	if (_retries == 0) {          // Sending new packet
		state = setDestination(dest);  // Setting destination in packet structure
		if (state == 0) {
			state = setPayload(payload);
		}
	} else {
		state = setPacketLength();
		packet_sent.retry = _retries;
	}
	writeRegister(REG_FIFO_TX_BASE_ADDR, 0x00);
	writeRegister(REG_FIFO_ADDR_PTR, 0x00);
	if (state == 0) {
		state = 1;
		// Writing packet to send in FIFO
		writeRegister(REG_FIFO, packet_sent.dst);
		writeRegister(REG_FIFO, packet_sent.src);
		writeRegister(REG_FIFO, packet_sent.packnum);
		writeRegister(REG_FIFO, packet_sent.length);
		for (unsigned int i = 0; i < _payloadlength; i++) {
			writeRegister(REG_FIFO, packet_sent.data[i]);
		}
		writeRegister(REG_FIFO, packet_sent.retry);
		state = 0;
		Serial.println(F("## Packet set and written in FIFO ##"));
		// Print the complete packet if debug_mode
		for (unsigned int i = 0; i < _payloadlength; i++) {
			Serial.print(packet_sent.data[i], HEX);
			Serial.print("|");
		}
	}
	writeRegister(REG_OP_MODE, st0);
	return state;
}

uint8_t SX1278::sendWithMAXTimeout() 
{ 
	return sendWithTimeout(MAX_TIMEOUT); 
}

uint8_t SX1278::sendWithTimeout()
{
	setTimeout();
	return sendWithTimeout(_sendTime);
}

uint8_t SX1278::sendWithTimeout(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t value = 0x00;
	unsigned long previous;

	previous = millis();
	clearFlags();
	writeRegister(REG_OP_MODE, LORA_TX_MODE);

	value = readRegister(REG_IRQ_FLAGS);

	// Wait until the packet is sent (TX Done flag) or the timeout expires
	while ((bitRead(value, 3) == 0) && (millis() - previous < wait)) {
		value = readRegister(REG_IRQ_FLAGS);
		// avoid overflow!
		if (millis() < previous) {
			previous = millis();
		}
	}
	state = 1;

	if (bitRead(value, 3) == 1) {
		state = 0;
		Serial.println(F("Packet successfully sent"));
	} else {
		Serial.println(F("Timeout has expired"));
	}

	clearFlags();
	return state;
}

uint8_t SX1278::sendPacketMAXTimeout(uint8_t dest, char *payload)
{
	return sendPacketTimeout(dest, payload, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketMAXTimeout(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	return sendPacketTimeout(dest, payload, length16, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketTimeout(uint8_t dest, char *payload)
{
	uint8_t state = 2;

	state = setPacket(dest, payload);
	if (state == 0) {
		state = sendWithTimeout();
	}
	return state;
}

uint8_t SX1278::sendPacketTimeout(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	truncPayload(length16);
	state_f = setPacket(dest, payload);
	if (state_f == 0) {
		state_f = sendWithTimeout();
	}
	return state_f;
}

uint8_t SX1278::sendPacketTimeout(uint8_t dest, char *payload, uint32_t wait)
{
	uint8_t state = 2;

	state = setPacket(dest, payload);
	if (state == 0)	{
		state = sendWithTimeout(wait);
	}
	return state;
}

uint8_t SX1278::sendPacketTimeout(uint8_t dest, uint8_t *payload, uint16_t length16, uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	truncPayload(length16);
	setPacket(dest, payload);
	sendWithTimeout(wait);
	return state_f;
}

uint8_t SX1278::sendPacketMAXTimeoutACK(uint8_t dest, char *payload)
{
	return sendPacketTimeoutACK(dest, payload, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketMAXTimeoutACK(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	return sendPacketTimeoutACK(dest, payload, length16, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketTimeoutACK(uint8_t dest, char *payload)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	state = sendPacketTimeout(dest, payload);
	if (state == 0) {
		state = receive();
	} else {
		state_f = state;
	}
	if (state == 0) {
		if (availableData()) {
			state_f = getACK();
		} else {
			state_f = 9;
		}
	} else {
		state_f = state;
	}

	return state_f;
}

uint8_t SX1278::sendPacketTimeoutACK(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	// Sending packet to 'dest' destination
	state = sendPacketTimeout(dest, payload, length16);

	// Trying to receive the ACK
	if (state == 0) {
		state = receive();  // Setting Rx mode to wait an ACK
	} else {
		state_f = state;
	}
	if (state == 0) {
		if (availableData()) {
			state_f = getACK();
		} else {
			state_f = 9;
		}
	} else {
		state_f = state;
	}

	return state_f;
}

uint8_t SX1278::sendPacketTimeoutACK(uint8_t dest, char *payload, uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	state = sendPacketTimeout(dest, payload, wait);
	if (state == 0) {
		state = receive();
	} else {
		state_f = 1;
	}
	if (state == 0) {
		if (availableData()) {
			state_f = getACK();
		} else {
			state_f = 9;
		}
	} else {
		state_f = 1;
	}

	return state_f;
}

uint8_t SX1278::sendPacketTimeoutACK(uint8_t dest, uint8_t *payload,
                                     uint16_t length16, uint32_t wait)
{
	uint8_t state = 2;
	uint8_t state_f = 2;

	state = sendPacketTimeout(dest, payload, length16, wait);
	if (state == 0) {
		state = receive();
	} else {
		state_f = 1;
	}
	if (state == 0) {
		if (availableData()) {
			state_f = getACK();
		} else {
			state_f = 9;
		}
	} else {
		state_f = 1;
	}

	return state_f;
}

uint8_t SX1278::getACK() { return getACK(MAX_TIMEOUT); }

uint8_t SX1278::getACK(uint32_t wait)
{
	uint8_t state = 2;
	uint8_t value = 0x00;
	unsigned long previous;
	boolean a_received = false;

	previous = millis();

	value = readRegister(REG_IRQ_FLAGS);
	// Wait until the ACK is received (RxDone flag) or the timeout expires
	while ((bitRead(value, 6) == 0) && (millis() - previous < wait)) {
		value = readRegister(REG_IRQ_FLAGS);
		if (millis() < previous) {
			previous = millis();
		}
	}
	if (bitRead(value, 6) == 1) {  // ACK received
		a_received = true;
	}
	// Standby para minimizar el consumo
	writeRegister(REG_OP_MODE, LORA_STANDBY_MODE);

	if (a_received) {
		//----	writeRegister(REG_FIFO_ADDR_PTR, 0x00);  // Setting address
		//pointer in FIFO data buffer
		// Storing the received ACK
		ACK.dst = _destination;
		ACK.src = readRegister(REG_FIFO);
		ACK.packnum = readRegister(REG_FIFO);
		ACK.length = readRegister(REG_FIFO);
		ACK.data[0] = readRegister(REG_FIFO);

		// Checking the received ACK
		if (ACK.dst == packet_sent.src) {
			if (ACK.src == packet_sent.dst) {
				if (ACK.packnum == packet_sent.packnum) {
					if (ACK.length == 0) {
						if (ACK.data[0] == CORRECT_PACKET) {
							state = 0;
							// Printing the received ACK
							Serial.println(F("## ACK received:"));
							Serial.print(ACK.dst, HEX);
							Serial.print("|");
							Serial.print(ACK.src, HEX);
							Serial.print("|");
							Serial.print(ACK.packnum, HEX);
							Serial.print("|");
							Serial.print(ACK.length, HEX);
							Serial.print("|");
							Serial.print(ACK.data[0], HEX);
							Serial.println(F(" ##"));
							Serial.println();
						} else {
							state = 3;
						}
					} else {
						state = 4;
					}
				} else {
					state = 5;
				}
			} else {
				state = 6;
			}
		} else {
			state = 7;
		}
	} else {
		state = 8;
		Serial.println(F("** ACK lost **"));
	}
	clearFlags();
	return state;
}

uint8_t SX1278::sendPacketMAXTimeoutACKRetries(uint8_t dest, char *payload)
{
	return sendPacketTimeoutACKRetries(dest, payload, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketMAXTimeoutACKRetries(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	return sendPacketTimeoutACKRetries(dest, payload, length16, MAX_TIMEOUT);
}

uint8_t SX1278::sendPacketTimeoutACKRetries(uint8_t dest, char *payload)
{
	uint8_t state = 2;

	state = 1;
	while ((state != 0) && (_retries <= _maxRetries)) {
		state = sendPacketTimeoutACK(dest, payload);
		_retries++;
	}
	_retries = 0;

	return state;
}

uint8_t SX1278::sendPacketTimeoutACKRetries(uint8_t dest, uint8_t *payload, uint16_t length16)
{
	uint8_t state = 2;

	// Sending packet to 'dest' destination and waiting an ACK response.
	state = 1;
	while ((state != 0) && (_retries <= _maxRetries)) {
		state = sendPacketTimeoutACK(dest, payload, length16);
		_retries++;
	}
	_retries = 0;
	return state;
}

uint8_t SX1278::sendPacketTimeoutACKRetries(uint8_t dest, char *payload, uint32_t wait)
{
	uint8_t state = 2;

	state = 1;
	while ((state != 0) && (_retries <= _maxRetries)) {
		state = sendPacketTimeoutACK(dest, payload, wait);
		_retries++;
	}
	_retries = 0;
	return state;
}

uint8_t SX1278::sendPacketTimeoutACKRetries(uint8_t dest, uint8_t *payload,
                                            uint16_t length16, uint32_t wait)
{
	uint8_t state = 2;

	// Sending packet to 'dest' destination and waiting an ACK response.
	state = 1;
	while ((state != 0) && (_retries <= _maxRetries)) {
		state = sendPacketTimeoutACK(dest, payload, length16, wait);
		_retries++;
	}
	_retries = 0;

	return state;
}

uint8_t SX1278::getTemp()
{
	uint8_t st0;
	st0 = readRegister(REG_OP_MODE);  
	// Allowing access to FSK registers while in LoRa standby mode
	writeRegister(REG_OP_MODE, LORA_STANDBY_FSK_REGS_MODE);

	_temp = readRegister(REG_TEMP);
	if (_temp & 0x80) {
		_temp = ((~_temp + 1) & 0xFF);
	} else {
		_temp = (_temp & 0xFF);
	}
	writeRegister(REG_OP_MODE, st0);
	return 0;
}

void SX1278::showRxRegisters()
{
	uint8_t reg;
	for (int i = 0x00; i < 0x80; i++) {
		reg = readRegister(i);
	}
}

bool SX1278::cadDetected()
{
	uint8_t val = 0;
	unsigned long time = millis();
	// set LNA
	sx1278.writeRegister(REG_LNA, 0x23);
	sx1278.clearFlags();
	sx1278.getRSSI();

	Serial.print(F("Inside CAD DETECTION -> RSSI: "));
	Serial.println(sx1278._RSSI);

	Serial.println(F("Set CAD mode"));
	// Setting LoRa CAD mode
	sx1278.writeRegister(REG_OP_MODE, 0x87);
	// Wait for IRQ CadDone
	val = sx1278.readRegister(REG_IRQ_FLAGS);
	while ((bitRead(val, 2) == 0) && (millis() - time) < 10000) {
		val = sx1278.readRegister(REG_IRQ_FLAGS);
	}
	// After waiting or detecting CadDone
	// check 'CadDetected' bit in 'RegIrqFlags' register
	if (bitRead(val, 0) == 1) {
		Serial.println(F("CAD true"));
		return true;
	}
	Serial.println(F("CAD false"));
	return false;
}
SX1278 sx1278 = SX1278();

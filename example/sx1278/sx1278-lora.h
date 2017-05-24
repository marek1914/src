/* Library for managing Semtech modules */

#ifndef SX1278_h
#define SX1278_h

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef inttypes_h
#include <inttypes.h>
#endif

#define SX1278_debug_mode 0

#define SX1278_SS SS

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

/* 54 regs */
#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define REG_PA_CONFIG 0x09
#define REG_PA_RAMP 0x0A
#define REG_OCP 0x0B
#define REG_LNA 0x0C
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS_MASK 0x11
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13
#define REG_RX_HEADER_CNT_VALUE_MSB 0x14
#define REG_RX_HEADER_CNT_VALUE_LSB 0x15
#define REG_RX_PACKET_CNT_VALUE_MSB 0x16
#define REG_RX_PACKET_CNT_VALUE_LSB 0x17
#define REG_MODEM_STAT 0x18
#define REG_PKT_SNR_VALUE 0x19
#define REG_PKT_RSSI_VALUE 0x1A
#define REG_RSSI_VALUE_LORA 0x1B
#define REG_HOP_CHANNEL 0x1C
#define REG_MODEM_CONFIG1 0x1D
#define REG_MODEM_CONFIG2 0x1E
#define REG_SYMB_TIMEOUT_LSB 0x1F
#define REG_PREAMBLE_MSB_LORA 0x20
#define REG_PREAMBLE_LSB_LORA 0x21
#define REG_PAYLOAD_LENGTH_LORA 0x22
#define REG_MAX_PAYLOAD_LENGTH 0x23
#define REG_HOP_PERIOD 0x24
#define REG_FIFO_RX_BYTE_ADDR 0x25
#define REG_MODEM_CONFIG3 0x26
#define REG_FEI_MSB 0x28
#define REG_FEI_MID 0x29
#define REG_FEI_LSB 0x2A
#define REG_RSSI_WIDEBAND 0x2C
#define REG_DETECT_OPTIMIZE 0x31
#define REG_INVERT_IQ 0x33
#define REG_DETECTION_THRESHOLD 0x37
#define REG_SYNC_WORD 0x39
#define REG_DIO_MAPPING1 0x40
#define REG_DIO_MAPPING2 0x41
#define REG_VERSION 0x42
#define REG_TCXO 0x4B
#define REG_PA_DAC 0x4D
#define REG_FORMER_TEMP 0x5B
#define REG_AGC_REF 0x61
#define REG_AGC_THRESH1 0x62
#define REG_AGC_THRESH2 0x63
#define REG_AGC_THRESH3 0x64
#define REG_PLL 0x70

//cf: center frequency
const uint32_t CH_DEFAULT = 0x6c8000;  // default channel, cf: 434.000MHz

// FREQUENCY CHANNELS (BANDWIDTH 500KHz):
const uint32_t CH_1_BW_500 = 0x6c5345;  // cf: 433.3 ( 433.050 -433.550 )
const uint32_t CH_2_BW_500 = 0x6c7af3;  // cf: 433.92 ( 433.670 - 433.920 )
const uint32_t CH_3_BW_500 = 0x6ca2a1;  // cf: 434.54 ( 434.290 - 434.790 )

// FREQUENCY CHANNELS (BANDWIDTH 250KHz):
const uint32_t CH_1_BW_250 = 0x6c4b45;  // cf: 433.175 ( 433.050 - 433.300 )
const uint32_t CH_2_BW_250 = 0x6c5e57;  // cf: 433.473 ( 433.348 - 433.598 )
const uint32_t CH_3_BW_250 = 0x6c716a;  // cf: 433.771 ( 433.646 - 433.896 )
const uint32_t CH_4_BW_250 = 0x6c847c;  // cf: 434.069 ( 433.944 - 434.194 )
const uint32_t CH_5_BW_250 = 0x6c978f;  // cf: 434.367 ( 434.242 - 434.492 )
const uint32_t CH_6_BW_250 = 0x6caaa1;  // cf: 434.665 ( 434.540 - 434.790 )

// FREQUENCY CHANNELS (BANDWIDTH 125KHz):
const uint32_t CH_1_BW_125 = 0x6c4745;  // cf: 433.1125 ( 433.050 - 433.175 )
const uint32_t CH_2_BW_125 = 0x6c4fe1;  // cf: 433.247  ( 433.1845 - 433.3095 )
const uint32_t CH_3_BW_125 = 0x6c587c;  // cf: 433.3815 ( 433.319 - 433.444 )
const uint32_t CH_4_BW_125 = 0x6c6118;  // cf: 433.516  ( 433.4535 - 433.5785 )
const uint32_t CH_5_BW_125 = 0x6c69b3;  // cf: 433.6505 ( 433.588 - 433.713 )
const uint32_t CH_6_BW_125 = 0x6c724f;  // cf: 433.785  ( 433.7225 - 433.8475 )
const uint32_t CH_7_BW_125 = 0x6c7af3;  // cf: 433.92   ( 433.8575 - 433.9825 )
const uint32_t CH_8_BW_125 = 0x6c8397;  // cf: 434.055  ( 433.9925 - 434.1175 )
const uint32_t CH_9_BW_125 = 0x6c8c32;  // cf: 434.1895 ( 434.127 - 434.252 )
const uint32_t CH_10_BW_125 = 0x6c94ce; // cf: 434.324  ( 434.2615 - 434.3865 )
const uint32_t CH_11_BW_125 = 0x6c9d6a; // cf: 434.4585 ( 434.396 - 434.521 )
const uint32_t CH_12_BW_125 = 0x6ca605; // cf: 434.593  ( 434.5305 - 434.6555 )
const uint32_t CH_13_BW_125 = 0x6CaeA1; // cf: 434.7275 ( 434.665  - 434.790 )

// FREQUENCY CHANNELS (BANDWIDTH < 125KHz: separate 72.5KHz):
const uint32_t CH_1 = 0x6c4597;   // cf: 433.086MHz
const uint32_t CH_2 = 0x6c4a3b;   // cf: 433.159MHz
const uint32_t CH_3 = 0x6c4edf;   // cf: 433.231MHz
const uint32_t CH_4 = 0x6c5383;   // cf: 433.304MHz
const uint32_t CH_5 = 0x6c5827;   // cf: 433.376MHz
const uint32_t CH_6 = 0x6c5ccb;   // cf: 433.449MHz
const uint32_t CH_7 = 0x6c616f;   // cf: 433.521MHz
const uint32_t CH_8 = 0x6c6613;   // cf: 433.594MHz
const uint32_t CH_9 = 0x6c6ab7;   // cf: 433.666MHz
const uint32_t CH_10 = 0x6c6f5b;  // cf: 433.739MHz
const uint32_t CH_11 = 0x6c73ff;  // cf: 433.811MHz
const uint32_t CH_12 = 0x6c78a3;  // cf: 433.884MHz
const uint32_t CH_13 = 0x6c7d47;  // cf: 433.956MHz
const uint32_t CH_14 = 0x6c81eb;  // cf: 434.029MHz
const uint32_t CH_15 = 0x6c868f;  // cf: 434.101MHz
const uint32_t CH_16 = 0x6c8b33;  // cf: 434.174MHz
const uint32_t CH_17 = 0x6c8fd8;  // cf: 434.246MHz
const uint32_t CH_18 = 0x6c947c;  // cf: 434.319MHz
const uint32_t CH_19 = 0x6c9920;  // cf: 434.391MHz
const uint32_t CH_20 = 0x6c9dc4;  // cf: 434.464MHz
const uint32_t CH_21 = 0x6ca268;  // cf: 434.536MHz
const uint32_t CH_22 = 0x6ca70c;  // cf: 434.609MHz
const uint32_t CH_23 = 0x6cabb0;  // cf: 434.682MHz
const uint32_t CH_24 = 0x6cb054;  // cf: 434.754MHz

// LORA BANDWIDTH:
const uint8_t BW_7_8 = 0x00;
const uint8_t BW_10_4 = 0x01;
const uint8_t BW_15_6 = 0x02;
const uint8_t BW_20_8 = 0x03;
const uint8_t BW_31_2 = 0x04;
const uint8_t BW_41_7 = 0x05;
const uint8_t BW_62_5 = 0x06;
const uint8_t BW_125 = 0x07;
const uint8_t BW_250 = 0x08;
const uint8_t BW_500 = 0x09;

const double SignalBwLog[] = {5.0969100130080564143587833158265,
                              5.397940008672037609572522210551,
                              5.6989700043360188047862611052755};

// LORA CODING RATE:
const uint8_t CR_5 = 0x01;  // CR = 4/5
const uint8_t CR_6 = 0x02;  // CR = 4/6
const uint8_t CR_7 = 0x03;  // CR = 4/7
const uint8_t CR_8 = 0x04;  // CR = 4/8

// LORA SPREADING FACTOR:
const uint8_t SF_6 = 0x06;
const uint8_t SF_7 = 0x07;
const uint8_t SF_8 = 0x08;
const uint8_t SF_9 = 0x09;
const uint8_t SF_10 = 0x0A;
const uint8_t SF_11 = 0x0B;
const uint8_t SF_12 = 0x0C;

// LORA MODES:
const uint8_t LORA_SLEEP_MODE = 0x80;
const uint8_t LORA_STANDBY_MODE = 0x81;
const uint8_t LORA_TX_MODE = 0x83;
const uint8_t LORA_RX_MODE = 0x85;
const uint8_t LORA_STANDBY_FSK_REGS_MODE = 0xC1;

// FSK MODES:
const uint8_t FSK_SLEEP_MODE = 0x00;
const uint8_t FSK_STANDBY_MODE = 0x01;
const uint8_t FSK_TX_MODE = 0x03;
const uint8_t FSK_RX_MODE = 0x05;

// OTHER CONSTANTS:

const uint8_t st_SD_ON = 1;
const uint8_t st_SD_OFF = 0;
const uint8_t HEADER_ON = 0;
const uint8_t HEADER_OFF = 1;
const uint8_t CRC_ON = 1;
const uint8_t CRC_OFF = 0;
const uint8_t LORA = 1;
const uint8_t FSK = 0;
const uint8_t BROADCAST_0 = 0x00;
const uint8_t MAX_LENGTH = 255;
const uint8_t MAX_PAYLOAD = 251;
const uint8_t MAX_LENGTH_FSK = 64;
const uint8_t MAX_PAYLOAD_FSK = 60;
const uint8_t ACK_LENGTH = 5;
const uint8_t OFFSET_PAYLOADLENGTH = 5;
const uint8_t OFFSET_RSSI = 137;
const uint8_t NOISE_FIGURE = 6.0;
const uint8_t NOISE_ABSOLUTE_ZERO = 174.0;
const uint16_t MAX_TIMEOUT = 10000;     // 10000 msec = 10.0 sec
const uint32_t MAX_WAIT = 12000;        // 12000 msec = 12.0 sec
const uint32_t MESH_TIMEOUT = 3600000;  // 3600000 msec = 3600 sec = 1 hour
const uint8_t MAX_RETRIES = 5;
const uint8_t CORRECT_PACKET = 0;
const uint8_t INCORRECT_PACKET = 1;

struct pack {
	uint8_t dst;
	uint8_t src;
	uint8_t packnum;//Packet number
	uint8_t length; //Packet length
	uint8_t data[MAX_PAYLOAD];//Packet payload
	uint8_t retry;
};

class SX1278
{
   public:
	SX1278();
	uint8_t ON();
	void OFF();
	byte readRegister(byte address);
	void writeRegister(byte address, byte data);
	void clearFlags();
	uint8_t setLORA();

	// It sets the FSK mode on.
	uint8_t setFSK();

	// It gets the BW, SF and CR of the module.
	uint8_t getMode();

	// It sets the BW, SF and CR of the module.
	int8_t setMode(uint8_t mode);

	// It gets the header mode configured.
	uint8_t getHeader();

	// It sets explicit header mode.
	int8_t setHeaderON();

	// It sets implicit header mode.
	int8_t setHeaderOFF();
	uint8_t getCRC();
	uint8_t setCRC_ON();
	uint8_t setCRC_OFF();

	//! It is true if the SF selected exists.
	boolean isSF(uint8_t spr);

	int8_t getSF();
	uint8_t setSF(uint8_t spr);

	boolean isBW(uint16_t band);

	int8_t getBW();

	// It sets the BW.
	int8_t setBW(uint16_t band);

	// It is true if the CR selected exists.
	boolean isCR(uint8_t cod);

	// It gets the CR configured.
	int8_t getCR();

	// It sets the CR.
	int8_t setCR(uint8_t cod);

	// It is true if the channel selected exists.
	boolean isChannel(uint32_t ch);

	// It gets frequency channel the module is using.
	uint8_t getChannel();

	// It sets frequency channel the module is using.
	int8_t setChannel(uint32_t ch);

	// It gets the output power of the signal.
	uint8_t getPower();

	// It sets the output power of the signal.
	int8_t setPower(char p);

	// It sets the output power of the signal.
	int8_t setPowerNum(uint8_t pow);

	// It gets the preamble length configured.
	uint8_t getPreambleLength();

	// It sets the preamble length.
	uint8_t setPreambleLength(uint16_t l);

	// It gets the payload length of the last packet to send/receive.
	uint8_t getPayloadLength();

	// It sets the packet length to send/receive.
	int8_t setPacketLength();

	// It sets the packet length to send/receive.
	int8_t setPacketLength(uint8_t l);

	// It gets the node address of the mote.
	uint8_t getNodeAddress();

	// It sets the node address of the mote.
	int8_t setNodeAddress(uint8_t addr);

	// It gets the SNR of the latest received packet.
	int8_t getSNR();

	// It gets the current value of RSSI.
	uint8_t getRSSI();

	// It gets the RSSI of the latest received packet.
	int16_t getRSSIpacket();
	uint8_t setRetries(uint8_t ret);
	uint8_t getMaxCurrent();
	int8_t setMaxCurrent(uint8_t rate);

	// It gets the content of the main configuration registers.
	uint8_t getRegs();

	// It sets the maximum number of bytes from a frame that fit in a packet
	// structure.
	/*
	It stores in global '_payloadlength' variable the maximum number of bytes.
	\param uint16_t length16 : total frame length.
	\return '0' on success, '1' otherwise
	 */
	uint8_t truncPayload(uint16_t length16);

	// It writes an ACK in FIFO to send it.
	uint8_t setACK();

	// It puts the module in reception mode.
	uint8_t receive();

	// It receives a packet before MAX_TIMEOUT.
	uint8_t receivePacketMAXTimeout();

	// It receives a packet before a timeout.
	uint8_t receivePacketTimeout();

	// It receives a packet before a timeout.
	uint8_t receivePacketTimeout(uint32_t wait);

	// It receives a packet before MAX_TIMEOUT and reply with an ACK.
	uint8_t receivePacketMAXTimeoutACK();

	// It receives a packet before a timeout and reply with an ACK.
	uint8_t receivePacketTimeoutACK();

	// It receives a packet before a timeout and reply with an ACK.
	uint8_t receivePacketTimeoutACK(uint32_t wait);

	// It puts the module in 'promiscuous' reception mode.
	uint8_t receiveAll();

	// It puts the module in 'promiscuous' reception mode with a timeout.
	uint8_t receiveAll(uint32_t wait);

	// It checks if there is an available packet and its destination.
	boolean availableData();

	// It checks if there is an available packet and its destination before a
	// timeout.
	boolean availableData(uint32_t wait);

	// It writes a packet in FIFO in order to send it.
	uint8_t setPacket(uint8_t dest, char *payload);

	// It writes a packet in FIFO in order to send it.
	uint8_t setPacket(uint8_t dest, uint8_t *payload);

	// It reads a received packet from the FIFO, if it arrives before ending
	// MAX_TIMEOUT time.
	uint8_t getPacketMAXTimeout();

	// It reads a received packet from the FIFO, if it arrives before ending
	// '_sendTime' time.
	int8_t getPacket();

	// It receives and gets a packet from FIFO, if it arrives before ending
	// 'wait' time.
	int8_t getPacket(uint32_t wait);

	// It sends the packet stored in FIFO before ending MAX_TIMEOUT.
	uint8_t sendWithMAXTimeout();

	// It sends the packet stored in FIFO before ending _sendTime time.
	uint8_t sendWithTimeout();

	// It tries to send the packet stored in FIFO before ending 'wait' time.
	uint8_t sendWithTimeout(uint32_t wait);

	// It tries to send the packet which payload is a parameter before ending
	// MAX_TIMEOUT.
	uint8_t sendPacketMAXTimeout(uint8_t dest, char *payload);

	// It tries to send the packet which payload is a parameter before ending
	// MAX_TIMEOUT.
	uint8_t sendPacketMAXTimeout(uint8_t dest, uint8_t *payload, uint16_t length);

	// It sends the packet which payload is a parameter before ending
	// MAX_TIMEOUT.
	uint8_t sendPacketTimeout(uint8_t dest, char *payload);

	// It sends the packet which payload is a parameter before ending
	// MAX_TIMEOUT.
	uint8_t sendPacketTimeout(uint8_t dest, uint8_t *payload, uint16_t length);

	// It sends the packet which payload is a parameter before ending 'wait' time
	uint8_t sendPacketTimeout(uint8_t dest, char *payload, uint32_t wait);

	// It sends the packet which payload is a parameter before ending 'wait' time
	uint8_t sendPacketTimeout(uint8_t dest, uint8_t *payload, uint16_t length,
	                          uint32_t wait);

	//! It sends the packet which payload is a parameter before MAX_TIMEOUT,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketMAXTimeoutACK(uint8_t dest, char *payload);

	//! It sends the packet which payload is a parameter before MAX_TIMEOUT,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param uint8_t payload: packet payload.
	\param uint16_t length : payload buffer length.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketMAXTimeoutACK(uint8_t dest, uint8_t *payload,
	                                uint16_t length);

	//! It sends the packet which payload is a parameter before a timeout,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketTimeoutACK(uint8_t dest, char *payload);

	//! It sends the packet which payload is a parameter before a timeout,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param uint8_t payload: packet payload.
	\param uint16_t length : payload buffer length.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketTimeoutACK(uint8_t dest, uint8_t *payload,
	                             uint16_t length);

	//! It sends the packet which payload is a parameter before 'wait' time,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\param uint32_t wait : time to wait to send the packet.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketTimeoutACK(uint8_t dest, char *payload, uint32_t wait);

	//! It sends the packet which payload is a parameter before 'wait' time,
	//! and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param uint8_t payload: packet payload.
	\param uint16_t length : payload buffer length.
	\param uint32_t wait : time to wait to send the packet.
	\return '9'  --> The ACK lost (no data available)
	        '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t sendPacketTimeoutACK(uint8_t dest, uint8_t *payload,
	                             uint16_t length, uint32_t wait);

	// It sets the destination of a packet.
	int8_t setDestination(uint8_t dest);

	// It sets the waiting time to send a packet.
	uint8_t setTimeout();

	//! It gets the theoretical value of the time-on-air of the packet
	/*! \remarks http://www.semtech.com/images/datasheet/sx1278.pdf
	It stores in global '_sendTime' variable the time for each mode.
	\return float: time on air depending on module settings and packet length
	 */
	float timeOnAir();
	float timeOnAir(uint16_t payloadlength);

	// It sets the payload of the packet that is going to be sent.
	uint8_t setPayload(char *payload);

	// It sets the payload of the packet that is going to be sent.
	uint8_t setPayload(uint8_t *payload);

	// If an ACK is received, it gets it and checks its content.
	/*
	return '8'  --> The ACK lost
	       '7'  --> The ACK destination incorrectly received
	       '6'  --> The ACK source incorrectly received
	       '5'  --> The ACK number incorrectly received
	       '4'  --> The ACK length incorrectly received
	       '3'  --> N-ACK received
	       '2'  --> The ACK has not been received
	       '1'  --> not used (reserved)
	       '0'  --> The ACK has been received with no errors
	*/
	uint8_t getACK();

	// It receives and gets an ACK from FIFO, if it arrives before ending
	// 'wait' time.
	/*
	\param uint32_t wait : time to wait while there is no an ACK received.
	\return '8'  --> The ACK lost
	        '7'  --> The ACK destination incorrectly received
	        '6'  --> The ACK source incorrectly received
	        '5'  --> The ACK number incorrectly received
	        '4'  --> The ACK length incorrectly received
	        '3'  --> N-ACK received
	        '2'  --> The ACK has not been received
	        '1'  --> not used (reserved)
	        '0'  --> The ACK has been received with no errors
	*/
	uint8_t getACK(uint32_t wait);

	// It sends a packet, waits to receive an ACK and updates the _retries
	// value, before ending MAX_TIMEOUT time.
	/*
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketMAXTimeoutACKRetries(uint8_t dest, char *payload);

	// It sends a packet, waits to receive an ACK and updates the _retries
	// value, before ending MAX_TIMEOUT time.
	/*
	\param uint8_t dest : packet destination.
	\param uint8_t *payload : packet payload.
	\param uint16_t length : payload buffer length.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketMAXTimeoutACKRetries(uint8_t dest, uint8_t *payload,
	                                       uint16_t length);

	// It sends a packet, waits to receive an ACK and updates the _retries
	/*
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketTimeoutACKRetries(uint8_t dest, char *payload);

	// It sends a packet, waits to receive an ACK and updates the _retries
	/*
	\param uint8_t dest : packet destination.
	\param uint8_t *payload : packet payload.
	\param uint16_t length : payload buffer length.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketTimeoutACKRetries(uint8_t dest, uint8_t *payload,
	                                    uint16_t length);

	// It sends a packet, waits to receive an ACK and updates the _retries
	// value, before ending 'wait' time.
	/*
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\param uint32_t wait : time to wait while trying to send the packet.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketTimeoutACKRetries(uint8_t dest, char *payload,
	                                    uint32_t wait);

	// It sends a packet, waits to receive an ACK and updates the _retries
	// value, before ending 'wait' time.
	/*
	\param uint8_t dest : packet destination.
	\param uint8_t *payload : packet payload.
	\param uint16_t length : payload buffer length.
	\param uint32_t wait : time to wait while trying to send the packet.
	\return '0' on success, '1' otherwise
	*/
	uint8_t sendPacketTimeoutACKRetries(uint8_t dest, uint8_t *payload,
	                                    uint16_t length, uint32_t wait);

	// It gets the internal temperature of the module.
	/*
	It stores in global '_temp' variable the module temperature.
	\return '0' on success, '1' otherwise
	*/
	uint8_t getTemp();

	// It prints the registers related to RX via USB
	void showRxRegisters();

	// It sets the RTC settings with Meshlium timestamp configuration
	/* This function sends a special Frame to Meshlium (Meshlium's address must
	 * be '1'), and then Meshlium returns an answer with the timestamp. This
	 * function parses the info and sets the RTC Time and Date.
	 * \return  'true' on cad detected, 'false' if not detected
	*/
	bool cadDetected();

	// SD state.
	// st_SD = 00  --> SD_OFF
	// st_SD = 01  --> SD_ON
	uint8_t st_SD;

	//bandwidth configured in LoRa mode.
	// bandwidth = 0000  --> BW = 7.8KHz
	// bandwidth = 0001  --> BW = 10.4KHz
	// bandwidth = 0010  --> BW = 15.6KHz
	// bandwidth = 0011  --> BW = 20.8KHz
	// bandwidth = 0100  --> BW = 31.2KHz
	// bandwidth = 0101  --> BW = 41.7KHz
	// bandwidth = 0110  --> BW = 62.5KHz
	// bandwidth = 0111  --> BW = 125KHz
	// bandwidth = 1000  --> BW = 250KHz
	// bandwidth = 1001  --> BW = 500KHz
	uint8_t _bandwidth;

	// coding rate configured in LoRa mode.
	//  codingRate = 001  --> CR = 4/5
	//  codingRate = 010  --> CR = 4/6
	//  codingRate = 011  --> CR = 4/7
	//  codingRate = 100  --> CR = 4/8
	uint8_t _codingRate;

	// spreading factor configured in LoRa mode.
	// spreadingFactor = 6   --> SF = 6, 64 chips/symbol
	// spreadingFactor = 7   --> SF = 7, 128 chips/symbol
	// spreadingFactor = 8   --> SF = 8, 256 chips/symbol
	// spreadingFactor = 9   --> SF = 9, 512 chips/symbol
	// spreadingFactor = 10  --> SF = 10, 1024 chips/symbol
	// spreadingFactor = 11  --> SF = 11, 2048 chips/symbol
	// spreadingFactor = 12  --> SF = 12, 4096 chips/symbol
	uint8_t _spreadingFactor;

	// frequency channel.
	// channel = 0x6b0000 --> CH = 1,  28MHz
	// channel = 0x6b2000 --> CH = 2,  28.5MHz
	// channel = 0x6b4000 --> CH = 3,  29MHz
	// channel = 0x6b6000 --> CH = 4,  29.5MHz
	// channel = 0x6b8000 --> CH = 5,  30MHz
	// channel = 0x6ba000 --> CH = 6,  30.5MHz
	// channel = 0x6bc000 --> CH = 7,  31MHz
	// channel = 0x6be000 --> CH = 8,  31.5MHz
	// channel = 0x6c0000 --> CH = 9,  32MHz
	// channel = 0x6c2000 --> CH = 10, 432.5MHz
	// channel = 0x6c4000 --> CH = 11, 433MHz
	// channel = 0x6c6000 --> CH = 12, 433.5MHz
	// channel = 0x6c8000 --> CH = 13, 434MHz
	// channel = 0x6ca000 --> CH = 14, 434.5MHz
	// channel = 0x6cc000 --> CH = 15, 435MHz
	// channel = 0x6ce000 --> CH = 16, 435.5MHz
	// channel = 0x6d0000 --> CH = 17, 436MHz
	// channel = 0x6d2000 --> CH = 18, 436.5MHz
	// channel = 0x6d4000 --> CH = 19, 437MHz
	// channel = 0x6d6000 --> CH = 20, 437.5MHz
	// channel = 0x6d8000 --> CH = 21, 438MHz
	// channel = 0x6da000 --> CH = 22, 438.5MHz
	// channel = 0x6dc000 --> CH = 23, 439MHz
	// channel = 0x6de000 --> CH = 24, 439.5MHz
	uint32_t _channel;
	uint8_t _power;
	int8_t _SNR;
	int8_t _RSSI;
	int16_t _RSSIpacket;
	uint16_t _preamblelength;
	uint16_t _payloadlength;
	uint8_t _nodeAddress;
	uint8_t _header;
	uint8_t _hreceived;//header received while waiting a packet to arrive.
	uint8_t _CRC;
	uint8_t _destination;
	uint8_t _packetNumber;
	uint8_t _reception;
	uint8_t _retries;
	uint8_t _maxRetries;
	uint8_t _maxCurrent;
	uint8_t _modem;// indicates FSK or LoRa modem.
	pack packet_sent;
	pack packet_received;
	pack ACK;
	int _temp;
	uint16_t _sendTime;
};

extern SX1278 sx1278;

#endif

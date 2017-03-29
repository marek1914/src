/* polling mode */

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

#define APP_ADDR    (uint32_t)0x08004000

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart3;

uint8_t buf[1024 * 8];
uint8_t slice_crc[256]; 
int8_t  path[128];

char *url = "http://123.56.196.100/device/updateFile";

void Error_Handler(void)
{
	while (1) {}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
	                              RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void MX_USART1_UART_Init(void)
{
	uart1.Instance = USART1;
	uart1.Init.BaudRate = 115200;
	uart1.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.Init.StopBits = UART_STOPBITS_1;
	uart1.Init.Parity = UART_PARITY_NONE;
	uart1.Init.Mode = UART_MODE_TX_RX;
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&uart1) != HAL_OK) {
		Error_Handler();
	}
}

void MX_USART3_UART_Init(void)
{
	uart3.Instance = USART3;
	uart3.Init.BaudRate = 115200;
	uart3.Init.WordLength = UART_WORDLENGTH_8B;
	uart3.Init.StopBits = UART_STOPBITS_1;
	uart3.Init.Parity = UART_PARITY_NONE;
	uart3.Init.Mode = UART_MODE_TX_RX;
	uart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&uart3) != HAL_OK) {
		Error_Handler();
	}
}

void MX_GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&uart3, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

// 2K alignment data
void flash_write2k(uint32_t addr, uint8_t* buf)
{
	uint32_t PAGEError = 0, data, tmp;
	FLASH_EraseInitTypeDef EraseInitStruct;
	
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  	EraseInitStruct.PageAddress = addr;
  	EraseInitStruct.NbPages     = 1; // 1k per page

	HAL_FLASH_Unlock();
	
	HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
	
	tmp = addr;
	while (addr < tmp+0x800){
		data = *(uint32_t*)buf;
    	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
    	addr += 4;
		buf += 4;
  }
	
	HAL_FLASH_Lock();
}

int httpaction(uint16_t * content)
{
	uint8_t tmp[2];
	uint8_t *s;
	uint16_t len;

	// max 30s
	HAL_UART_Receive(&uart1, tmp, 2, 30000);
	if (tmp[0] != '\r' || tmp[1] != '\n')
		return 0;

	s = buf;
	while (HAL_UART_Receive(&uart1, s, 1, 3000) == HAL_OK) {
		s++;
	}
	// string terminal
	*s = 0;

	printf("\r\n%s\r\n", buf);
	
	for (s = buf;; s = NULL) {
		s = strtok(s, "\r\n");
		if (!s) break;
		len = atoi(s+=20);
		s = strchr(s,',')+1;
		*content = atoi(s);
	}
	return len;
}

void httpread(void)
{
	uint8_t *s = buf;

	while (HAL_UART_Receive(&uart1, s, 1, 500) == HAL_OK) {
		s++;
	}
}

void at(char* at)
{
	uint8_t tmp;
	
	printf("%s", at);
	// Clear cache data
	HAL_UART_Receive(&uart1, &tmp, 1, 0);
	HAL_UART_Transmit(&uart1, at, strlen(at), 50);
}

int wait_status(void)
{
	uint8_t tmp[7] = {0};

	HAL_UART_Receive(&uart1, tmp, 6, 500);

	//printf("\r\n--%s++", tmp);
	if (!strcmp(tmp, "\r\nOK\r\n")) {
		printf("\r\nok\r\n\r\n");
		return 1;
	} else {
		HAL_UART_Receive(&uart1, tmp, 3, 500);
		printf("\r\nerror\r\n\r\n");
		return 0;
	}
}

int skip(int cnt)
{
	uint8_t tmp;
	int i;
	
	for (i = 0; i < cnt; i++)
		HAL_UART_Receive(&uart1, &tmp, 1, 500);
}

void gprs_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
	// need 7s+ ,print @@Ver:V2_029-OPEN-01 ...
	HAL_Delay(9000);
	//wait for @@Ver
	//do {
	//	HAL_UART_Receive(&uart1, &data, 1, 100);
	//} while(data != '@');
	at("ate0\r");
	//return ate0\r\r\nOK\r\n
	skip(5);
	wait_status();
}


unsigned char _crc8(unsigned short data)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (data & 0x8000) data = data ^ (0x1070U<<3);
		data = data << 1;
	}
	return (unsigned char)(data >> 8);
}

unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len)
{
	int i;

	for (i = 0; i < len; i++) crc = _crc8((crc ^ vptr[i]) << 8);
	return crc;
}

/*init para action read term*/
uint16_t download(char *file, uint16_t *content)
{
	uint16_t len;
	char num[5];
	
	//at("at+httpterm\r");
	//wait_status();
	at("at+httpinit\r");
	wait_status();

	strcpy(path, "at+httppara=1,\"");
	strcat(path, url);
	strcat(path, "/split/");
	strcat(path, file);
	strcat(path, "\"\r");
	at(path);
	wait_status();

	at("at+httpaction=0\r");
	wait_status();
	len = httpaction(content);
	
	strcpy(path, "at+httpread=0,");
	sprintf(num, "%d", len);
	strcat(path, num);
	strcat(path, "\r");
	at(path);
	httpread();
	//temp ugly!
	puts("");

	at("at+httpterm\r");
	wait_status();

	printf("------------\r\n");
	return len;
}

uint16_t parse_index(void)
{
	uint8_t *p;
	uint16_t count, i;
	uint16_t len, contentLen;
	uint8_t crc, crc1;
	
	len = download("index.txt", &contentLen);
	
	printf("buffer:\r\n%s\r\n", buf);
	
	p = buf + (len - contentLen);
	// no '\n' ok?
	crc = strtol(p += 4, NULL, 16);
	// 7byte :  crc:xx\n
	crc1 = crc8(0, p += 3, contentLen - 7);
	count = strtol(p += 6, NULL, 16);

	printf("count=%d crc=%x crc1=%x\n", count, crc, crc1);

	p += 3;

	char *s_crc;

// TODO: merge
	s_crc = strtok(p, "\n ");
	slice_crc[0] = strtol(s_crc, NULL, 16);

	for (i = 1; i < count; i++) {
		s_crc = strtok(NULL, "\n ");
		slice_crc[i] = strtol(s_crc, NULL, 16);
	}
	return count;
}

/*
void parse_data(uint16_t *packLen, uint16_t *contentLen)
{
	p = strtok(buf, "\r\n");

	if (strcmp(p, "HTTP/1.1 200 OK")) {
		printf("fail\n");
		return;
	} else {
		printf("head ok\n");
	}
}
*/

int main(void)
{
	uint8_t *p, crc;
	uint16_t len, content;
	uint32_t addr;
	uint16_t i, count;

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();
	
	gprs_init();
	
	count = parse_index();
	printf("count: %d\r\n", count);
	
	addr = APP_ADDR;
	for (i = 0; i < count; ) {
		char tmp[3];
		sprintf(tmp, "%02x", i);
		len = download(tmp, &content);

		p = buf + len - content;
		
		crc = crc8(0, p, content);
		printf("%d %d crc:%x %x\n", len, content, crc, slice_crc[i]);
		
		if (crc == slice_crc[i]) {
			int j;
			
			printf("crc OK\n");
			for (j = 0; j < content/0x800; j++) {
				flash_write2k(addr, p);
				addr += 0x800;
				p += 0x800;
				printf("addr=0x%x\n", addr);
			}
			i++;
		} else {
			printf("crc FAIL\n");
		}
	}
	
	addr = *(__IO uint32_t*)(APP_ADDR + 4);
	__set_MSP(*(__IO uint32_t*)APP_ADDR);
	((void(*)(void))addr)();

  while(1);
}


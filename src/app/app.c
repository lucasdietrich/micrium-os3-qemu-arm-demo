#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <osal.h>

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>

#define CONSOLE_UART_DEVICE USART1

static UART_HandleTypeDef huart;

static void Error_Handler(void);

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 0xFFFFu);

	return ch;
}

void app_init(void)
{
	HAL_Init();

	huart.Instance = CONSOLE_UART_DEVICE;

	huart.Init.BaudRate     = 115200u;
	huart.Init.WordLength   = UART_WORDLENGTH_8B;
	huart.Init.StopBits     = UART_STOPBITS_1;
	huart.Init.Parity       = UART_PARITY_NONE;
	huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart.Init.Mode         = UART_MODE_TX_RX;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart) != HAL_OK)
	{
		Error_Handler();
	}
}


static const char message[] = "%lu - Hello world from QEMU ARM stm32f405 running Micrium OS 3 !\n";
static char buf[0x100];

void app_task(void *p_arg)
{
	HAL_StatusTypeDef status;
	uint32_t i = 0u;

	for (;;) {
		int wn = snprintf(buf, sizeof(buf), message, i);
		status = HAL_UART_Transmit(&huart, (uint8_t *)buf, wn, 0xFFFFu);
		if (status != HAL_OK) {
			goto error;
		}

		i++;
		k_sleep(K_MSEC(1000u));
	}

error:
	Error_Handler();
	__builtin_unreachable();
}

static void Error_Handler(void)
{
	__disable_irq();

	for (;;) {
		
	}
}
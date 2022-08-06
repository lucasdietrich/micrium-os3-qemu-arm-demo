#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <os.h>

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>

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

	huart.Instance = USART1;

	huart.Init.BaudRate     = 115200;
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

static void Error_Handler(void)
{
	__disable_irq();

	for (;;) {
		
	}
}

void app_task(void *p_arg)
{
	for (;;)
	{

	}
}
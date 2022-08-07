#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <osal.h>

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>

#define CONSOLE_UART1_DEVICE USART1
#define CONSOLE_UART2_DEVICE USART2
#define CONSOLE_UART3_DEVICE USART3
#define CONSOLE_UART4_DEVICE UART4
#define CONSOLE_UART5_DEVICE UART5
#define CONSOLE_UART6_DEVICE USART6

static void Error_Handler(void);

// int fputc(int ch, FILE *f)
// {
// 	HAL_UART_Transmit(&huart[0u], (uint8_t *)&ch, 1, 0xFFFFu);

// 	return ch;
// }

static HAL_StatusTypeDef app_uart_init(UART_HandleTypeDef *huart,
				       USART_TypeDef *device)
{
	/* Initialize UART */
	huart->Instance = device;

	huart->Init.BaudRate     = 115200u;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_StatusTypeDef ret = HAL_UART_Init(huart);

	if (ret != HAL_OK) {
		Error_Handler();
	}

	return ret;
}

static HAL_StatusTypeDef app_uart_send(UART_HandleTypeDef *huart,
				       const char *buf,
				       size_t len)
{
	HAL_StatusTypeDef ret;

	ret = HAL_UART_Transmit(huart, (uint8_t *)buf, len, 0xFFFFu);

	return ret;
}

static UART_HandleTypeDef huart[6u];

void app_init(void)
{
	HAL_Init();

	app_uart_init(&huart[0u], CONSOLE_UART1_DEVICE);
	app_uart_init(&huart[1u], CONSOLE_UART2_DEVICE);
	app_uart_init(&huart[2u], CONSOLE_UART3_DEVICE);
	app_uart_init(&huart[3u], CONSOLE_UART4_DEVICE);
	app_uart_init(&huart[4u], CONSOLE_UART5_DEVICE);
	app_uart_init(&huart[5u], CONSOLE_UART6_DEVICE);
}

static const char message[] = "%lu - uart %p - Hello from QEMU ARM stm32f405 running Micrium OS 3 !\n";
static char buf[0x100];

void app_task(void *p_arg)
{
	for (uint32_t _i = 0u;;_i++) {
		int wn;

		for (uint8_t j = 0; j < 6u; j++) {
			wn = snprintf(buf, sizeof(buf), message, _i, huart[j].Instance);
			if (app_uart_send(&huart[j], buf, wn) != HAL_OK) {
				goto error;
			}
		}
		
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
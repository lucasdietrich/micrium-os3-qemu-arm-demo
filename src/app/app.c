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

#define UART1_DEVICE USART1
#define UART2_DEVICE USART2
#define UART3_DEVICE USART3
#define UART4_DEVICE UART4
#define UART5_DEVICE UART5
#define UART6_DEVICE USART6

#define UART_INIT(_device) { .Instance = _device }

static void Error_Handler(void);

// int fputc(int ch, FILE *f)
// {
// 	HAL_UART_Transmit(&huart[0u], (uint8_t *)&ch, 1, 0xFFFFu);

// 	return ch;
// }

static HAL_StatusTypeDef app_uart_init(UART_HandleTypeDef *huart)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	if (huart->Instance == NULL) {
		goto error;
	}

	/* Initialize UART */
	huart->Init.BaudRate = 115200u;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;

	ret = HAL_UART_Init(huart);

error:
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

static UART_HandleTypeDef huart[6u] = {
	UART_INIT(UART1_DEVICE),
	UART_INIT(UART2_DEVICE),
	UART_INIT(UART3_DEVICE),
	UART_INIT(UART4_DEVICE),
	UART_INIT(UART5_DEVICE),
	UART_INIT(UART6_DEVICE)
};

void app_init(void)
{
	HAL_Init();

	for (uint8_t i = 0; i < 6u; i++) {
		app_uart_init(&huart[i]);
	}
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
/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>
#include <stm32f4xx_ll_usart.h>

struct stm32_uart_serial_config
{
	USART_TypeDef *const uart;
};

#define STM32_UART_REGS_GET(_dev) \
	DEVICE_CONFIG_GET(_dev, struct stm32_uart_serial_config)->uart

#define STM32_UART_INIT(_name, _uart) \
	const struct device _name = { \
		.api = &api, \
		.data = &(UART_HandleTypeDef) { 0 }, \
		.config =  \
		&(struct stm32_uart_serial_config) { \
			.uart = _uart \
		} \
	}


static int init(const struct device *serial)
{
	UART_HandleTypeDef *const data =
		DEVICE_DATA_GET(serial, UART_HandleTypeDef);
	USART_TypeDef *const uart = STM32_UART_REGS_GET(serial);

	HAL_StatusTypeDef ret = HAL_ERROR;

	data->Instance = uart;

	/* Initialize UART */
	data->Init.BaudRate = 115200u;
	data->Init.WordLength = UART_WORDLENGTH_8B;
	data->Init.StopBits = UART_STOPBITS_1;
	data->Init.Parity = UART_PARITY_NONE;
	data->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	data->Init.Mode = UART_MODE_TX_RX;
	data->Init.OverSampling = UART_OVERSAMPLING_16;

	ret = HAL_UART_Init(data);

error:
	if (ret != HAL_OK) {
		Error_Handler();
	}
}

static void poll_out(const struct device *serial, unsigned char c)
{

}

static int poll_in(const struct device *serial, unsigned char *c)
{
	return 0u;
}

static const struct serial_api api = {
	.init = init,
	.poll_out = poll_out,
	.poll_in = poll_in
};

STM32_UART_INIT(stm32_uart1, USART1);
STM32_UART_INIT(stm32_uart2, USART2);
STM32_UART_INIT(stm32_uart3, USART3);
STM32_UART_INIT(stm32_uart4, UART4);
STM32_UART_INIT(stm32_uart5, UART5);
STM32_UART_INIT(stm32_uart6, USART6);
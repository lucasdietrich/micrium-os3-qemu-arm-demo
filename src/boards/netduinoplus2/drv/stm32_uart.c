/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>
#include <stm32f4xx_ll_usart.h>

#include <device.h>

#include <errno.h>

struct stm32_uart_serial_config
{
	USART_TypeDef *const uart;
};

#define STM32_UART_REGS_GET(_dev) \
	DEVICE_CONFIG_GET(_dev, struct stm32_uart_serial_config)->uart

#define STM32_UART_INIT(_name, _uart) \
	const struct device _name = { \
		.api = &api, \
		.data = &(USART_HandleTypeDef) { 0 }, \
		.config =  \
		&(struct stm32_uart_serial_config) { \
			.uart = _uart \
		} \
	}

static int convert_error(HAL_StatusTypeDef err)
{
	switch (err) {
	case HAL_OK:
		return 0;
	case HAL_ERROR:
		return -EIO;
	case HAL_BUSY:
		return -EBUSY;
	case HAL_TIMEOUT:
		return -ETIMEDOUT;
	default:
		return -EIO;
	}
}

static int init(const struct device *serial)
{
        UART_HandleTypeDef *const handle = serial->data;
        HAL_StatusTypeDef ret = HAL_ERROR;

	handle->Instance = ((struct stm32_uart_serial_config *)serial->config)->uart;

	/* Initialize UART */
	handle->Init.BaudRate = 115200u;
	handle->Init.WordLength = UART_WORDLENGTH_8B;
	handle->Init.StopBits = UART_STOPBITS_1;
	handle->Init.Parity = UART_PARITY_NONE;
	// handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	handle->Init.Mode = UART_MODE_TX_RX;
	// handle->Init.OverSampling = UART_OVERSAMPLING_16;

	ret = HAL_UART_Init(handle);

	return convert_error(ret);
}

static void poll_out(const struct device *serial, unsigned char c)
{
        UART_HandleTypeDef *const uart = serial->data;

	HAL_UART_Transmit(uart, &c, 1u, 0u);
}

static int poll_in(const struct device *serial, unsigned char *c)
{
        UART_HandleTypeDef *const uart = serial->data;

	HAL_StatusTypeDef ret = HAL_UART_Receive(uart, c, 1u, 0u);

	return convert_error(ret);
}

static const struct serial_api api = {
	.init = init,
	.poll_out = poll_out,
	.poll_in = poll_in
};

STM32_UART_INIT(stm32_usart1, USART1);
STM32_UART_INIT(stm32_usart2, USART2);
STM32_UART_INIT(stm32_usart3, USART3);
STM32_UART_INIT(stm32_uart4, UART4);
STM32_UART_INIT(stm32_uart5, UART5);
STM32_UART_INIT(stm32_usart6, USART6);
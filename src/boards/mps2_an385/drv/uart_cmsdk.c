/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

#include <CMSDK_CM3.h>

#define UART_BAUDRATE 115200u

/*
 * MPS2 UART Driver - cmsdk_apb_uart :
 *
 * - Zephyr RTOS: 
 *   - https://github.com/zephyrproject-rtos/zephyr/blob/main/drivers/serial/uart_cmsdk_apb.c
 *   - DTS: https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/mps2_an385/mps2_an385.dts
 * - QEMU driver emulation: https://github.com/qemu/qemu/blob/master/hw/char/cmsdk-apb-uart.c
 * 
 * Read technical reference manual ARM DDI 0479D, section cmsdk_apb_uart
 */

struct uart_cmsdk_serial_config
{
	CMSDK_UART_TypeDef *const uart;
};

#define CMSDK_UART_REGS_GET(_dev) \
	DEVICE_CONFIG_GET(_dev, struct uart_cmsdk_serial_config)->uart

#define CMSDK_UART_INIT(_name, _uart) \
	const struct device _name = { \
		.api = &api, \
		.data = NULL, \
		.config =  \
		&(struct uart_cmsdk_serial_config) { \
			.uart = _uart \
		} \
	}

static void init(const struct device *serial)
{
	CMSDK_UART_TypeDef *const uart = CMSDK_UART_REGS_GET(serial);

	/* assumed FCPU = PCLK = 25MHz */
	uart->BAUDDIV = (FCPU / UART_BAUDRATE);

	/* Enable RX and TX */
	uart->CTRL = CMSDK_UART_CTRL_RXEN_Msk | CMSDK_UART_CTRL_TXEN_Msk;
}

static void poll_out(const struct device *serial, unsigned char c)
{
	CMSDK_UART_TypeDef *const uart = CMSDK_UART_REGS_GET(serial);

	while (uart->STATE & CMSDK_UART_STATE_TXBF_Msk) {
		/* Wait for TX FIFO to be empty */
	}

	uart->DATA = c;
}

static int poll_in(const struct device *serial, unsigned char *c)
{
	CMSDK_UART_TypeDef *const uart = CMSDK_UART_REGS_GET(serial);

	if (!(uart->STATE & CMSDK_UART_STATE_RXBF_Msk)) {
		/* No data available */
		return -1;
	}

	/* Read the character in the buffer */
	*c = uart->DATA;

	return 0u;
}

static const struct serial_api api = {
	.init = init,
	.poll_out = poll_out,
	.poll_in = poll_in
};

CMSDK_UART_INIT(cmsdk_uart0, CMSDK_UART0);
CMSDK_UART_INIT(cmsdk_uart1, CMSDK_UART1);
CMSDK_UART_INIT(cmsdk_uart2, CMSDK_UART2);
CMSDK_UART_INIT(cmsdk_uart3, CMSDK_UART3);
CMSDK_UART_INIT(cmsdk_uart4, CMSDK_UART4);
/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

#include <LM3S6965.h>
#include <core_cm3.h>

#define UART_BAUDRATE 115200u

#define FR_BUSY (1u << 3u)
#define FR_RXFE (1u << 4u)
#define FR_TXFF (1u << 5u)
#define FR_RXFF (1u << 6u)
#define FR_TXFE (1u << 7u)

#define CTL_EN (1u << 0u)
#define CTL_SIREN (1u << 1u)

#define LCRH_FEN (1u << 4u)
#define LCRH_WLEN_5 (0u << 5u)
#define LCRH_WLEN_6 (1u << 5u)
#define LCRH_WLEN_7 (2u << 5u)
#define LCRH_WLEN_8 (3u << 5u)

struct stellaris_uart_serial_config
{
	UART0_Type *const uart;
};

#define STELLARIS_UART_REGS_GET(_dev) \
	DEVICE_CONFIG_GET(_dev, struct stellaris_uart_serial_config)->uart

#define STELLARIS_UART_DEFINE(_name, _uart) \
	const struct device _name = { \
		.api = &api, \
		.data = NULL, \
		.config =  \
		&(struct stellaris_uart_serial_config) { \
			.uart = _uart \
		} \
	}

static int init(const struct device *serial)
{
	/* Stellaris® LM3S6965 Microcontroller page 453 */

	UART0_Type *const uart = STELLARIS_UART_REGS_GET(serial);

	/* 1. Disable the UART. */
	uart->CTL &= ~CTL_EN;

	/* 2. Wait for the end of transmission or reception
	 * of the current character. */
	while (uart->FR & FR_BUSY) {
	}

	/* 3. Flush the transmit FIFO by disabling bit 4 (FEN) in
	 * the line control register (UARTLCRH). */
	uart->LCRH &= ~LCRH_FEN;

	/* Set baudrate to default value */
	const uint32_t baud_div = (UART_BAUDRATE * 16u);
	const uint32_t baud_rem = FCPU % baud_div;

	const uint32_t int_part = FCPU / baud_div;
	const uint32_t frac_part = (baud_rem * 64u + 0.5); /* page 435 */

	/* Write registers */
	uart->IBRD = int_part & 0xFFFFu;
	uart->FBRD = frac_part & 0x3Fu;

	/* Configure UART
	 * - Stick parity disabled
	 * - 8bits UART word length
	 * - fifos disabled
	 * - two stop bits disabled
	 * - even parity disabled
	 * - no parity checking
	 * - send break disabled
	 */
	uart->LCRH = LCRH_WLEN_8;

	/* Enable UART */
	uart->CTL |= CTL_EN;

	return 0u;
}

static void poll_out(const struct device *serial, unsigned char c)
{
	UART0_Type *const uart = STELLARIS_UART_REGS_GET(serial);

	/* Wait for the transmit FIFO to be empty */
	while (!(uart->FR & FR_TXFE)) {
	}

	uart->DR = c;
}

static int poll_in(const struct device *serial, unsigned char *c)
{
	UART0_Type *const uart = STELLARIS_UART_REGS_GET(serial);

	if (uart->FR & FR_RXFE) {
		return -1;
	}

	*c = uart->DR;
	
	return 0u;
}

static const struct serial_api api = {
	.init = init,
	.poll_out = poll_out,
	.poll_in = poll_in
};

STELLARIS_UART_DEFINE(stellaris_uart0, UART0);
STELLARIS_UART_DEFINE(stellaris_uart1, UART1);
STELLARIS_UART_DEFINE(stellaris_uart2, UART2);
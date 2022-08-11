/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

void serial_init(const struct device *serial)
{
	SERIAL_API_GET(serial)->init(serial);
}

void serial_poll_out(const struct device *serial, unsigned char c)
{
	SERIAL_API_GET(serial)->poll_out(serial, c);
}

int serial_poll_in(const struct device *serial, unsigned char *c)
{
	return SERIAL_API_GET(serial)->poll_in(serial, c);
}
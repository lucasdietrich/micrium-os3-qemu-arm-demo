/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

int serial_init(const struct device *serial)
{
	return SERIAL_API_GET(serial)->init(serial);
}

void serial_poll_out(const struct device *serial, unsigned char c)
{
	SERIAL_API_GET(serial)->poll_out(serial, c);
}

int serial_poll_in(const struct device *serial, unsigned char *c)
{
	return SERIAL_API_GET(serial)->poll_in(serial, c);
}

int timer_set_callback(const struct device *timer,
		       timer_callback_t callback,
		       void *user_data)
{
	return TIMER_API_GET(timer)->set_callback(timer, callback, user_data);
}

int timer_start(const struct device *timer, uint32_t ticks)
{
	return TIMER_API_GET(timer)->start(timer, ticks);
}

int timer_stop(const struct device *timer)
{
	return TIMER_API_GET(timer)->stop(timer);
}
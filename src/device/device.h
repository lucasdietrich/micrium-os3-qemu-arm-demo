/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdio.h>
#include <stddef.h>

#include <common.h>

struct device
{
	void *const data;
	const void *config;
	const void *api;
};

#define DEVICE_DATA_GET(_device, _type) \
	((_type *) (_device)->data)

#define DEVICE_CONFIG_GET(_device, _type) \
	((_type *) (_device)->config)

#define DEVICE_API_GET(_device, _type) \
	((const _type *) (_device)->api)
	

struct serial_api
{
	int (*init)(const struct device *serial);
	void (*poll_out)(const struct device *serial, unsigned char c);
	int (*poll_in)(const struct device *serial, unsigned char *c);
};

#define SERIAL_API_GET(_device) \
	DEVICE_API_GET(_device, struct serial_api)

int serial_init(const struct device *serial);

void serial_poll_out(const struct device *serial, unsigned char c);

int serial_poll_in(const struct device *serial, unsigned char *c);

typedef void (*timer_callback_t)(const struct device *timer,
				       void *user_data);

struct timer_api
{
	int (*set_callback)(const struct device *timer,
			    timer_callback_t callback,
			    void *user_data);
	int (*start)(const struct device *timer, uint32_t ticks);
	int (*stop)(const struct device *timer);
};

#define TIMER_API_GET(_device) \
	DEVICE_API_GET(_device, struct timer_api)

int timer_set_callback(const struct device *timer,
		       timer_callback_t callback,
		       void *user_data);

int timer_start(const struct device *timer, uint32_t ticks);

int timer_stop(const struct device *timer);


#endif /* _DEVICE_H_ */
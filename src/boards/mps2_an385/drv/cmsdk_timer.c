/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>

#include <CMSDK_CM3.h>

#include <errno.h>

/*
 * MPS2 UART Driver - cmsdk_apb_timer :
 *
 * - Zephyr RTOS:
 *   - https://github.com/zephyrproject-rtos/zephyr/blob/main/drivers/counter/timer_tmr_cmsdk_apb.c
 *   - DTS: https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/mps2_an385/mps2_an385.dts
 * - QEMU driver emulation: https://github.com/qemu/qemu/blob/master/hw/timer/cmsdk-apb-timer.c
 *
 * Read technical reference manual ARM DDI 0479D, section cmsdk_apb_timer
 */

#define TIMER_INT_ENABLE		(1 << 0)
#define TIMER_EXT_CLOCK			(1 << 1)
#define TIMER_EXT_ENABLE		(1 << 2)
#define TIMER_ENABLE			(1 << 3)

#define TIMER_ISR_PRIORITY		3u

struct cmsdk_timer
{
	CMSDK_TIMER_TypeDef *timer;
	IRQn_Type irq_n;

	timer_callback_t callback;
	void *user_data;
};

#define CMSDK_TIMER_REGS_GET(_dev) \
	DEVICE_CONFIG_GET(_dev, struct cmsdk_timer)->timer

static inline void timer_handler(const struct device *timer)
{
	struct cmsdk_timer *config =
		DEVICE_CONFIG_GET(timer, struct cmsdk_timer);
	config->timer->INTCLEAR = 1;
	config->callback(timer, config->user_data);
}

#define CMSDK_TIMER_ISR_DEFINE(_device_ptr, _nvic_isr) \
void _nvic_isr(void) { timer_handler(_device_ptr); }

#define CMSDK_TIMER_DEFINE(_name, _timer, _nvic_isr_n, _nvic_isr) \
	const struct device _name = { \
		.api = &cmsdk_timer_api, \
		.data = NULL, \
		.config =  \
		&(struct cmsdk_timer) { \
			.timer = _timer, \
			.irq_n = _nvic_isr_n, \
			.callback = NULL, \
			.user_data = NULL \
		} \
	}; \
	CMSDK_TIMER_ISR_DEFINE(&_name, _nvic_isr)

static int set_callback(const struct device *timer,
			timer_callback_t callback,
			void *user_data)
{
	int ret = -EINVAL;
	struct cmsdk_timer *const config =
		DEVICE_CONFIG_GET(timer, struct cmsdk_timer);

	if (callback != NULL) {
		config->callback = callback;
		config->user_data = user_data;
		ret = 0;
	}

	return ret;
}

static int start(const struct device *timer, uint32_t ticks)
{
	struct cmsdk_timer *const config =
		DEVICE_CONFIG_GET(timer, struct cmsdk_timer);

	if (config->callback == NULL) {
		return -EINVAL;
	}

	CMSDK_TIMER_TypeDef *const regs = config->timer;

	/* Reset interrupt config and status */
	regs->CTRL = 0;
	regs->INTCLEAR = 1u;

	__NVIC_ClearPendingIRQ(config->irq_n);
	__NVIC_SetPriority(config->irq_n, TIMER_ISR_PRIORITY);
	__NVIC_EnableIRQ(config->irq_n);

	/* Configure timer */
	regs->VALUE = ticks;
	regs->RELOAD = ticks;

	/* Start timer */
	regs->CTRL = TIMER_INT_ENABLE | TIMER_ENABLE;

	return 0;
}

static int stop(const struct device *timer)
{
	CMSDK_TIMER_TypeDef *regs = CMSDK_TIMER_REGS_GET(timer);

	regs->INTCLEAR |= 1u;
	regs->CTRL = 0;

	return 0;
}

static const struct timer_api cmsdk_timer_api = {
	.set_callback = set_callback,
	.start = start,
	.stop = stop,
};

CMSDK_TIMER_DEFINE(cmsdk_timer0, CMSDK_TIMER0, TIMER0_IRQn, TIMER0_Handler);
CMSDK_TIMER_DEFINE(cmsdk_timer1, CMSDK_TIMER1, TIMER1_IRQn, TIMER1_Handler);
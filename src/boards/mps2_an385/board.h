/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MPS2_AN385_BOARD_H_
#define _MPS2_AN385_BOARD_H_

#include <device.h>
#include <CMSDK_CM3.h>

extern const struct device cmsdk_uart0;
extern const struct device cmsdk_uart1;
extern const struct device cmsdk_uart2;
extern const struct device cmsdk_uart3;
extern const struct device cmsdk_uart4;

#define serial_console (&cmsdk_uart0)
#define serial_log (&cmsdk_uart1)

#endif /* _MPS2_AN385_BOARD_H_ */
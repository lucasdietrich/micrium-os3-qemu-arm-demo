/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _APP_NET_H_
#define _APP_NET_H_

#include <os.h>

#include <Source/sntp-c_type.h>

int app_net_init(void);

CPU_BOOLEAN App_UDP_Client(CPU_CHAR *p_ip_addr);

void  App_TCP_ServerIPv4(void);

CPU_BOOLEAN App_SNTPc_SetClk (SNTPc_CFG *p_sntp_cfg);

#endif /* _APP_INIT_H_ */
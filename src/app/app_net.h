#ifndef _APP_NET_H_
#define _APP_NET_H_

#include <os.h>

int app_net_init(void);

CPU_BOOLEAN App_UDP_Client(CPU_CHAR *p_ip_addr);

void  App_TCP_ServerIPv4(void);

#endif /* _APP_INIT_H_ */
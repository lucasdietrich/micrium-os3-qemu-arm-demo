#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <board.h>
#include <osal.h>
#include <app_net.h>

#include <logging.h>
LOG_MODULE_REGISTER(app_net, LOG_LEVEL_DBG);


#if defined(CONFIG_NETWORKING)
#define NETWORKING_UDP_CLIENT
// #define NETWORKING_TCP_SERVER
#endif

#if defined(serial_console) && defined(serial_log)

int _write(int fd, char *buf, int count)
{
	for (; count != 0; --count) {
		serial_poll_out(serial_console, *buf++);
	}
	return count;
}

void timer0_app_handler(const struct device *dev, void *user_data)
{
	// serial_poll_out(serial_console, '!');
}

#define TCP_SERVER_TASK_PRIORITY 10
void tcpsrv(void *arg);
OS_TCB  tcpsrv_thread;
CPU_STK tcpsrv_stack[0x400u];

void app_init(void)
{
#if defined(CONFIG_NETWORKING)
	/* Networking stack must be initialized after OS has beenstarted */
	app_net_init();
#endif

	serial_init(serial_console);
	serial_init(serial_log);

	timer_set_callback(&cmsdk_timer0, timer0_app_handler, NULL);
	timer_start(&cmsdk_timer0, 25000000lu);

#if defined(NETWORKING_TCP_SERVER)
	OS_ERR uce;
	OSTaskCreate(&tcpsrv_thread, "sys_main", tcpsrv, (void *)0, TCP_SERVER_TASK_PRIORITY,
		     &tcpsrv_stack[0], 10, sizeof(tcpsrv_stack) / 4u, 5, 10,
		     (void *)0, OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR, &uce);
#endif
}

void app_task(void *p_arg)
{
	LOG_INF("App task starting");

	unsigned char c;
	for (;;) {
		if (serial_poll_in(serial_console, &c) == 0) {
			serial_poll_out(serial_console, c);
			serial_poll_out(serial_log, c);
		}
		
		k_sleep(K_MSEC(100u));

#if defined(NETWORKING_UDP_CLIENT)
		App_UDP_Client("192.168.10.216");

		k_sleep(K_SECONDS(5));
#endif
	}
}

void tcpsrv(void *arg)
{
	ARG_UNUSED(arg);

	for (;;) {
		App_TCP_ServerIPv4();
	}
}
#endif
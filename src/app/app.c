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

#if defined(CONFIG_NETWORKING)
		App_UDP_Client("192.168.10.216");

		k_sleep(K_SECONDS(5));
#endif
	}
}

#endif
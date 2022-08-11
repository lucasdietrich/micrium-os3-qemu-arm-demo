#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <bsp.h>
#include <board.h>

#include <osal.h>

#if defined(serial_console) && defined(serial_log)

void app_init(void)
{
	serial_init(serial_console);
	serial_init(serial_log);
}

void app_task(void *p_arg)
{
	unsigned char c;
	for (;;) {
		if (serial_poll_in(serial_console, &c) == 0) {
			serial_poll_out(serial_console, c);
			serial_poll_out(serial_log, c);
		}
		k_sleep(K_MSEC(100u));
	}
}

#endif
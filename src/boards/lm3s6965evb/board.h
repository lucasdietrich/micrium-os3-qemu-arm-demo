#ifndef _LM3S6965EVB_BOARD_H_
#define _LM3S6965EVB_BOARD_H_

#include <LM3S6965.h>
#include <core_cm3.h>

#include <device.h>

extern const struct device stellaris_uart0;
extern const struct device stellaris_uart1;
extern const struct device stellaris_uart2;

#define serial_console (&stellaris_uart0)
#define serial_log (&stellaris_uart1)

#endif /* _BOARD_H_ */
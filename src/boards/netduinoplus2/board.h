#ifndef _NETDUINOPLUS2_BOARD_H_
#define _NETDUINOPLUS2_BOARD_H_

#include <stm32f405xx.h>

extern const struct device stm32_usart1;
extern const struct device stm32_usart2;
extern const struct device stm32_usart3;
// extern const struct device stm32_uart4;
// extern const struct device stm32_uart5;
extern const struct device stm32_usart6;

#define serial_console (&stm32_usart1)
#define serial_log (&stm32_usart2)

#endif /* _NETDUINOPLUS2_BOARD_H_ */
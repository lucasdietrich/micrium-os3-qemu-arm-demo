/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * 
 * Notes:
 *   - after including this header, define LOG_LEVEL to one of this file constant : LOG_LEVEL_*
 * Example
 * 	#include <logging.h>
 * 	LOG_MODULE_REGISTER(app_net, LOG_LEVEL_DBG);
 *
 *   - use with miniterm : python3 -m serial.tools.miniterm --raw /dev/ttyACM0 921600
 */

#ifndef _APP_LOGGING_H
#define _APP_LOGGING_H

#include <stddef.h>
#include <stdbool.h>

// for colors : https://xdevs.com/guide/color_serial/
#define CONFIG_LOG_COLOR_ENABLED 1

#define LOG_COLOR_RESET_ALL "\033[m"

#define LOG_COLOR_BLACK_ON_WHITE "\033[0;30;41m"
#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_YELLOW "\033[0;33m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_MAGENTA "\033[0;35m"
#define LOG_COLOR_CYAN "\033[0;36m"
#define LOG_COLOR_WHITE "\033[0;37m"
#define LOG_COLOR_DEFAULT "\033[0;39m"

#define LOG_COLOR_GRAY_ON_WHITE "\033[1;30m"
#define LOG_COLOR_BRIGHT_RED "\033[1;31m"
#define LOG_COLOR_BRIGHT_GREEN "\033[1;32m"
#define LOG_COLOR_BRIGHT_YELLOW "\033[1;33m"
#define LOG_COLOR_BRIGHT_BLUE "\033[1;34m"
#define LOG_COLOR_BRIGHT_MAGENTA "\033[1;35m"
#define LOG_COLOR_BRIGHT_CYAN "\033[1;36m"
#define LOG_COLOR_BRIGHT_WHITE "\033[1;37m"
#define LOG_COLOR_BRIGHT_DEFAULT "\033[0;39m"

#define LOG_COLOR_BG_BLACK "\033[0;40m"
#define LOG_COLOR_BG_RED "\033[0;41m"
#define LOG_COLOR_BG_GREEN "\033[0;42m"
#define LOG_COLOR_BG_YELLOW "\033[0;43m"
#define LOG_COLOR_BG_BLUE "\033[0;44m"
#define LOG_COLOR_BG_MAGENTA "\033[0;45m"
#define LOG_COLOR_BG_CYAN "\033[0;46m"
#define LOG_COLOR_BG_WHITE "\033[0;47m"
#define LOG_COLOR_BG_RESET "\033[0;49m"

#define LOG_COLOR_WRN LOG_COLOR_BRIGHT_YELLOW
#define LOG_COLOR_ERR LOG_COLOR_BRIGHT_RED
#define LOG_COLOR_RESET LOG_COLOR_RESET_ALL


#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WRN 2
#define LOG_LEVEL_INF 3
#define LOG_LEVEL_DBG 4
#define LOG_LEVEL_VERBOSE 5
#define LOG_LEVEL_ALL LOG_LEVEL_VERBOSE

struct log_module {
	const char *name;
	int level;
	bool color;
};

struct log_line
{
	const struct log_module *module;
	int level;
	const char *func;
	size_t line;
};

#define LOG_MODULE_REGISTER(_name, _level) \
	static const struct log_module _log_module = \
		{ .name = #_name, .level = _level, .color = CONFIG_LOG_COLOR_ENABLED } \

int _log_print(const struct log_line *log,
	       const char *__restrict__ fmt,
	       ...);

#define LOG(_level, _fmt, ...) \
	if ((_level) <= (_log_module.level)) { \
		_log_print(&((const struct log_line) { \
			&_log_module, _level, __func__, __LINE__}), \
			_fmt, ## __VA_ARGS__); \
	}

#define LOG_VRB(fmt, ...) LOG(LOG_LEVEL_VERBOSE, fmt, ## __VA_ARGS__)
#define LOG_DBG(fmt, ...) LOG(LOG_LEVEL_DBG, fmt, ## __VA_ARGS__)
#define LOG_INF(fmt, ...) LOG(LOG_LEVEL_INF, fmt, ## __VA_ARGS__)
#define LOG_WRN(fmt, ...) LOG(LOG_LEVEL_WRN, fmt, ## __VA_ARGS__)
#define LOG_ERR(fmt, ...) LOG(LOG_LEVEL_ERR, fmt, ## __VA_ARGS__)

/*
#define LOG_HEXDUMP(level, data, len) \
   do { \
	if ((level) <= (LOG_LEVEL)) { \
		for(unsigned int i = 0; i < (len); i++) { \
			_log_printf(false, "%02x ", data[i]); \
		} \
		_log_printf(false, "\n"); \
	} \
   } while (0)

#define LOG_HEXDUMP_WITHSTART(level, data, len, _start, _end) \
   do { \
	if ((level) <= (LOG_LEVEL)) { \
		_log_printf(false, _start); \
		for(unsigned int i = 0; i < (len); i++) { \
			_log_printf(false, "%02x ", data[i]); \
		} \
		_log_printf(false, _end); \
	} \
   } while (0)
*/


#endif /* _APP_LOGGING_H */
/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include <logging.h>

/* https://github.com/lucasdietrich/LoRaMac-node/blob/lucas/playground/src/apps/playground/periodic-uplink-lpp/B-L072Z-LRWAN1-playground/logging.h */

int _log_print(const struct log_line *log,
	       const char *__restrict__ fmt,
	       ...)
{
	va_list args;
	va_start(args, fmt);

	if (log->level > log->module->level) {
		return -1;
	}

	const char *color_prefix = "";
	const char *level_prefix = "";
	if (log->module->color) {

		switch (log->level) {
		case LOG_LEVEL_ERR:
			color_prefix = LOG_COLOR_ERR;
			level_prefix = "err";
			break;
		case LOG_LEVEL_WRN:
			level_prefix = "wrn";
			color_prefix = LOG_COLOR_WRN;
			break;
		case LOG_LEVEL_INF:
			level_prefix = "inf";
			break;
		case LOG_LEVEL_DBG:
			level_prefix = "dbg";
			break;
		case LOG_LEVEL_VERBOSE:
			level_prefix = "vrb";
			break;
		default:
			break;
		}

		printf(color_prefix);
	}

	/* Write prefix */
	printf("%s<%s %s", color_prefix, level_prefix,  log->module->name);

	if (log->level == LOG_LEVEL_DBG) {
		printf(".%s", log->func);
	}

	printf("> ");

	/* Write log message */
	vprintf(fmt, args);

	/* Write suffix */
	if (log->module->color) {
		printf(LOG_COLOR_RESET "\n");
	} else {
		printf("\n");
	}

	va_end(args);

	return 0;
}
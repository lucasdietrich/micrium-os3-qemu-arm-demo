/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stm32f405xx.h>

#include <os.h>
#include <lib_mem.h>

#include <app.h>

#define OS_TICKS_PER_SEC OS_CFG_TICK_RATE_HZ

#define SYS_MAIN_TASK_STACK_SIZE 	0x400
#define SYS_MAIN_TASK_PRIORITY 		0

static OS_TCB  sys_main_thread;
static CPU_STK sys_main_stack[SYS_MAIN_TASK_STACK_SIZE];

void sys_task(void *p_arg)
{
	/* Configure systick once */
	if ((SysTick->CTRL & SysTick_CTRL_TICKINT_Msk) == 0u) {
		OS_CPU_SysTickInitFreq(FCPU);
	}

	app_init();

	app_task(p_arg);

	for (;;) {

	}
}

#define MISC_TASK_COUNT 	2u
#define MISC_TASK_STACK_SIZE 	0x100u
#define MISC_TASK_PRIORITY	12u
#define MISC_TASK_DELAY_SEC	5u

#if defined(MISC_TASK_COUNT) && (MISC_TASK_COUNT > 0u)

static OS_TCB  misk_tasks[MISC_TASK_COUNT];
static CPU_STK misk_task_stacks[MISC_TASK_COUNT][MISC_TASK_STACK_SIZE];

void misc_task(void *p_arg)
{
	OS_ERR uce;
	while (1u) {
		OSTimeDly(MISC_TASK_DELAY_SEC * OS_TICKS_PER_SEC, 
			  OS_OPT_TIME_DLY, &uce);
	}
}

#endif

int main(void)
{
	OS_ERR uce;

	CPU_Init();
	Mem_Init();

	OSInit(&uce);

#if defined(MISC_TASK_COUNT) && (MISC_TASK_COUNT > 0u)
	for (size_t i = 0; i < MISC_TASK_COUNT; i++) {
		OSTaskCreate(&misk_tasks[i],                    	/*  (1) Address of TCB assigned to the task      */
			     "task1",                                	/*  (2) Name you want to give the task           */
			     misc_task,					/*  (3) Address of the task itself               */
			     (void *)0,                              	/*  (4) "p_arg" is not used                      */
			     MISC_TASK_PRIORITY,			/*  (5) Priority you want to assign to the task  */
			     misk_task_stacks[i],                       /*  (6) Base address of task's stack             */
			     10,                                        /*  (7) Watermark limit for stack growth         */
			     MISC_TASK_STACK_SIZE / 4u,                 /*  (8) Stack size in number of CPU_STK elements */
			     5,                                         /*  (9) Size of task message queue               */
			     10,                                        /* (10) Time quanta (in number of ticks)         */
			     (void *)0,                                 /* (11) Extension pointer is not used            */
			     OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR, /* (12) Options               */
			     &uce);                                     /* (13) Error code            */
	}
#endif /* MISC_TASK_COUNT */
	
	OSTaskCreate(&sys_main_thread, "sys_main", sys_task, (void *)0, SYS_MAIN_TASK_PRIORITY,
		     &sys_main_stack[0], 10, sizeof(sys_main_stack) / 4u, 5, 10,
		     (void *)0, OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR, &uce);

	OSStart(&uce);

	__builtin_unreachable();
}
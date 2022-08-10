#ifndef _OSAL_H_
#define _OSAL_H_

#include <board.h>

/* OS Abstraction Layer */

#include <stdint.h>
#include <os.h>

#define K_MSEC(_ms) ((_ms) * OS_CFG_TICK_RATE_HZ / 1000u)
#define K_SECONDS(_sec) ((_sec) * OS_CFG_TICK_RATE_HZ)

static inline void k_sleep(uint32_t ticks)
{
	OS_ERR uce;
	OSTimeDly((CPU_INT32U)ticks, OS_OPT_TIME_DLY, &uce);
}

static inline void k_yield(void)
{
	k_sleep(1u);
}

static inline void k_sched_lock(void)
{
	OS_ERR uce;
	OSSchedLock(&uce);
}

static inline  void k_sched_unlock(void)
{
	OS_ERR uce;
	OSSchedUnlock(&uce);
}

static inline void Error_Handler(void)
{
	__disable_irq();

	for (;;) {

	}
}

#endif // _OSAL_H_
/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stm32f405xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_cortex.h>

#include <device.h>

#include <errno.h>

void MemManage_Handler(void)
{
	__asm volatile("BKPT #0");
}

int stm32_mpu_init(void *addr)
{
	MPU_Region_InitTypeDef reg;
	
	HAL_MPU_Disable();

	reg.Enable = MPU_REGION_ENABLE;
	reg.Number = MPU_REGION_NUMBER0;
	reg.BaseAddress = (uint32_t) addr;
	reg.Size = MPU_REGION_SIZE_64KB;
	reg.SubRegionDisable = 0x00u;
	reg.TypeExtField = MPU_TEX_LEVEL0;
	reg.IsCacheable = MPU_ACCESS_CACHEABLE;
	reg.IsBufferable = MPU_ACCESS_BUFFERABLE;
	reg.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	reg.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	reg.AccessPermission = MPU_REGION_NO_ACCESS;

	HAL_MPU_ConfigRegion(&reg);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

	return 0;
}
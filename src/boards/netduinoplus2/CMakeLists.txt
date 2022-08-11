set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/STM32F405RGTx_FLASH.ld" PARENT_SCOPE)
set(CPU "cortex-m4" PARENT_SCOPE)
set(target "${PROJECT_NAME}.elf" PARENT_SCOPE)
set(QEMU_MACHINE "netduinoplus2" PARENT_SCOPE)

set(BOARD_DEFINES
	"-DNETDUINOPLUS2"
	"-DCPU_CFG_NVIC_PRIO_BITS=4"
	"-DARMCM3"
	"-DSTM32F405xx"
	"-DFCPU=16000000ul"
)
target_compile_definitions(${target} PRIVATE ${BOARD_DEFINES})

file(GLOB BOARD_CFG_SOURCES ${CMAKE_CURRENT_LIST_DIR}/cfg/*.c)
target_sources(${target} PRIVATE ${BOARD_CFG_SOURCES})

set(BOARD_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/sys/startup_stm32f405xx.s
	${CMAKE_CURRENT_LIST_DIR}/sys/system_stm32f4xx.c

	${CMAKE_CURRENT_LIST_DIR}/cfg/net_cfg.c
	${CMAKE_CURRENT_LIST_DIR}/cfg/net_dev_cfg.c
)
target_sources(${target} PRIVATE ${BOARD_SOURCES})

file(GLOB BOARD_DEVICES_SOURCES ${CMAKE_CURRENT_LIST_DIR}/dev/*.c)
target_sources(${target} PRIVATE ${BOARD_DEVICES_SOURCES})

set(STM32_HAL_SRC_PATH ${CMAKE_SOURCE_DIR}/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src)
set(STM32_HAL_DRIVERS_SOURCES
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_adc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_adc_ex.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_can.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_cec.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal_cortex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_crc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_cryp.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_cryp_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dac.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dac_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dcmi.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dcmi_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dfsdm.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dma2d.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dma.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dma_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_dsi.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_eth.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_exti.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_flash.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_flash_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_flash_ramfunc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_fmpi2c.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_fmpi2c_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_fmpsmbus.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_fmpsmbus_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_gpio.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_hash.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_hash_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_hcd.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_i2c.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_i2c_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_i2s.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_i2s_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_irda.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_iwdg.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_lptim.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_ltdc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_ltdc_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_mmc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_msp_template.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_nand.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_nor.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_pccard.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_pcd.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_pcd_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_pwr.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_pwr_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_qspi.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal_rcc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_rcc_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_rng.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_rtc.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_rtc_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_sai.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_sai_ex.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_sd.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_sdram.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_smartcard.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_smbus.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_spdifrx.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_spi.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_sram.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal_tim.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_timebase_rtc_alarm_template.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_timebase_rtc_wakeup_template.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_timebase_tim_template.c
	# ${STM32_HAL_SRC_PATH}/stm32f4xx_hal_tim_ex.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal_uart.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_hal_usart.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_adc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_crc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_dac.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_dma2d.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_dma.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_exti.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_fmc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_fmpi2c.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_fsmc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_gpio.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_i2c.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_lptim.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_pwr.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_rcc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_rng.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_rtc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_sdmmc.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_spi.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_tim.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_usart.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_usb.c
	${STM32_HAL_SRC_PATH}/stm32f4xx_ll_utils.c
)
target_sources(${target} PRIVATE ${STM32_HAL_DRIVERS_SOURCES})

set(C_BOARD_INCLUDES
	# Cortex-M4 headers
	${CMAKE_SOURCE_DIR}/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
	${CMAKE_SOURCE_DIR}/STM32CubeF4/Drivers/CMSIS/Include

	# Drivers headers
	${CMAKE_SOURCE_DIR}/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc

	# Application drivers configuration
	${CMAKE_CURRENT_SOURCE_DIR}/sys/
	${CMAKE_CURRENT_SOURCE_DIR}/
)
target_include_directories(${target} PRIVATE ${C_BOARD_INCLUDES})

add_subdirectory(drv)
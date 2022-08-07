## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

cmake_minimum_required(VERSION 3.2)

project(ucos3-qemu-app)

set(SZ ${CMAKE_SIZE_UTIL})
set(OBJCOPY ${CMAKE_OBJCOPY_UTIL})
set(OBJDUMP ${CMAKE_OBJDUMP_UTIL})
set(NM ${CMAKE_NM_UTIL})
set(READELF ${CMAKE_READELF_UTIL})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_language(C ASM)

# Check that the target is statically linked (i.e. "-rdynamic" not set)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_C_FLAGS_DEBUG "-gdwarf-4 -g3")


set(ELF_PATH "${CMAKE_BINARY_DIR}/${target}")

# linker script copied from STM32CubeMX generated project
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F405RGTx_FLASH.ld")
set(CPU "cortex-m4")
set(target "${PROJECT_NAME}.elf")
set(QEMU_MACHINE "netduinoplus2")

set(C_SOURCES
	src/sys/startup_stm32f405xx.s
	src/sys/system_stm32f4xx.c

    src/os/os_app_hooks.c
	src/os/net_cfg.c
	src/os/net_dev_cfg.c
	src/entry.c
)

file(GLOB C_APP_SOURCES src/app/*.c)

set(C_INCLUDES
	# Cortex-M4 headers
	STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
	STM32CubeF4/Drivers/CMSIS/Include

	# Drivers headers
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc

	# Application drivers configuration
	src/drv

	# Application os configuration
	src/os

	# Application headers
	src/app
    src
)

set(UCOS3_SOURCES_ARCH
    uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S
	uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c

	uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s
	uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c

	# uC-LIB/Ports/ARM-Cortex-M3/GNU/lib_mem_a.s
)

set(UCOS3_SOURCES_KAL "uC-Common/KAL/uCOS-III/kal.c")

set(UCOS3_SOURCES_KERNEL
	# uC-OS3/Source/__dbg_uCOS-III.c

	uC-OS3/Source/os_cfg_app.c
	uC-OS3/Source/os_core.c
	uC-OS3/Source/os_dbg.c
	uC-OS3/Source/os_flag.c
	uC-OS3/Source/os_mem.c
	uC-OS3/Source/os_msg.c
	uC-OS3/Source/os_mutex.c
	uC-OS3/Source/os_prio.c
	uC-OS3/Source/os_q.c
	uC-OS3/Source/os_sem.c
	uC-OS3/Source/os_stat.c
	uC-OS3/Source/os_task.c
	uC-OS3/Source/os_tick.c
	uC-OS3/Source/os_time.c
	uC-OS3/Source/os_tmr.c
	uC-OS3/Source/os_var.c

	uC-CPU/cpu_core.c
)

file(GLOB UCOS3_SOURCES_TCPIP_BASE uC-TCP-IP/Source/*.c)
file(GLOB UCOS3_SOURCES_TCPIP_IPV4 uC-TCP-IP/IP/IPv4/*.c)
file(GLOB UCOS3_SOURCES_TCPIP_IPV6 uC-TCP-IP/IP/IPv6/*.c)
file(GLOB UCOS3_SOURCES_TCPIP_IF uC-TCP-IP/IF/*.c)
set(UCOS3_SOURCES_TCPIP ${UCOS3_SOURCES_TCPIP_BASE} ${UCOS3_SOURCES_TCPIP_IPV4} ${UCOS3_SOURCES_TCPIP_IPV6} ${UCOS3_SOURCES_TCPIP_IF})


set(STM32_HAL_DRIVERS_SOURCES
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cec.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dfsdm.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dsi.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpsmbus.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpsmbus_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_lptim.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_mmc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp_template.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_qspi.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai_ex.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smbus.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spdifrx.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c
	# STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_crc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dac.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma2d.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmpi2c.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_lptim.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_pwr.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rng.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
	STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c
)

# file(GLOB STM32_LL_SOURCES STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_*.c)

file(GLOB UCOS3_SOURCES_LIB uC-LIB/*.c)

set(UCOS3_INCLUDES
    uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU
    uC-OS3/Source
	uC-OS3

	uC-CPU/ARM-Cortex-M/ARMv7-M/GNU
	uC-CPU
	uC-LIB
	uC-TCP-IP
	uC-Common
)

set(ARCH_DEFINES
	"-DCPU_CFG_NVIC_PRIO_BITS=4"
	"-DARMCM7_SP"
	"-DSTM32F405xx"
	"-DFCPU=16000000ul"
)

add_executable(${target} ${C_SOURCES} ${C_APP_SOURCES} ${UCOS3_SOURCES_ARCH} ${UCOS3_SOURCES_KERNEL} ${UCOS3_SOURCES_LIB} ${STM32_HAL_DRIVERS_SOURCES} ${UCOS3_SOURCES_TCPIP} ${UCOS3_SOURCES_KAL})

target_include_directories(${target} PRIVATE ${C_INCLUDES} ${UCOS3_INCLUDES})

target_compile_options(${target} PRIVATE
	-mcpu=${CPU}
	-mthumb

	-fdata-sections
	-ffunction-sections

	-Wall
)

target_compile_definitions(${target} PRIVATE ${ARCH_DEFINES})

target_link_options(${target} PRIVATE
	-mcpu=${CPU}
	-mthumb

	-T${LINKER_SCRIPT}

	-lc
	-lm
	-lnosys
	
	# already defined in the toolchain
	# -specs=nano.specs
	-Wl,-Map=${PROJECT_NAME}.map,--cref
	-Wl,--gc-sections
	-Wl,--print-memory-usage
)



# Print size of executable
add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${SZ} -G -d ${target} > ${PROJECT_NAME}.size.txt
)

# Convert to hex & bin
add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${OBJCOPY} -O ihex ${target} ${PROJECT_NAME}.hex
        COMMAND ${OBJCOPY} -O binary -S ${target} ${PROJECT_NAME}.bin)

if(DEFINED DO_DISASSEMBLY AND DO_DISASSEMBLY STREQUAL 1) 
	# disassembly
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${OBJDUMP} -S ${target} > objdump_src.s VERBATIM
		COMMAND ${OBJDUMP} -d ${target} > objdump.s VERBATIM
		COMMAND ${OBJDUMP} -D ${target} > objdump_all.s VERBATIM
		COMMAND ${OBJDUMP} -h ${target} > objdump_sections.s VERBATIM
		COMMAND ${READELF} -a ${target} > readelf.txt VERBATIM
		COMMAND ${NM} --print-size --size-sort --radix=x ${target} > nm.txt VERBATIM
		DEPENDS ${target}
	)
endif()

# generate custom target to debug in qemu
add_custom_target(qemu
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=0,align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon,mux=on -mon chardev=myCon,mode=readline -serial chardev:myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -s -S 
)

# generate custom command to run only in qemu
add_custom_target(run
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=0,align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon,mux=on -mon chardev=myCon,mode=readline -serial chardev:myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile

)

# Two followin configuration are equivalent
# -nodefaults -chardev stdio,id=myCon,mux=on -serial chardev:myCon
# -nodefaults -chardev stdio,id=con,mux=on -serial chardev:con


# -chardev file,id=mon,path=/home/lucas/ucos3-qemu-app/io.txt -serial mon:stdio

# Check these options 
# -kernel ${target}
# -nodefaults 
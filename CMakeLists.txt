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

set(ELF_PATH "${CMAKE_BINARY_DIR}/${target}")

# linker script is a adapted copy of CMSIS_5/Device/ARM/ARMCM7/Source/GCC/gcc_arm.ld
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ld.ld")
set(CPU "cortex-m3")
set(target "${PROJECT_NAME}.elf")
set(QEMU_MACHINE "lm3s6965evb")

set(C_SOURCES
	# CMSIS_5/Device/ARM/ARMCM7/Source/GCC/startup_ARMCM7.S
	# CMSIS_5/Device/ARM/ARMCM7/Source/startup_ARMCM7.c
	CMSIS_5/Device/ARM/ARMCM7/Source/system_ARMCM7.c
	
	src/startup_ARMCM7.c
	src/main.c
    src/os/os_app_hooks.c
)

set(C_INCLUDES
	CMSIS_5/Device/ARM/ARMCM7/Include
	CMSIS_5/CMSIS/Core/Include

    src
    src/os
)

set(UCOS3_SOURCES_ARCH
    uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S
	uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c

	uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s
	uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c

	# uC-LIB/Ports/ARM-Cortex-M3/GNU/lib_mem_a.s
)

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

file(GLOB UCOS3_SOURCES_LIB uC-LIB/*.c)

set(UCOS3_INCLUDES
    uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU
    uC-OS3/Source

	uC-CPU/ARM-Cortex-M/ARMv7-M/GNU
	uC-CPU
	uC-LIB
)

set(UC_DEFINES
	"-DCPU_CFG_NVIC_PRIO_BITS=4"
	"-DARMCM7"
)

add_executable(${target} ${C_SOURCES} ${UCOS3_SOURCES_ARCH} ${UCOS3_SOURCES_KERNEL} ${UCOS3_SOURCES_LIB})

target_include_directories(${target} PRIVATE ${C_INCLUDES} ${UCOS3_INCLUDES})

target_compile_options(${target} PRIVATE
	-mcpu=${CPU}
	-mthumb

	-fdata-sections
	-ffunction-sections

	-Wall
)

target_compile_definitions(${target} PRIVATE ${UC_DEFINES})

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

# generate custom target for debug in qemu
add_custom_target(qemu
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -bios ${target} -s -S -nographic -singlestep
	DEPENDS ${target}
)
## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

cmake_minimum_required(VERSION 3.2)

project(ucos3-qemu-app)

# https://github.com/zephyrproject-rtos/zephyr/blob/main/cmake/modules/extensions.cmake#L1487-L1497
function(add_subdirectory_ifdef feature_toggle source_dir)
  if(${${feature_toggle}})
    add_subdirectory(${source_dir} ${ARGN})
  endif()
endfunction()

function(target_sources_ifdef feature_toggle target scope item)
  if(${${feature_toggle}})
    target_sources(${target} ${scope} ${item} ${ARGN})
  endif()
endfunction()

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

set(CMAKE_C_FLAGS_DEBUG "-gdwarf-4 -g3 -Og")
message(STATUS CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG})

set(ELF_PATH "${CMAKE_BINARY_DIR}/${target}")
set(target "${PROJECT_NAME}.elf")

set(ICOUNT_SHIFT 0)
set(QEMU_NET_TAP_MODEL_ARG "")

################################################################################
# Create EXE
add_executable(${target})

if (NOT DEFINED BOARD)
	message(STATUS "Board not defined, selecting default board 'netduinoplus2'")
	set(BOARD "mps2_an385")
endif()

################################################################################
# Include board CMakeLists.txt and general CMakeLists.txt

message(STATUS "Board: ${BOARD}")
if (BOARD STREQUAL "netduinoplus2")
	add_subdirectory(src/boards/netduinoplus2)
elseif(BOARD STREQUAL "lm3s6965evb")
	add_subdirectory(src/boards/lm3s6965evb)
elseif(BOARD STREQUAL "mps2_an385")
	add_subdirectory(src/boards/mps2_an385)
else()
	message(FATAL_ERROR "Unknown board '${BOARD}'")
endif()

add_subdirectory(src/app)
add_subdirectory(src/device)
add_subdirectory(src/subsys)

################################################################################
# OS source and includes

set(C_SOURCES
    src/os/os_app_hooks.c
	src/entry.c
)

set(OS_ARCH_SOURCES
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c
)

set(OS_CPU_ARCH_SOURCES
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c
)

set(OS_CPU_CORE_SOURCES
	${CMAKE_SOURCE_DIR}/uC-CPU/cpu_core.c
)

set(OS_KAL_SOURCES "uC-Common/KAL/uCOS-III/kal.c")

set(OS_KERNEL_SOURCES
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
)

# TCP/IP stack
if (DEFINED CONFIG_NETWORKING AND CONFIG_NETWORKING STREQUAL 1)
	file(GLOB OS_TCPIP_CORE_SOURCES uC-TCP-IP/Source/*.c)
	file(GLOB OS_TCPIP_IPV4_SOURCES uC-TCP-IP/IP/IPv4/*.c)
	file(GLOB OS_TCPIP_IPV6_SOURCES uC-TCP-IP/IP/IPv6/*.c)
	file(GLOB OS_TCPIP_IF_SOURCES 	uC-TCP-IP/IF/*.c)
	file(GLOB OS_SNTPC_SOURCES 		uC-SNTPc/Source/*.c)
	file(GLOB OS_DNSC_SOURCES 		uC-DNSc/Source/*.c)
	set(OS_TCPIP_SOURCES 
		${OS_TCPIP_CORE_SOURCES} 
		${OS_TCPIP_IPV4_SOURCES} 
		${OS_TCPIP_IPV6_SOURCES} 
		${OS_TCPIP_IF_SOURCES} 
		${OS_SNTPC_SOURCES} 
		${OS_DNSC_SOURCES}
	)
	set(OS_TCPIP_INCLUDES
		uC-TCP-IP
		uC-SNTPc
		uC-DNSc
	)
	target_compile_definitions(${target} PRIVATE -DCONFIG_NETWORKING)
endif()

# FS
if (DEFINED CONFIG_FS AND CONFIG_FS STREQUAL 1)
	file (GLOB OS_FS_SOURCES uC-FS/Source/*.c)
	file (GLOB OS_FS_FAT_SOURCES uC-FS/FAT/*.c)
	set(OS_FS_OS_SOURCES uC-FS/OS/uCOS-III/fs_os.c)
	set(OS_FS_RAMDISK_SOURCES uC-FS/Dev/RAMDisk/fs_dev_ramdisk.c)
	set(OS_FS_SOURCES
		${OS_FS_SOURCES} 
		${OS_FS_FAT_SOURCES} 
		${OS_FS_OS_SOURCES} 
		${OS_TCPIP_IF_SOURCES} 
		${OS_FS_RAMDISK_SOURCES}
	)
	set(OS_FS_INCLUDES
		uC-FS
		uC-FS/OS/uCOS-III
		uC-FS/Dev/RAMDisk
	)
	target_compile_definitions(${target} PRIVATE -DCONFIG_FS)
endif()

set(OS_CLK_SOURCES uC-Clk/OS/uCOS-III/clk_os.c uC-Clk/Source/clk.c)

file(GLOB OS_LIB_SOURCES uC-LIB/*.c)

target_sources(${target} PRIVATE 
	${C_SOURCES} 
	${OS_ARCH_SOURCES} 
	${OS_CPU_ARCH_SOURCES}
	${OS_CPU_CORE_SOURCES}
	${OS_KAL_SOURCES} 
	${OS_KERNEL_SOURCES} 
	${OS_TCPIP_SOURCES} 
	${OS_FS_SOURCES}
	${OS_CLK_SOURCES}
	${OS_LIB_SOURCES}
)

set(OS_INCLUDES
    uC-OS3/Source
	uC-OS3

	uC-CPU
	uC-LIB
	uC-Common
	uC-Clk

    uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU
	uC-CPU/ARM-Cortex-M/ARMv7-M/GNU
)

set(C_INCLUDES
	# Application os configuration
	src/os

	# BSP headers
	src/boards

	# Application headers
    src
)
target_include_directories(${target} PRIVATE 
	${C_INCLUDES}
	${OS_INCLUDES}
	${OS_TCPIP_INCLUDES}
	${OS_FS_INCLUDES}
)

message(STATUS "Compiling ${target} CPU=${CPU}")

target_compile_options(${target} PRIVATE
	-mcpu=${CPU}
	-mthumb

	-fdata-sections
	-ffunction-sections

	-Wall

	# Complaints too much without this
	-Wno-maybe-uninitialized
)

target_compile_definitions(${target} PRIVATE ${ARCH_DEFINES})

target_link_options(${target} PRIVATE
	-mcpu=${CPU}
	-mthumb

	-T${LINKER_SCRIPT}

	-lc
	-lm
	# -nostdlib
	# -lnosys
	# -specs=nano.specs
	
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

# If CONFIG_NETWORKING is set, define QEMU_NET_ARGS
if(DEFINED CONFIG_NETWORKING AND CONFIG_NETWORKING STREQUAL 1)
	if (QEMU_NET_TAP_MODEL_ARG STREQUAL "")
		message(FATAL_ERROR "QEMU_NET_TAP_MODEL_ARG is not set")
	endif()
	set(QEMU_NET_ARGS "-nic tap,model=${QEMU_NET_TAP_MODEL_ARG},script=no,downscript=no,ifname=meth")
else()
	set(QEMU_NET_ARGS "")
endif()

# Tips
separate_arguments(QEMU_NET_ARGS)

# generate custom target to debug in qemu
add_custom_target(qemu
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=${ICOUNT_SHIFT},align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile ${QEMU_NET_ARGS} -s -S
)

# generate custom command to run only in qemu
add_custom_target(run
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=${ICOUNT_SHIFT},align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile ${QEMU_NET_ARGS}
)

# Show help for debug:
# -d trace:help
# -d help
# -d cpu_reset

# Enable debug in lan9118 
# goto `hw/net/lan9118.c` and set DEBUG_LAN9118 

# Two followin configuration are equivalent
# -nodefaults -chardev stdio,id=myCon,mux=on -serial chardev:myCon
# -nodefaults -chardev stdio,id=con,mux=on -serial chardev:con


# -chardev file,id=mon,path=/home/lucas/ucos3-qemu-app/io.txt -serial mon:stdio

# Check these options 
# -kernel ${target}
# -nodefaults 
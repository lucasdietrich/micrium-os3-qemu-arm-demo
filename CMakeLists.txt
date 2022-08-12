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
set(QEMU_ADD_ARGS "")

add_executable(${target})

if (NOT DEFINED BOARD)
	message(STATUS "Board not defined, selecting default board 'netduinoplus2'")
	set(BOARD "netduinoplus2")
endif()

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

# add_subdirectory(src/boards/${BOARD})

add_subdirectory(src/app)
add_subdirectory(src/device)
add_subdirectory(src/middlewares)

set(C_SOURCES
    src/os/os_app_hooks.c
	src/entry.c
)

set(UCOS3_ARCH_SOURCES
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c

	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c
)

set(UCOS3_SOURCES_KAL "uC-Common/KAL/uCOS-III/kal.c")

set(UCOS3_KERNEL_SOURCES
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

file(GLOB UCOS3_LIB_SOURCES uC-LIB/*.c)

set(C_UCOS3_INCLUDES
    uC-OS3/Source
	uC-OS3

	uC-CPU
	uC-LIB
	uC-TCP-IP
	uC-Common

    ${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU
)

target_sources(${target} PRIVATE ${C_SOURCES} ${UCOS3_KERNEL_SOURCES} ${UCOS3_LIB_SOURCES} ${UCOS3_SOURCES_TCPIP} ${UCOS3_SOURCES_KAL} ${UCOS3_ARCH_SOURCES})

set(C_INCLUDES
	# Application os configuration
	src/os

	# BSP headers
	src/boards

	# Application headers
    src
)
target_include_directories(${target} PRIVATE ${C_INCLUDES} ${C_UCOS3_INCLUDES})

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

separate_arguments(QEMU_ADD_ARGS)

# generate custom target to debug in qemu
add_custom_target(qemu
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=${ICOUNT_SHIFT},align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile ${QEMU_ADD_ARGS} -s -S
)

# generate custom command to run only in qemu
add_custom_target(run
	COMMAND qemu-system-arm -cpu ${CPU} -machine ${QEMU_MACHINE} -nodefaults -nographic -semihosting -icount shift=${ICOUNT_SHIFT},align=off,sleep=on -rtc clock=vm -vga none -nographic -device loader,file=${target} -pidfile qemu.pid -chardev stdio,id=myCon -serial chardev:myCon -chardev file,id=myFile,mux=on,path=io.txt -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile -serial chardev:myFile ${QEMU_ADD_ARGS}
)

# Two followin configuration are equivalent
# -nodefaults -chardev stdio,id=myCon,mux=on -serial chardev:myCon
# -nodefaults -chardev stdio,id=con,mux=on -serial chardev:con


# -chardev file,id=mon,path=/home/lucas/ucos3-qemu-app/io.txt -serial mon:stdio

# Check these options 
# -kernel ${target}
# -nodefaults 
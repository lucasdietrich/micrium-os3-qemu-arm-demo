set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/gcc_arm.ld" PARENT_SCOPE)
set(CPU "cortex-m3" PARENT_SCOPE)
set(target "${PROJECT_NAME}.elf" PARENT_SCOPE)
set(QEMU_MACHINE "mps2-an385" PARENT_SCOPE)
set(ICOUNT_SHIFT 5 PARENT_SCOPE)

set(BOARD_DEFINES
	"-DMPS2_AN385"
	"-DCPU_CFG_NVIC_PRIO_BITS=3"
	"-DARMCM3"
	"-DFCPU=25000000ul"
)
target_compile_definitions(${target} PRIVATE ${BOARD_DEFINES})

file(GLOB BOARD_CFG_SOURCES ${CMAKE_CURRENT_LIST_DIR}/cfg/*.c)
target_sources(${target} PRIVATE ${BOARD_CFG_SOURCES})

set(BOARD_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/sys/startup_ARMCM3.S
	# ${CMAKE_CURRENT_LIST_DIR}/sys/startup_ARMCM3.c
	${CMAKE_CURRENT_LIST_DIR}/sys/system_ARMCM3.c

	${CMAKE_CURRENT_LIST_DIR}/cfg/net_cfg.c
	${CMAKE_CURRENT_LIST_DIR}/cfg/net_dev_cfg.c
)
target_sources(${target} PRIVATE ${BOARD_SOURCES})

file(GLOB BOARD_DEVICES_SOURCES ${CMAKE_CURRENT_LIST_DIR}/dev/*.c)
target_sources(${target} PRIVATE ${BOARD_DEVICES_SOURCES})

set(UCOS3_ARCH_SOURCES
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S
	${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c

	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c
)
target_sources(${target} PRIVATE ${UCOS3_ARCH_SOURCES})

set(C_BOARD_INCLUDES
	# Cortex-M3 headers
	${CMAKE_SOURCE_DIR}/CMSIS_5/Device/ARM/ARMCM3/Include
	${CMAKE_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include

	# Application drivers configuration
	${CMAKE_CURRENT_LIST_DIR}/drv
	${CMAKE_CURRENT_LIST_DIR}/cfg
	${CMAKE_CURRENT_LIST_DIR}/sys
	${CMAKE_CURRENT_LIST_DIR}/dev
	${CMAKE_CURRENT_LIST_DIR}/.

	# Rest of specific Micrium headers
    ${CMAKE_SOURCE_DIR}/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/GNU
	${CMAKE_SOURCE_DIR}/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU
)
target_include_directories(${target} PRIVATE ${C_BOARD_INCLUDES})
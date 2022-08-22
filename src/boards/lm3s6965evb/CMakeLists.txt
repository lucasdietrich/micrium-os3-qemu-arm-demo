set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/gcc_arm.ld" PARENT_SCOPE)
set(CPU "cortex-m3" PARENT_SCOPE)
set(QEMU_MACHINE "lm3s6965evb" PARENT_SCOPE)
# set(QEMU_MACHINE "lm3s811evb" PARENT_SCOPE)
set(ICOUNT_SHIFT 5 PARENT_SCOPE)
set(QEMU_NET_TAP_MODEL_ARG "stellaris" PARENT_SCOPE)

set(BOARD_DEFINES
	"-DLM3S6965EVB"
	"-DCPU_CFG_NVIC_PRIO_BITS=4"
	"-DARMCM3"
	"-DSTM32F405xx"
	"-DFCPU=12000000ul"
)

# uC-TCP-IP/Dev/Ether/LM3Sxxxx/net_dev_lm3sxxxx.c

target_compile_definitions(${target} PRIVATE ${BOARD_DEFINES})

set(BOARD_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/sys/startup_ARMCM3.S
	# ${CMAKE_CURRENT_LIST_DIR}/sys/startup_ARMCM3.c
	${CMAKE_CURRENT_LIST_DIR}/sys/system_ARMCM3.c
)
target_sources(${target} PRIVATE ${BOARD_SOURCES})

set(C_BOARD_INCLUDES
	# Cortex-M3 headers
	${CMAKE_SOURCE_DIR}/CMSIS_5/Device/ARM/ARMCM3/Include
	${CMAKE_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include

	${CMAKE_CURRENT_SOURCE_DIR}/sys/
	${CMAKE_CURRENT_SOURCE_DIR}/
)
target_include_directories(${target} PRIVATE ${C_BOARD_INCLUDES})

add_subdirectory(drv)
add_subdirectory(cfg)
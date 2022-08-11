set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/MPS2.ld" PARENT_SCOPE)
set(CPU "cortex-m3" PARENT_SCOPE)
set(target "${PROJECT_NAME}.elf" PARENT_SCOPE)
set(QEMU_MACHINE "mps2-an385" PARENT_SCOPE)
set(ICOUNT_SHIFT 4 PARENT_SCOPE)

set(BOARD_DEFINES
	"-DMPS2_AN385"
	"-DCPU_CFG_NVIC_PRIO_BITS=3"
	"-DARMCM3"
	"-DFCPU=25000000ul"
)
target_compile_definitions(${target} PRIVATE ${BOARD_DEFINES})

set(BOARD_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/sys/startup_MPS2.S
	${CMAKE_CURRENT_LIST_DIR}/sys/system_CMSDK_CM3.c
)
target_sources(${target} PRIVATE ${BOARD_SOURCES})

set(C_BOARD_INCLUDES
	# Cortex-M3 headers
	# ${CMAKE_SOURCE_DIR}/CMSIS_5/Device/ARM/ARMCM3/Include
	${CMAKE_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include

	${CMAKE_CURRENT_SOURCE_DIR}/sys/
	${CMAKE_CURRENT_SOURCE_DIR}/
)
target_include_directories(${target} PRIVATE ${C_BOARD_INCLUDES})

add_subdirectory(drv)
add_subdirectory(cfg)
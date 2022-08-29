## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

# TCP/IP stack
if (DEFINED CONFIG_NETWORKING AND CONFIG_NETWORKING STREQUAL 1)
	set(BOARD_NET_CFG_SOURCES 
		${CMAKE_CURRENT_LIST_DIR}/dns-c_cfg.c
        ${CMAKE_CURRENT_LIST_DIR}/net_cfg.c
        ${CMAKE_CURRENT_LIST_DIR}/net_dev_cfg.c
        ${CMAKE_CURRENT_LIST_DIR}/sntp-c_cfg.c
	)
    target_sources(${target} PRIVATE ${BOARD_NET_CFG_SOURCES})
endif()

target_include_directories(${target} PRIVATE .)
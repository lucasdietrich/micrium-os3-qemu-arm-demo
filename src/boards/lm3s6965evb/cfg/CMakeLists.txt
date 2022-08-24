## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 


target_sources_ifdef(CONFIG_NETWORKING ${target} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/src/boards/lm3s6965evb/cfg/dns-c_cfg.c
    ${CMAKE_CURRENT_LIST_DIR}/src/boards/lm3s6965evb/cfg/net_cfg.c
    ${CMAKE_CURRENT_LIST_DIR}/src/boards/lm3s6965evb/cfg/net_dev_cfg.c
    ${CMAKE_CURRENT_LIST_DIR}/src/boards/lm3s6965evb/cfg/sntp-c_cfg.c
)

target_include_directories(${target} PRIVATE .)
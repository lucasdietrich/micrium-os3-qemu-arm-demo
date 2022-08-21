## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

set(app_sources
    ${CMAKE_CURRENT_LIST_DIR}/app.c
    ${CMAKE_CURRENT_LIST_DIR}/fs/fs_app.c
)

target_sources(${target} PRIVATE ${app_sources})

target_sources_ifdef(CONFIG_NETWORKING ${target} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/app_net.c 
    ${CMAKE_CURRENT_LIST_DIR}/app_sntp.c 
    ${CMAKE_CURRENT_LIST_DIR}/udp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/udp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/tcp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/tcp_server.c
)

target_include_directories(${target} PRIVATE .)
target_include_directories(${target} PRIVATE fs)
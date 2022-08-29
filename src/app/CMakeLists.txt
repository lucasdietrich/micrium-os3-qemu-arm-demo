## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 


file(GLOB app_sources *.c)
target_sources(${target} PRIVATE ${app_sources})

target_include_directories(${target} PRIVATE .)

add_subdirectory_ifdef(CONFIG_FS ${CMAKE_CURRENT_LIST_DIR}/fs)
add_subdirectory_ifdef(CONFIG_NETWORKING ${CMAKE_CURRENT_LIST_DIR}/net)
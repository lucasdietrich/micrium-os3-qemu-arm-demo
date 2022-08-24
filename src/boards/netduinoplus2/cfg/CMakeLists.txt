## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

file(GLOB BOARD_CONFIG_SOURCES *.c)
target_sources(${target} PRIVATE ${BOARD_CONFIG_SOURCES})

target_include_directories(${target} PRIVATE .)
## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

file(GLOB APP_FS_SOURCES *.c)
target_sources(${target} PRIVATE ${APP_FS_SOURCES})

target_include_directories(${target} PRIVATE .)
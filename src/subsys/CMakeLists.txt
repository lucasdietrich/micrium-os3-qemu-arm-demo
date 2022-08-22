## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

file(GLOB subsystem_sources *.c)

target_sources(${target} PRIVATE ${subsystem_sources})
target_include_directories(${target} PRIVATE .)
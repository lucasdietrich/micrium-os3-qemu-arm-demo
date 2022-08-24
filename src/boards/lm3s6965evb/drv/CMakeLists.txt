## 
## Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
## 
## SPDX-License-Identifier: Apache-2.0
## 

file(GLOB DRIVERS_SOURCES *.c)
target_sources(${target} PRIVATE ${DRIVERS_SOURCES})
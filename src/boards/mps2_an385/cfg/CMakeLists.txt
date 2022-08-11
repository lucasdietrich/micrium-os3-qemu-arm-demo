file(GLOB BOARD_CONFIG_SOURCES *.c)
target_sources(${target} PRIVATE ${BOARD_CONFIG_SOURCES})

target_include_directories(${target} PRIVATE .)
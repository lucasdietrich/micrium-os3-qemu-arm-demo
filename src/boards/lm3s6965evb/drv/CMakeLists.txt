file(GLOB DRIVERS_SOURCES *.c)
target_sources(${target} PRIVATE ${DRIVERS_SOURCES})
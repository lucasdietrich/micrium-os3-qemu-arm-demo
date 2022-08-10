file(GLOB app_sources *.c)

target_sources(${target} PRIVATE ${app_sources})
target_include_directories(${target} PRIVATE .)
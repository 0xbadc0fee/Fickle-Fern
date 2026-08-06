add_subdirectory(libs/bios)

add_executable(boot_core
    src/main.c
)
set_target_properties(boot_core PROPERTIES SUFFIX ".elf")

target_include_directories(boot_core PRIVATE
    libs/bios
    src
)

target_compile_options(boot_core PRIVATE
    -mcpu=tc29xx -g2 -Os -std=gnu99 -fno-common -ffunction-sections -fdata-sections -Wall -Wdouble-promotion
)

target_link_libraries(boot_core PRIVATE
    esx_4cm_a_c_bios_boot_core
)

target_link_options(boot_core PRIVATE
    -mcpu=tc29xx
    -nocrt0
    -T "${CMAKE_CURRENT_SOURCE_DIR}/libs/bios/link_c_layout_2.ld"
    -Wl,--gc-sections
    -Wl,--mem-holes
    -Wl,--extmap=a
    -Wl,--cref
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/boot_core.map
    -Wl,-XML-Map=${CMAKE_CURRENT_BINARY_DIR}/boot_core.xml
)

esx_add_hex_output(boot_core)

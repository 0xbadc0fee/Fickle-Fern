add_subdirectory(libs/bios)
add_subdirectory(libs/opensyde_server)

add_executable(safety_core
    src/main.c
    src/opensyde/generated/osy_init.c
    src/opensyde/osy_app.c
    src/opensyde/osy_srv.c
)
set_target_properties(safety_core PROPERTIES SUFFIX ".elf")

target_include_directories(safety_core PRIVATE
    libs/bios
    libs/opensyde_server
    src
    src/opensyde
    src/opensyde/generated
)

target_compile_options(safety_core PRIVATE
    -mcpu=tc29xx -g2 -Os -std=gnu99 -fno-common -ffunction-sections -fdata-sections -Wall -Wdouble-promotion
    -fno-if-conversion
)

target_link_libraries(safety_core PRIVATE
    safety_core_opensyde_server_api
    esx_4cm_a_c_bios_safety_core
)

target_link_options(safety_core PRIVATE
    -mcpu=tc29xx
    -nocrt0
    -T "${CMAKE_CURRENT_SOURCE_DIR}/libs/bios/link_c_layout_2.ld"
    -Wl,--gc-sections
    -Wl,--mem-holes
    -Wl,--extmap=a
    -Wl,--cref
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/safety_core.map
    -Wl,-XML-Map=${CMAKE_CURRENT_BINARY_DIR}/safety_core.xml
)

esx_add_hex_output(safety_core)

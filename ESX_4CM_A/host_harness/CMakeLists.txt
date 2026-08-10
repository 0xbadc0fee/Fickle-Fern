cmake_minimum_required(VERSION 3.20)

# Host-DLL build variant of appl_core (Silent-Sagebrush harness target, -DSVG_HARNESS).
# Compiles a DELIBERATELY NARROW subset of the SAME appl_core source the TriCore build
# uses (see ../appl_core/CMakeLists.txt) with a native MinGW-w64 compiler instead,
# producing a host x86-64 .dll for logic-only Python-driven testing. See CLAUDE.md at
# the repo root for the full design rationale and hard scope constraints.
#
# THIS IS A SEPARATE CMAKE PROJECT, configured and built independently of the
# TriCore build - never add this directory via add_subdirectory() from
# ../CMakeLists.txt, and never point this project's build directory at the same
# location as the TriCore build's.
#
# Usage:
#   cmake -S ESX_4CM_A/host_harness -B build-harness -G Ninja -DCMAKE_TOOLCHAIN_FILE=ESX_4CM_A/cmake/host-toolchain.cmake
#   cmake --build build-harness
#
# File list here is intentionally a subset of appl_core's full source list - see
# handoff_phase1.md's "Recommended next steps" for the file-by-file inclusion
# decisions and why several files (alarm_handler_lib.c, nvm_handler_lib.c,
# can_handler_lib.c, and everything outside this HAL/IO cluster) are still
# deliberately excluded rather than forgotten.

project(svg_harness C)

add_library(svg_harness SHARED
    ../appl_core/src/System/IO/hw_outputs.c
    ../appl_core/src/System/IO/hw_inputs.c
    ../appl_core/src/HAL/output_handler_lib.c
    ../appl_core/src/HAL/input_handler_lib.c
    ../appl_core/src/Testing/harness_bios_stubs.c
    ../appl_core/src/Testing/harness_can_devs.c
    ../appl_core/src/Testing/harness_api.c
)

set_target_properties(svg_harness PROPERTIES PREFIX "")

target_compile_definitions(svg_harness PRIVATE SVG_HARNESS)

# Same include-path set as ../appl_core/CMakeLists.txt's appl_core target (verified
# clean for this file subset across every probe to date) - kept identical rather than
# hand-pruned so files can move between the TriCore and host targets without either
# side's include list needing separate upkeep.
target_include_directories(svg_harness PRIVATE
    ../appl_core/libs/bios
    ../appl_core/src/AgvChassis
    ../appl_core/libs/opensyde_j1939_handler
    ../appl_core/libs/lwip/include
    ../appl_core/libs/opensyde_server
    ../appl_core/src/opensyde
    ../appl_core/src/opensyde/j1939
    ../appl_core/src/opensyde/generated
    ../appl_core/src
    ../appl_core/src/HAL
    ../appl_core/src/HAL/STW_4CM_HAL
    ../appl_core/src/System
    ../appl_core/src/System/IO
    ../appl_core/src/System/CAN
    ../appl_core/src/System/CAN/Devices
    ../appl_core/src/System/Ethernet
    ../appl_core/src/System/NVM
    ../appl_core/src/AgvWork
    ../appl_core/src/AgvDiag
    ../appl_core/src/AgvHMI
    ../appl_core/src/AgvHelper
    ../appl_core/src/Testing
)

target_compile_options(svg_harness PRIVATE
    -std=gnu99 -Wall -Wdouble-promotion
)

# --exclude-all-symbols: MinGW's ld auto-exports every public symbol from a -shared
# build by default (unlike MSVC) - without this flag, all of gt_can_devs, every
# vendor BIOS stub, and every raw device-state global in the HAL layer would be
# directly readable/writable from Python via ctypes, defeating the whole point of
# the accessor-function boundary (CLAUDE.md's Bridge Interface, Approach B). This
# makes export strictly opt-in: only symbols explicitly marked
# __declspec(dllexport) (harness_init/harness_step/accessors - added in a later
# step) will appear in the export table.
target_link_options(svg_harness PRIVATE
    -Wl,--exclude-all-symbols
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/svg_harness.map
)

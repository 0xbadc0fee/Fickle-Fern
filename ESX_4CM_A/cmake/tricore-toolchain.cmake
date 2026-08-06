# CMake toolchain file for the HighTec TriCore GCC cross compiler used to build
# appl_core / boot_core / safety_core outside of the LogiCAD (Eclipse CDT) IDE.
#
# Usage:
#   cmake -S ESX_4CM_A -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=ESX_4CM_A/cmake/tricore-toolchain.cmake
#
# The toolchain root can be overridden with -DTRICORE_TOOLCHAIN_ROOT=... if it is
# not installed at the default HighTec location.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tricore)

if(CMAKE_HOST_WIN32)
    set(_exe_suffix ".exe")
else()
    set(_exe_suffix "")
endif()

if(NOT DEFINED TRICORE_TOOLCHAIN_ROOT)
    if(CMAKE_HOST_WIN32)
        set(TRICORE_TOOLCHAIN_ROOT "C:/HIGHTEC/toolchains/tricore/v4.9.2.0" CACHE PATH "Root of the HighTec TriCore toolchain")
    else()
        set(TRICORE_TOOLCHAIN_ROOT "/opt/HIGHTEC/toolchains/tricore/v4.9.2.0" CACHE PATH "Root of the HighTec TriCore toolchain")
    endif()
endif()

set(TRICORE_TOOLCHAIN_BIN "${TRICORE_TOOLCHAIN_ROOT}/bin")

set(CMAKE_C_COMPILER   "${TRICORE_TOOLCHAIN_BIN}/tricore-gcc${_exe_suffix}")
set(CMAKE_ASM_COMPILER "${TRICORE_TOOLCHAIN_BIN}/tricore-as${_exe_suffix}")
set(CMAKE_CXX_COMPILER "${TRICORE_TOOLCHAIN_BIN}/tricore-gcc${_exe_suffix}")

set(CMAKE_OBJCOPY "${TRICORE_TOOLCHAIN_BIN}/tricore-objcopy${_exe_suffix}" CACHE FILEPATH "objcopy")
set(CMAKE_AR      "${TRICORE_TOOLCHAIN_BIN}/tricore-ar${_exe_suffix}" CACHE FILEPATH "archiver")
set(CMAKE_SIZE    "${TRICORE_TOOLCHAIN_BIN}/tricore-size${_exe_suffix}" CACHE FILEPATH "size")

# This is a bare-metal target (-nocrt0, linked via a custom .ld script). CMake's
# compiler sanity check tries to link a full executable; that fails here without
# a linker script and entry point. Restrict the check to a static-library try-compile.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH "${TRICORE_TOOLCHAIN_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

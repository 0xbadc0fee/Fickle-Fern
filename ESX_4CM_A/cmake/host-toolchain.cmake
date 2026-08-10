# CMake toolchain file for the MinGW-w64 host build of the SVG_HARNESS variant
# (see CLAUDE.md and ../host_harness/CMakeLists.txt). This is a native Windows
# x86-64 build, NOT a cross-compile - contrast with tricore-toolchain.cmake,
# which cross-compiles for the TriCore target. Never invoke both toolchains
# against the same build directory; never add host_harness via add_subdirectory
# from the top-level ESX_4CM_A/CMakeLists.txt, which is TriCore-toolchain-bound.
#
# Usage:
#   cmake -S ESX_4CM_A/host_harness -B build-harness -G Ninja -DCMAKE_TOOLCHAIN_FILE=ESX_4CM_A/cmake/host-toolchain.cmake
#
# IMPORTANT: gcc.exe's actual compiler proper, cc1.exe, depends on DLLs that live
# alongside it in this toolchain's bin/ directory (libgmp/libmpfr/libmpc/libiconv/
# libwinpthread/zlib). Pointing CMAKE_C_COMPILER at gcc.exe by full path is NOT
# sufficient on its own - the invoking shell's PATH must also include this
# toolchain's bin/ directory at BOTH configure time and build time, or cc1.exe
# fails to load silently (exit 1, no diagnostic). See CLAUDE.md's toolchain note
# for the full story (previously misdiagnosed as a Bash-tool sandbox problem;
# re-confirmed to be a plain PATH issue affecting both Bash and PowerShell
# equally). Prepend it yourself before running cmake/ninja, e.g.:
#   PowerShell: $env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
#   Bash:       export PATH="/c/msys64/mingw64/bin:$PATH"
#
# The toolchain root can be overridden with -DMINGW_TOOLCHAIN_ROOT=... if it is
# not installed at the default MSYS2 location.

if(NOT DEFINED MINGW_TOOLCHAIN_ROOT)
    set(MINGW_TOOLCHAIN_ROOT "C:/msys64/mingw64" CACHE PATH "Root of the MSYS2 MinGW-w64 toolchain")
endif()

set(CMAKE_C_COMPILER "${MINGW_TOOLCHAIN_ROOT}/bin/gcc.exe")

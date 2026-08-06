# Splits the merged compile_commands.json (written by CMake into the build directory)
# into a per-core copy placed at <core>/compile_commands.json in the source tree, so an
# LSP (clangd, ccls) started from an editor rooted at appl_core/, boot_core/ or
# safety_core/ discovers the right compiler flags by walking up from the open file.
#
# Invoked as: cmake -DCOMPDB_INPUT=... -DCOMPDB_CORES="a;b;c" -DCOMPDB_ROOT=... -P split_compile_commands.cmake

if(NOT EXISTS "${COMPDB_INPUT}")
    message(FATAL_ERROR "split_compile_commands: input not found: ${COMPDB_INPUT}")
endif()

file(READ "${COMPDB_INPUT}" _json)
string(JSON _count LENGTH "${_json}")

if(_count EQUAL 0)
    message(FATAL_ERROR "split_compile_commands: ${COMPDB_INPUT} contains no entries")
endif()

math(EXPR _last "${_count} - 1")

foreach(_core ${COMPDB_CORES})
    set(_entries "[]")
    set(_entries_count 0)
    foreach(_i RANGE ${_last})
        string(JSON _entry GET "${_json}" ${_i})
        string(JSON _file GET "${_entry}" file)
        string(REPLACE "\\" "/" _file_norm "${_file}")
        string(FIND "${_file_norm}" "/${_core}/" _pos)
        if(NOT _pos EQUAL -1)
            string(JSON _entries SET "${_entries}" ${_entries_count} "${_entry}")
            math(EXPR _entries_count "${_entries_count} + 1")
        endif()
    endforeach()
    set(_out "${COMPDB_ROOT}/${_core}/compile_commands.json")
    file(WRITE "${_out}" "${_entries}")
    message(STATUS "split_compile_commands: wrote ${_entries_count} entries to ${_out}")
endforeach()

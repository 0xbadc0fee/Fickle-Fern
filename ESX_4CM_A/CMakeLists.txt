cmake_minimum_required(VERSION 3.20)

project(Flory_8772-4CM C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------------------------------------------------
# Reproduces the LogiCAD / Eclipse CDT "post-build step" for a core:
#   1. tricore-objcopy -O ihex  <elf> <core>_unsigned.hex
#   2. openSYDEFlashSignatureGenerator -i <core>_unsigned.hex -o <core>.hex
# Output is written next to the executable in the build tree - it intentionally does NOT
# touch the checked-in <core>/result/<core>.hex, which serves as the known-good reference
# used to validate that this CMake build produces an equivalent image.
# ----------------------------------------------------------------------------------------------------------------------
function(esx_add_hex_output target)
    set(core_dir "${CMAKE_CURRENT_SOURCE_DIR}")
    set(out_dir "${CMAKE_CURRENT_BINARY_DIR}")
    set(unsigned_hex "${out_dir}/${target}_unsigned.hex")
    set(signed_hex "${out_dir}/${target}.hex")

    if(CMAKE_HOST_WIN32)
        set(fsg_exe "${core_dir}/bat/osy_fsg/windows/openSYDEFlashSignatureGenerator.exe")
    else()
        set(fsg_exe "${core_dir}/bat/osy_fsg/linux/openSYDEFlashSignatureGenerator")
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${target}>" "${unsigned_hex}"
        COMMAND ${fsg_exe} -i "${unsigned_hex}" -o "${signed_hex}"
        BYPRODUCTS "${unsigned_hex}" "${signed_hex}"
        COMMENT "esx: generating signed HEX for ${target}"
        VERBATIM
    )
endfunction()

add_subdirectory(boot_core)
add_subdirectory(appl_core)
add_subdirectory(safety_core)

# Give each core its own compile_commands.json (see cmake/split_compile_commands.cmake)
# so clangd/ccls in Neovim/Emacs resolve correct flags when opening files under
# appl_core/, boot_core/ or safety_core/ directly.
add_custom_target(split_compile_commands ALL
    COMMAND ${CMAKE_COMMAND}
        -DCOMPDB_INPUT=${CMAKE_BINARY_DIR}/compile_commands.json
        -DCOMPDB_CORES=boot_core\;appl_core\;safety_core
        -DCOMPDB_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/split_compile_commands.cmake
    COMMENT "esx: splitting compile_commands.json per core for LSP use"
    VERBATIM
)

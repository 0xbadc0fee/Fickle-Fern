# ----------------------------------------------------------------------------------------------------------------------
# lwIP library
#
# Custom Targets:
# - lwip-api
#
# Dependencies:
# - application_core_c_bios
#
# This module is a precompiled, ready to use library. It is immutable and can't be further configured. Target 'lwip-api'
# does only export the library name and the include path to its API definition. Consuming targets can add entries to
# their INCLUDE_DIRECTORIES by $<TARGET_PROPERTY:lwip-api,INTERFACE_INCLUDE_DIRECTORIES>
#
# Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
# ----------------------------------------------------------------------------------------------------------------------
add_library(lwip-api STATIC IMPORTED GLOBAL)
set_target_properties(lwip-api PROPERTIES
   IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libesx_4cm_a_lwip.a"
)
target_include_directories(lwip-api INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_directories(lwip-api INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
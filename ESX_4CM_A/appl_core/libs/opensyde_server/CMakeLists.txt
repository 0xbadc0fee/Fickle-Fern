# ----------------------------------------------------------------------------------------------------------------------
# openSYDE Server library for target_name Application Core
#
# Custom Targets:
#  - application_core_opensyde_server
#
# Dependencies:
# - application_core_c_bios
# - lwip
#
# This module is a precompiled, ready to use library. It is immutable and can't be further configured.
# Target 'application_core_opensyde_server' does only export the library name and the include path to its API
# definition. Consuming targets can add entries to their INCLUDE_DIRECTORIES by
# - $<TARGET_PROPERTY:application_core_opensyde_server_api,INTERFACE_INCLUDE_DIRECTORIES>
#
# Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
# ----------------------------------------------------------------------------------------------------------------------
add_library(application_core_opensyde_server_api STATIC IMPORTED GLOBAL)
set_target_properties(application_core_opensyde_server_api PROPERTIES
   IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libesx_4cm_a_opensyde_server_application_core.a"
)
target_include_directories(application_core_opensyde_server_api INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_directories(application_core_opensyde_server_api INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
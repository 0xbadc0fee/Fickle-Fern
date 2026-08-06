add_subdirectory(libs/bios)
add_subdirectory(libs/lwip)
add_subdirectory(libs/opensyde_server)
add_subdirectory(libs/opensyde_j1939_handler)

add_executable(appl_core
    src/AgvChassis/cooling_fan_control.c
    src/AgvChassis/engine_starter_control.c
    src/AgvChassis/lighting_control.c
    src/AgvChassis/misc_control.c
    src/AgvChassis/power_assist_control.c
    src/AgvChassis/propulsion_control.c
    src/AgvChassis/throttle_control.c
    src/AgvDiag/dashboard_handler.c
    src/AgvDiag/fault_handler.c
    src/AgvHelper/lowpass_filter.c
    src/AgvHelper/moving_avg_filter.c
    src/AgvHelper/pid_output.c
    src/AgvHelper/ramp_calc.c
    src/AgvHelper/toggle_button.c
    src/AgvWork/auger_cart_control.c
    src/AgvWork/cleaning_chains_control.c
    src/AgvWork/elevator_control.c
    src/AgvWork/front_sweeps_control.c
    src/AgvWork/header_lift_control.c
    src/AgvWork/hitch_position_control.c
    src/AgvWork/rotary_trap_control.c
    src/AgvWork/stick_box_control.c
    src/AgvWork/stick_remover_control.c
    src/AgvWork/suction_fan_control.c
    src/HAL/STW_4CM_HAL/system.c
    src/HAL/alarm_handler_lib.c
    src/HAL/can_handler_lib.c
    src/HAL/input_handler_lib.c
    src/HAL/nvm_handler_lib.c
    src/HAL/output_handler_lib.c
    src/System/Ethernet/ethernet_init.c
    src/System/IO/hw_inputs.c
    src/System/IO/hw_outputs.c
    src/System/NVM/nvm_handler.c
    src/main.c
    src/opensyde/generated/comm_j1939_can1.c
    src/opensyde/generated/comm_j1939_can2.c
    src/opensyde/generated/configuration_data_pool.c
    src/opensyde/generated/dashboard_data_pool.c
    src/opensyde/generated/j1939_data_pool.c
    src/opensyde/generated/osy_init.c
    src/opensyde/j1939/osy_app_j1939.c
    src/opensyde/osy_app.c
    src/opensyde/osy_srv.c
)
set_target_properties(appl_core PROPERTIES SUFFIX ".elf")

target_include_directories(appl_core PRIVATE
    src
    src/AgvChassis
    src/AgvDiag
    src/AgvHelper
    src/AgvHMI
    src/AgvWork
    src/HAL
    src/HAL/STW_4CM_HAL
    src/System
    src/System/CAN
    src/System/CAN/Devices
    src/System/Ethernet
    src/System/IO
    src/System/NVM
    src/opensyde
    src/opensyde/generated
    src/opensyde/j1939
)

target_compile_options(appl_core PRIVATE
    -mcpu=tc29xx -g2 -Os -std=gnu99 -fno-common -ffunction-sections -fdata-sections -Wall -Wdouble-promotion
)

target_link_libraries(appl_core PRIVATE
    application_core_opensyde_server_api
    application_core_j1939_handler_api
    lwip-api
    esx_4cm_a_c_bios_application_core
)

target_link_options(appl_core PRIVATE
    -mcpu=tc29xx
    -nocrt0
    -T "${CMAKE_CURRENT_SOURCE_DIR}/libs/bios/link_c_layout_2.ld"
    -Wl,--gc-sections
    -Wl,--mem-holes
    -Wl,--extmap=a
    -Wl,--cref
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/appl_core.map
    -Wl,-XML-Map=${CMAKE_CURRENT_BINARY_DIR}/appl_core.xml
)

esx_add_hex_output(appl_core)

//-----------------------------------------------------------------------------
/**
 * \file       hw_inputs.c
 * \brief      System - Hardware Inputs Module
 *
 * \addtogroup System
 * @{
 * \addtogroup HwInputs Hardware Inputs
 *
 * The Hardware Inputs module manages the physical pin assignments and
 * direct hardware-level reading for the controller's inputs. It serves
 * as the low-level interface connecting physical sensors, switches,
 * and signals to the logical input handlers within the system.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2025
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Dec 9, 2025 kyle.boch
 *
 * @{
 */
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
#include "stwtypes.h"

//PROJECT
#include "input_handler_lib.h"
#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
//TODO_STW: Abstract X_IN_.... to "CX_PINXX"

// Define vehicle specific inputs
T_VehicleInput tvi_hyd_oil_temp =
{
    .Name_Description       = "HYD_OIL_TEMP",
    .u16_hardwareID         = X_IN_IDA5V_7,
    .e_inputType            = IT_VOLTAGE,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520100,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 500,
    .s32_diagMax            = 4500,
};

T_VehicleInput tvi_relief_press =
{
    .Name_Description       = "RELIEF_PRESS",
    .u16_hardwareID         = X_IN_IDA35V_6,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520101,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_head_limit =
{
    .Name_Description       = "HEAD_LIMIT",
    .u16_hardwareID         = X_IN_IACV_3,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520102,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 12000,
};

T_VehicleInput tvi_right_switch =
{
    .Name_Description       = "RIGHT_SWITCH",
    .u16_hardwareID         = X_IN_IDA35V_3,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520103,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_left_switch =
{
    .Name_Description       = "LEFT_SWITCH",
    .u16_hardwareID         = X_IN_IDA35V_4,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520104,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_air_restrict =
{
    .Name_Description       = "AIR_RESTRICT",
    .u16_hardwareID         = X_IN_IDA5V_1,
    .e_inputType            = IT_VOLTAGE,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520105,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 500,
    .s32_diagMax            = 4500,
};

T_VehicleInput tvi_fuel_level =
{
    .Name_Description       = "FUEL_LEVEL",
    .u16_hardwareID         = X_IN_IDA5V_2,
    .e_inputType            = IT_VOLTAGE,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520106,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 500,
    .s32_diagMax            = 4500,
};

T_VehicleInput tvi_traction_valve =
{
    .Name_Description       = "TRACTION_VALVE",
    .u16_hardwareID         = X_IN_IDA35V_5,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520107,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_hyd_fluid_level =
{
    .Name_Description       = "HYD_FLUID_LEVEL",
    .u16_hardwareID         = X_IN_IDA35V_1,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520108,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_wheel_speed =
{
    .Name_Description       = "WHEEL_SPEED",
    .u16_hardwareID         = X_IN_IDA5V_3,
    .e_inputType            = IT_FREQ,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520109,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 600,
    .s32_diagMax            = 20000000,
};

T_VehicleInput tvi_park_brake =
{
    .Name_Description       = "PARK_BRAKE",
    .u16_hardwareID         = X_IN_IACV_2,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520110,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 12000,
};

T_VehicleInput tvi_fan_speed =
{
    .Name_Description       = "FAN_SPEED",
    .u16_hardwareID         = X_IN_IDA5V_4,
    .e_inputType            = IT_FREQ,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520111,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 600,
    .s32_diagMax            = 20000000,
};

T_VehicleInput tvi_ignition_switch =
{
    .Name_Description       = "IGNITION_SWITCH",
    .u16_hardwareID         = X_IN_IACV_1,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520112,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 12000,
};

T_VehicleInput tvi_cab_door =
{
    .Name_Description       = "CAB_DOOR",
    .u16_hardwareID         = X_IN_IDA35V_2,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520113,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

T_VehicleInput tvi_throttle_up =
{
    .Name_Description       = "THROTTLE_UP",
    .u16_hardwareID         = X_IN_IDA5V_5,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520114,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 5000,
};

T_VehicleInput tvi_throttle_down =
{
    .Name_Description       = "THROTTLE_DOWN",
    .u16_hardwareID         = X_IN_IDA5V_6,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520115,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 5000,
};

T_VehicleInput tvi_head_pressure =
{
    .Name_Description       = "HEAD_PRESSURE",
    .u16_hardwareID         = X_IN_IDA35V_7,
    .e_inputType            = IT_DIGITAL,
    .f32_inputValue         = FALSE,
    .f32_prevInputValue     = FALSE,
    .mq_inputChanged        = TRUE,
    .u8_diagEnabled         = TRUE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520116,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },
    .u16_dti                = 1000,
    .s32_diagMin            = 0,
    .s32_diagMax            = 19000,
};

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Hardware Inputs
 *
 *  This funciton takes all the inputs described as modules global variables and adds them to the update list using
 *  add_hwInput().  After all inputs are added to the update list, the handler (and all inputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return s16_error Error Code
 */
sint16 init_hwInputs(void)
{
    sint16 s16_return = C_NO_ERR;

    s16_return |= add_hwInput(tvi_hyd_oil_temp);
    s16_return |= add_hwInput(tvi_relief_press);
    s16_return |= add_hwInput(tvi_head_limit);
    s16_return |= add_hwInput(tvi_right_switch);
    s16_return |= add_hwInput(tvi_left_switch);
    s16_return |= add_hwInput(tvi_air_restrict);
    s16_return |= add_hwInput(tvi_fuel_level);
    s16_return |= add_hwInput(tvi_traction_valve);
    s16_return |= add_hwInput(tvi_hyd_fluid_level);
    s16_return |= add_hwInput(tvi_wheel_speed);
    s16_return |= add_hwInput(tvi_park_brake);
    s16_return |= add_hwInput(tvi_fan_speed);
    s16_return |= add_hwInput(tvi_ignition_switch);
    s16_return |= add_hwInput(tvi_cab_door);
    s16_return |= add_hwInput(tvi_throttle_up);
    s16_return |= add_hwInput(tvi_throttle_down);
    s16_return |= add_hwInput(tvi_head_pressure);

    s16_return |= init_inputHandler();

    return s16_return;
}

/** \brief Update Hardware Inputs
 *
 *  This funciton makes a single call to update_inputHandler (see corresponding documentation)takes all the inputs described as modules global variables and adds them to the update list using
 *  add_hwInput().  After all inputs are added to the update list, the handler (and all inputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return update_inputHandler() - Pass through return code for indication of update_inputHandler execution.
 */
sint16 update_hwInputs(void)
{
    return update_inputHandler();
}


//EOF

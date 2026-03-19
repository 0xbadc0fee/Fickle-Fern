//-----------------------------------------------------------------------------
/*! \file       hw_outputs.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 4, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_out.h"
#include "output_handler_lib.h"

#include "SPN_definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
// Define vehicle specific inputs

T_VehicleOutput tvo_auto_unload =
{
    .Name_Description = "AUTO_UNLOAD",
    .u16_hardwareID = X_OUT_OPHSP2A_13,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520200,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_manual_unload =
{
    .Name_Description = "MANUAL_UNLOAD",
    .u16_hardwareID = X_OUT_OPL4A_1,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520201,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_cool_fan_direc =
{
    .Name_Description = "COOL_FAN_DIRECTION",
    .u16_hardwareID = X_OUT_OPHSP4A_2,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520202,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_cool_fan_speed =
{
    .Name_Description = "COOL_FAN_SPEED",
    .u16_hardwareID = X_OUT_OPHSP2A_9,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520203,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_flow_control =
{
    .Name_Description = "FLOW_CONTROL",
    .u16_hardwareID = X_OUT_OPHSP2A_8,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520204,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_starter_relay =
{
    .Name_Description = "STARTER_RELAY",
    .u16_hardwareID = X_OUT_OPHSP4A_1,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520205,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_dv_sweep =
{
    .Name_Description = "DV_SWEEP",
    .u16_hardwareID = X_OUT_OPHSP2A_10,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520206,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_head_lift =
{
    .Name_Description = "HEAD_LIFT_COIL",
    .u16_hardwareID = X_OUT_OPL4A_3,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520207,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_UB},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_GND},
            [2] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OL},
            [3] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OC}
        }
    },

    .u16_dti = 1000,
};

T_VehicleOutput tvo_head_lower =
{
    .Name_Description = "HEAD_LOWER_COIL",
    .u16_hardwareID = X_OUT_OPL4A_2,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520208,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_UB},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_GND},
            [2] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OL},
            [3] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OC}
        }
    },

    .u16_dti = 1000
};



T_VehicleOutput tvo_hitch_extend =
{
    .Name_Description = "HITCH_EXTEND",
    .u16_hardwareID = X_OUT_OPHSP2A_14,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520209,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_UB},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_GND},
            [2] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OL},
            [3] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OC}
        }
    },

    .u16_dti = 1000
};

T_VehicleOutput tvo_hitch_retract =
{
    .Name_Description = "HITCH_RETRACT",
    .u16_hardwareID = X_OUT_OPHSP2A_15,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520210,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_UB},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_SHORT_GND},
            [2] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OL},
            [3] = {.u8_is_active = FALSE, .u8_fmi_value = e_OUTFAULT_OC}
        }
    },

    .u16_dti = 1000
};

T_VehicleOutput tvo_taillights =
{
    .Name_Description = "TAIL_LIGHTS",
    .u16_hardwareID = X_OUT_OPL2A_2,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520211,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_headlights =
{
    .Name_Description = "HEADLIGHTS",
    .u16_hardwareID = X_OUT_OPL2A_3,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520212,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_worklights =
{
    .Name_Description = "WORKLIGHTS",
    .u16_hardwareID = X_OUT_OPL2A_4,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520213,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_regen_allow =
{
    .Name_Description = "REGEN_ALLOW",
    .u16_hardwareID = X_OUT_OPL4A_4,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520214,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_power_assist =
{
    .Name_Description = "POWER_ASSIST",
    .u16_hardwareID = X_OUT_OPHSP4A_4,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520215,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_traction_valve =
{
    .Name_Description = "TRACTION_VALVE",
    .u16_hardwareID = X_OUT_OPHSP2A_1,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520216,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_shift_coil =
{
    .Name_Description = "SHIFT_COIL",
    .u16_hardwareID = X_OUT_OPHSP2A_12,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520217,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_backup_alarm =
{
    .Name_Description = "BACKUP_ALARM",
    .u16_hardwareID = X_OUT_OPHSP4A_3,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520218,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_propel_fwd =
{
    .Name_Description = "PROPEL_FWD",
    .u16_hardwareID = X_OUT_OPHSP2A_4,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520219,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_propel_rev =
{
    .Name_Description = "PROPEL_REV",
    .u16_hardwareID = X_OUT_OPHSP2A_5,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520220,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_rotary_trap =
{
    .Name_Description = "ROTARY_TRAP",
    .u16_hardwareID = X_OUT_OPHSP2A_7,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520221,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_shaft_pump =
{
    .Name_Description = "SHAFT_PUMP",
    .u16_hardwareID = X_OUT_OPHSP2A_11,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520222,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_stickbox_close =
{
    .Name_Description = "STICKBOX_CLOSE",
    .u16_hardwareID = X_OUT_OPHSP2A_3,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520223,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_stickbox_on=
{
    .Name_Description = "STICKBOX_ON",
    .u16_hardwareID = X_OUT_OPHSP2A_2,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520224,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_stick_remover=
{
    .Name_Description = "STICK_REMOVER",
    .u16_hardwareID = X_OUT_OPL2A_1,
    .e_outputType = OT_DIGITAL,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520225,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

T_VehicleOutput tvo_fan_hydro_fwd =
{
    .Name_Description = "FAN_HYDRO_FWD",
    .u16_hardwareID = X_OUT_OPHSP2A_6,
    .e_outputType = OT_PWM,

    .f32_outputValue = 0.0F,
    .f32_prevOutputValue = 0.0F,
    .mq_outputChanged = TRUE,

    .u8_diagEnabled = FALSE,
    .t_fault=
    {
        .u8_dm1_enable      = FALSE,
        .u8_fault_status    = FALSE,
        .u32_spn            = SPN_520226,
        .t_fmi = {
            [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
            [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
        }
    },

    .u16_dti = 0,
};

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize Hardware Outputs
 *
 *  This funciton takes all the outputs described as modules global variables and adds them to the update list using
 *  add_hwOutput().  After all outputs are added to the update list, the handler (and all outputs in the update list) are initialized
 *  according to their set configuration.
 *
 *  \return s16_error Error Code
 */
sint16 init_hwOutputs(void)
{
    sint16 s16_return = C_NO_ERR;

    s16_return |= add_hwOutput(tvo_auto_unload);
    s16_return |= add_hwOutput(tvo_manual_unload);
    s16_return |= add_hwOutput(tvo_cool_fan_direc);
    s16_return |= add_hwOutput(tvo_cool_fan_speed);
    s16_return |= add_hwOutput(tvo_flow_control);
    s16_return |= add_hwOutput(tvo_starter_relay);
    s16_return |= add_hwOutput(tvo_dv_sweep);
    s16_return |= add_hwOutput(tvo_head_lift);
    s16_return |= add_hwOutput(tvo_head_lower);
    s16_return |= add_hwOutput(tvo_hitch_extend);
    s16_return |= add_hwOutput(tvo_hitch_retract);
    s16_return |= add_hwOutput(tvo_taillights);
    s16_return |= add_hwOutput(tvo_headlights);
    s16_return |= add_hwOutput(tvo_worklights);
    s16_return |= add_hwOutput(tvo_regen_allow);
    s16_return |= add_hwOutput(tvo_power_assist);
    s16_return |= add_hwOutput(tvo_traction_valve);
    s16_return |= add_hwOutput(tvo_shift_coil);
    s16_return |= add_hwOutput(tvo_backup_alarm);
    s16_return |= add_hwOutput(tvo_propel_fwd);
    s16_return |= add_hwOutput(tvo_propel_rev);
    s16_return |= add_hwOutput(tvo_rotary_trap);
    s16_return |= add_hwOutput(tvo_shaft_pump);
    s16_return |= add_hwOutput(tvo_stickbox_close);
    s16_return |= add_hwOutput(tvo_stickbox_on);
    s16_return |= add_hwOutput(tvo_stick_remover);
    s16_return |= add_hwOutput(tvo_fan_hydro_fwd);

    s16_return |= init_outputHandler();

    return s16_return;
}

/** \brief Update Hardware Outputs
 *
 *  This funciton makes a single call to update_outputHandler (see corresponding documentation)takes all the outputs described as modules global variables and gathers
 *  the latest values and fault status'
 *
 *  \return update_outputHandler() - Pass through return code for indication of update_outputHandler execution.
 */
sint16 update_hwOutputs(void)
{
    return update_outputHandler();
}

//EOF

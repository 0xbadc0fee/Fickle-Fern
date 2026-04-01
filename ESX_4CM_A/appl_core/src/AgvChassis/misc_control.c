//-----------------------------------------------------------------------------
/*! \file       misc_control.c
    \brief      The Miscellaneous Control Module shall provide supporting logic for
    auxiliary features within the 8772 Harvester application.

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     March 6, 2026 Tiffany.Gohnert
 */

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "system.h"
//PROJECT
#include "misc_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "fault_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define PROGRAM_START_DEB_MS (3500u) //3.5 seconds
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 update_filterMinder(void);
sint16 update_fuelLevel(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_MiscControl mt_misc;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
//-----------------------------------------------------------------------------
// FR-23.1, FR-23.2, FR-23.3
// IR-23.1
//-----------------------------------------------------------------------------
sint16 update_filterMinder(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_fault = FALSE;
    float32 f32_raw = 0.0F;
    float32 f32_gauge = 0.0F;

    get_inputFaultStatus("AIR_FILTER_RESTRICTION", &u8_fault);

    if(u8_fault == FALSE)
    {
        get_inputValue("AIR_FILTER_RESTRICTION", f32_raw);

        f32_gauge = ((f32_raw - FILTER_MINDER_RAW_MIN) * PERCENT_SCALE) /
        (FILTER_MINDER_RAW_MAX - FILTER_MINDER_RAW_MIN);

        f32_gauge = clampF32(f32_gauge, 0.0F, 100.0F);
    }
    else
    {
        s16_error += C_WARN;
    }

    *(mt_misc.pf32_filter_minder_gauge_pct) = f32_gauge;
    *(mt_misc.pf32_filter_restriction_pct) =
    clampF32((f32_gauge * 100.0F) / FILTER_MINDER_RESTRICTION_MAX_PCT, 0.0F, 100.0F);
    *(mt_misc.pu8_service_filter_status) =
    (f32_gauge >= FILTER_MINDER_RESTRICTION_MAX_PCT) ? TRUE : FALSE;

    return s16_error;
}

/** \brief Update Fuel Level AgvChassis - Miscellaneous Control
 *
 *  This function updates the fuel level the AgvChassis - Miscellaneous Control Logic.
 *  FR-23.4, FR-23.5, FR-23.6, FR-23.7 IR-23.2, IR-23.3
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_fuelLevel(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_fuel_flt = FALSE;
    float32 f32_raw = 0.0F;
    float32 f32_sensor = 0.0F;
    float32 f32_gauge = mt_misc.f32_last_fuel_gauge;
    float32 f32_deadband = 0.0F;
    uint8 u8_low = FALSE;
    uint32 u32_time_now_ms = 0u;

    get_inputFaultStatus("FUEL_LEVEL", &u8_fuel_flt);

    if((u8_fuel_flt == FALSE) && (mt_misc.pt_nvm_misc_control->u16_fuel_high_deadband != NULL))
    {
        get_inputValue("FUEL_LEVEL", f32_raw);

        if(f32_raw <= FUEL_FAULT_THRESHOLD)
        {
            // IR-23.2, IR-23.3 Safe state
            f32_sensor = 0.0F;
            f32_gauge = 0.0F;
            mt_misc.f32_last_fuel_gauge = 0.0F;
            mt_misc.u8_low_fuel_timer_active = FALSE;
            mt_misc.u32_low_fuel_timer_start_ms = 0u;
            u8_low = FALSE;
            s16_error += C_WARN;
        }
        else
        {
            // FR-23.4 Read fuel level sensor input and convert to normalized percentage output
            f32_sensor = ((f32_raw - FUEL_RAW_MIN) * PERCENT_SCALE_01PCT) /
            (FUEL_RAW_MAX - FUEL_RAW_MIN);
            f32_sensor = clampF32(f32_sensor, 0.0F, PERCENT_SCALE_01PCT);

            lowpassFilter(&mt_misc.t_fuel_level_lpf, f32_sensor, FUEL_SAFE_STATE);
            f32_sensor = clampF32(mt_misc.t_fuel_level_lpf.f32_output, 0.0F, PERCENT_SCALE_01PCT);

            // FR-23.5 Apply configurable Fuel High Deadband
            f32_deadband = clampF32(*(mt_misc.pt_nvm_misc_control->u16_fuel_high_deadband), 0.0F, PERCENT_SCALE_01PCT);

            if((f32_sensor < mt_misc.f32_last_fuel_gauge) ||
            (f32_sensor >= (mt_misc.f32_last_fuel_gauge + f32_deadband)))
            {
                f32_gauge = f32_sensor;
            }

            mt_misc.f32_last_fuel_gauge = f32_gauge;

            // FR-23.6 Compare Fuel Gauge to Low Fuel Set Point and require 5 seconds
            u32_time_now_ms = get_system_time_ms();

            if(f32_gauge <= FUEL_LOW_SETPOINT)
            {
                if(mt_misc.u8_low_fuel_timer_active == FALSE)
                {
                    mt_misc.u8_low_fuel_timer_active = TRUE;
                    mt_misc.u32_low_fuel_timer_start_ms = u32_time_now_ms;
                }
                else if((u32_time_now_ms - mt_misc.u32_low_fuel_timer_start_ms) >= FUEL_LOW_DELAY_MS)
                {
                    u8_low = TRUE;
                }
            }
            else
            {
                mt_misc.u8_low_fuel_timer_active = FALSE;
                mt_misc.u32_low_fuel_timer_start_ms = 0u;
            }
        }

        // FR-23.7 Transmit outputs to display via CAN
        *(mt_misc.pf32_fuel_level_sensor_pct) = f32_sensor / PERCENT_SCALE;
        *(mt_misc.pf32_fuel_level_gauge_pct)  = f32_gauge / PERCENT_SCALE;
        *(mt_misc.pu8_low_fuel_status)        = u8_low;

        return s16_error;


    }
    /** \brief Initialize AgvChassis - Miscellaneous Control
     *
     *  This function initializes the AgvChassis - Miscellaneous Control Logic.
     *
     *  \param _ui Pointer to the project's UI Structure
     *  \param _chkCooling Fan Pointer to the global Miscellaneous Control Checkpoints Structure
     *
     *  \return s16_error Error Code
     *  \retval C_NO_ERR Function Executed Properly
     */
    sint16 init_miscControl(T_UserInterface *_ui, T_ChkPoints_Mis *_chk_misc,T_Config_MiscrControl *_nvm_misc_control)
    {
        sint16 s16_error = C_NO_ERR;

        if((_ui == NULL) || (_chk_misc == NULL) ||  (_nvm_misc_control == NULL))
        {
            return C_WARN;
        }
        mt_misc.pf32_filter_minder_gauge_pct     = &_ui->t_display.f32_filter_minder_gauge_pct;
        mt_misc.pf32_filter_restriction_pct      = &_ui->t_display.f32_filter_restriction_pct;
        mt_misc.pu8_service_filter_status        = &_ui->t_display.u8_service_filter_status;

        mt_misc.pf32_fuel_level_sensor_pct       = &_ui->t_display.f32_fuel_level_sensor_pct;
        mt_misc.pf32_fuel_level_gauge_pct        = &_ui->t_display.f32_fuel_level_gauge_pct;
        mt_misc.pu8_low_fuel_status              = &_ui->t_display.u8_low_fuel_status;

        mt_misc.pu8_door_open_status             = &_ui->t_display.u8_door_open_status;
        mt_misc.pu8_low_hydraulic_fluid_indicator= &_ui->t_display.u8_low_hydraulic_fluid_indicator;
        mt_misc.pu8_display_brakes_engaged       = &_ui->t_display.u8_brakes_engaged_status;

        mt_misc.pu8_sw_major_revision            = &_ui->t_display.u8_software_major_revision;
        mt_misc.pu8_sw_minor_revision            = &_ui->t_display.u8_software_minor_revision;

        mt_misc.pu8_clear_machine_faults_cmd     = &_ui->t_display.u8_clear_machine_faults_cmd;

        mt_misc.f32_last_fuel_gauge_pct          = MISC_SAFE_PERCENT;
        lowpassFilter_init(&mt_misc.t_fuel_level_lpf, 0.05F, FUEL_SAFE_STATE);

        //Populate local copy of nvm variables
        mt_misc.pt_nvm_misc_control= _nvm_misc_control;

        //Populate local copy of checkpoints
        mt_misc.pt_cp_misc = _chk_misc;

        return s16_error;
    }

    /** \brief Update AgvChassis - Miscellaneous Control
     *
     *  This function contains the cyclically logic for AgvChassis - Miscellaneous Control
     *
     *  Primary logic for this function
     *
     *  Additional interlocks are utilized throughout the logic.
     *
     *
     *  \return s16_error Error Code
     *  \retval C_NO_ERR Function Executed Properly
     */
    sint16 update_miscControl(void)
    {
        sint16 s16_error = C_NO_ERR;

        uint8 u8_in_fault = FALSE;
        uint8 u8_out_fault = FALSE;
        float32 f32_value = 0.0F;

        if((mt_misc.pu8_door_open_status == NULL) ||
        (mt_misc.pu8_low_hydraulic_fluid_indicator == NULL) ||
        (mt_misc.pu8_display_brakes_engaged == NULL) ||
        (mt_misc.pu8_sw_major_revision == NULL) ||
        (mt_misc.pu8_sw_minor_revision == NULL) ||
        (mt_misc.pu8_clear_machine_faults_cmd == NULL))
        {
            return C_WARN;
        }

        s16_error += update_filterMinder() ;

        s16_error += update_fuelLevel();

        // FR-23.9 Transmit Door Open Status to display
        get_inputFaultStatus("CAB_DOOR", &u8_in_fault);
        if(u8_in_fault == FALSE)
        {
            get_inputValue("CAB_DOOR", f32_value);
            *(mt_misc.pu8_door_open_status) = (f32_value != FALSE) ? TRUE : FALSE;
        }
        else
        {
            *(mt_misc.pu8_door_open_status) = FALSE;
            s16_error += C_WARN;
        }

        // FR-23.10 Read Hydraulic Fluid Level Switch input and output indicator to display
        get_inputFaultStatus("HYD_FLUID_LEVEL", &u8_in_fault);
        if(u8_in_fault == FALSE)
        {
            get_inputValue("HYD_FLUID_LEVEL", f32_value);
            *(mt_misc.pu8_low_hydraulic_fluid_indicator) = f32_value;
        }
        else
        {
            *(mt_misc.pu8_low_hydraulic_fluid_indicator) = FALSE;
            s16_error += C_WARN;
        }

        // FR-23.11 Read Brakes Engaged input and output it to the REGEN Allow Relay hardware and to the display
        get_inputFaultStatus("PARK_BRAKE", &u8_in_fault);
        get_outputFaultStatus("REGEN_ALLOW_RELAY", &u8_out_fault);

        if((u8_in_fault == FALSE) && (u8_out_fault == FALSE))
        {
            get_inputValue("PARK_BRAKE", f32_value);

            f32_value = (f32_value != FALSE) ? TRUE : FALSE;

            set_outputValue("REGEN_ALLOW_RELAY", f32_value);
            *(mt_misc.pu8_display_brakes_engaged) = f32_value;
        }
        else
        {
            *(mt_misc.pu8_display_brakes_engaged) = FALSE;
            s16_error += C_WARN;
        }

        // FR-23.12 Transmit current Major and Minor Software Revision to display
        *(mt_misc.pu8_sw_major_revision) = MISC_SW_MAJOR_REV;
        *(mt_misc.pu8_sw_minor_revision) = MISC_SW_MINOR_REV;

        // FR-23.13 Read Clear Machine Faults Command from display and clear associated faults
        if(*(mt_misc.pu8_clear_machine_faults_cmd) != FALSE)
        {
            clear_machineFaults();
        }

        return s16_error;
    }

    //EOF

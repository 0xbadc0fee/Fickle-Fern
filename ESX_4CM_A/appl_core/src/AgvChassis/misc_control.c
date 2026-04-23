//-----------------------------------------------------------------------------
/**
 * \file       misc_control.c
 * \brief      AgvChassis - Miscellaneous Control Implementation
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup MiscControl
 * @{
 */

/**
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * March 6, 2026 Tiffany.Gohnert
 */
//-----------------------------------------------------------------------------
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

/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 update_filterMinder(void);
sint16 update_fuelLevel(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_MiscControl mt_misc; //!< Internal state instance for miscellaneous control

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/** \brief Update Filter Minder AgvChassis - Miscellaneous Control
 *
 *  This function updates the filter minder in AgvChassis - Miscellaneous Control Logic.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_filterMinder(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_minder_flt = FALSE;
    uint8 u8_service_filter_on = FALSE;
    //uint8 u8_fault_active = FALSE;

    float32 f32_raw = 0.0F;
    float32 f32_filter_pct = 0.0F;

    uint32 u32_now_ms = get_system_time_ms();


    s16_error += get_inputFaultStatus("AIR_RESTRICT", &u8_minder_flt);

    if(u8_minder_flt)
    {
        s16_error += C_WARN;
    }
    else
    {
        s16_error += get_inputValue("AIR_RESTRICT", &f32_raw);

        // Convert raw input to restriction percent (0..100)
        f32_filter_pct = ((100.0f / (mt_misc.pt_config->u16_af_max_voltage-mt_misc.pt_config->u16_af_min_voltage)) * f32_raw) -
                         ((100.0f*FM_RAW_MIN)/(mt_misc.pt_config->u16_af_max_voltage-mt_misc.pt_config->u16_af_min_voltage));

        f32_filter_pct = CLAMP(f32_filter_pct, 0.0F, 100.0F);

        // Filter restriction percent
        movingAdvFlt(&mt_misc.t_minder_flt, f32_filter_pct);

        // If filtered output is greater than stored max for 1000 ms, reset stored max to filter value
        if(mt_misc.t_minder_flt.f32_out > mt_misc.pt_config->u8_filter_rstn_max)
        {
            if(mt_misc.u8_minder_timer_active == FALSE)
            {
                mt_misc.u8_minder_timer_active = TRUE;
                mt_misc.u32_minder_timer_start_ms = u32_now_ms;
            }
            else if((u32_now_ms - mt_misc.u32_minder_timer_start_ms) >= 1000u)
            {
                mt_misc.pt_config->u8_filter_rstn_max = mt_misc.t_minder_flt.f32_out;
            }
        }
        else
        {
            mt_misc.u8_minder_timer_active = FALSE;
            mt_misc.u32_minder_timer_start_ms = 0u;
        }

        // Service on if max is greater than threshold
        u8_service_filter_on = (mt_misc.pt_config->u8_filter_rstn_max >= FM_SERVICE_THRESH) ? TRUE : FALSE;

        //TODO_STW
        //u8_fault_active = (mt_misc.t_minder_flt.f32_out <=  mt_misc.pt_config->u8_af_fault_pct) ? TRUE : FALSE;
    }

    // Outputs
    *(mt_misc.pf32_filter_restriction_pct) = mt_misc.pt_config->u8_filter_rstn_max;
    *(mt_misc.pu8_service_filter_status) = u8_service_filter_on;

    //Checkpoints
    mt_misc.pt_cp_misc->f32_filter_rest_pct = mt_misc.pt_config->u8_filter_rstn_max;
    mt_misc.pt_cp_misc->f32_minder_gauge_pct= mt_misc.t_minder_flt.f32_out;
    mt_misc.pt_cp_misc->u8_service_filter_status = u8_service_filter_on;

    return s16_error;
}

/** \brief Update Fuel Level AgvChassis - Miscellaneous Control
 *
 *  This function updates the fuel level in AgvChassis - Miscellaneous Control Logic.
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 update_fuelLevel(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_fuel_flt = FALSE;
    float32 f32_raw_fuel_level = 0.0F;
    float32 f32_fuel_pct = 0.0;
    uint8 u8_low = FALSE;
    uint32 u32_now_ms = get_system_time_ms();

    s16_error += get_inputFaultStatus("FUEL_LEVEL", &u8_fuel_flt);

    if(u8_fuel_flt)
    {
        s16_error += C_WARN;
    }
    else
    {
        s16_error += get_inputValue("FUEL_LEVEL", &f32_raw_fuel_level);

        // FR-23.4 Read fuel level sensor input and convert to normalized percentage output
        f32_fuel_pct = ((100.0f / (mt_misc.pt_config->u16_fuel_full_voltage-FUEL_RAW_MIN)) * f32_raw_fuel_level) -
                         ((100.0f*FUEL_RAW_MIN)/(mt_misc.pt_config->u16_fuel_full_voltage-FUEL_RAW_MIN));

        f32_fuel_pct = CLAMP(f32_fuel_pct, 0.0F, 100.0F);

        // fuel level average percent
        movingAdvFlt(&mt_misc.t_fuel_level_flt, f32_fuel_pct);

        // If filtered output is less than low level for 1000 ms, trigger fuel low indicator
        if(mt_misc.t_fuel_level_flt.f32_out < FUEL_LOW_SETPOINT)
        {
            if(mt_misc.u8_low_fuel_timer_active == FALSE)
            {
                mt_misc.u8_low_fuel_timer_active = TRUE;
                mt_misc.u32_low_fuel_timer_start_ms = u32_now_ms;
            }
            else if((u32_now_ms - mt_misc.u32_low_fuel_timer_start_ms) >= FUEL_LOW_DELAY_MS)
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
    *(mt_misc.pu8_fuel_level_sensor) = (uint8)((mt_misc.t_fuel_level_flt.f32_out * 255.0F) / 100.0f);
    *(mt_misc.pf32_fuel_level_gauge_pct)  = mt_misc.t_fuel_level_flt.f32_out;
    *(mt_misc.pu8_low_fuel_status)        = u8_low;

    //Checkpoints
    mt_misc.pt_cp_misc->f32_fuel_level_sensor =  (*(mt_misc.pu8_fuel_level_sensor));
    mt_misc.pt_cp_misc->f32_fuel_level_gauge_pct= mt_misc.t_fuel_level_flt.f32_out;

    return s16_error;
}

/** \brief Initialize AgvChassis - Miscellaneous Control
 *
 *  This function initializes the AgvChassis - Miscellaneous Control Logic.
 *
 *  \param _can_devs Pointer to the project's UI Structure
 *  \param _chk_misc Fan Pointer to the global Miscellaneous Control Checkpoints Structure
 *  \param _nvm_misc_control Fan Pointer to the global Miscellaneous Control NVM Structure
 *
 *  \return s16_error Error Code
 *  \retval C_NO_ERR Function Executed Properly
 */
sint16 init_miscControl(T_CANDevices *_can_devs, T_ChkPoints_Mis *_chk_misc,T_Config_MiscrControl *_nvm_misc_control)
{
    sint16 s16_error = C_NO_ERR;

    if((_can_devs == NULL) || (_chk_misc == NULL) ||  (_nvm_misc_control == NULL))
    {
        return C_WARN;
    }

    mt_misc.pu8_filter_minder_gauge           = &_can_devs->t_display.u8_filter_minder_gauge;
    mt_misc.pf32_filter_restriction_pct       = &_can_devs->t_display.f32_filter_restriction_pct;
    mt_misc.pu8_service_filter_status         = &_can_devs->t_display.u8_service_filter_status;

    mt_misc.pu8_fuel_level_sensor             = &_can_devs->t_display.u8_fuel_level_sensor;
    mt_misc.pf32_fuel_level_gauge_pct         = &_can_devs->t_display.f32_fuel_level_gauge_pct;
    mt_misc.pu8_low_fuel_status               = &_can_devs->t_display.u8_low_fuel_status;

    mt_misc.pu8_door_open_status              = &_can_devs->t_display.u8_door_open_status;
    mt_misc.pu8_low_hydraulic_fluid_indicator = &_can_devs->t_display.u8_low_hydraulic_fluid_indicator;
    mt_misc.pu8_brakes_engaged                = &_can_devs->t_display.u8_brakes_engaged_status;

    mt_misc.pu8_sw_major_revision            = &_can_devs->t_display.u8_software_major_revision;
    mt_misc.pu8_sw_minor_revision            = &_can_devs->t_display.u8_software_minor_revision;

    mt_misc.pu8_clear_faults_cmd     = &_can_devs->t_display.u8_clear_faults_cmd;


    s16_error += movingFltInit(&mt_misc.t_fuel_level_flt,
                                mt_misc.f32_fuel_level_buf,
                                FUEL_BUF_LEN,
                                FUEL_FILTER_SAFE_OUTPUT,
                                FUEL_FILTER_SAMPLE_NO,
                                FUEL_FILTER_SAMPLE_MS);

    s16_error += movingFltInit(&mt_misc.t_minder_flt,
                                mt_misc.f32_minder_buf,
                                FILTER_MINDER_BUF_LEN,
                                FILTER_MINDER_FILTER_SAFE_OUTPUT,
                                FILTER_MINDER_FILTER_SAMPLE_NO,
                                FILTER_MINDER_FILTER_SAMPLE_MS);

    mt_misc.u8_minder_timer_active = FALSE;
    mt_misc.u8_low_fuel_timer_active = FALSE;

    //Populate local copy of nvm variables
    mt_misc.pt_config= _nvm_misc_control;

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
    (mt_misc.pu8_brakes_engaged == NULL) ||
    (mt_misc.pu8_sw_major_revision == NULL) ||
    (mt_misc.pu8_sw_minor_revision == NULL) ||
    (mt_misc.pu8_clear_faults_cmd == NULL))
    {
        return C_WARN;
    }

    s16_error += update_filterMinder() ;
    s16_error += update_fuelLevel();

    // FR-23.9 Transmit Door Open Status to display
    get_inputFaultStatus("CAB_DOOR", &u8_in_fault);
    if(u8_in_fault == FALSE)
    {
        get_inputValue("CAB_DOOR", &f32_value);
        *(mt_misc.pu8_door_open_status) = (f32_value != FALSE) ? TRUE : FALSE;
    }
    else
    {
        *(mt_misc.pu8_door_open_status) = FALSE;
        s16_error += C_WARN;
    }
    mt_misc.pt_cp_misc->u8_door_open_status = *(mt_misc.pu8_door_open_status);


    // FR-23.10 Read Hydraulic Fluid Level Switch input and output indicator to display
    get_inputFaultStatus("HYD_FLUID_LEVEL", &u8_in_fault);
    if(u8_in_fault == FALSE)
    {
        get_inputValue("HYD_FLUID_LEVEL", &f32_value);
        *(mt_misc.pu8_low_hydraulic_fluid_indicator) = ((uint8)f32_value > 0)? FALSE : TRUE;
    }
    else
    {
        *(mt_misc.pu8_low_hydraulic_fluid_indicator) = FALSE;
        s16_error += C_WARN;
    }
    mt_misc.pt_cp_misc->u8_low_hyd_fluid_indicator = (*(mt_misc.pu8_low_hydraulic_fluid_indicator));


    // FR-23.11 Read Brakes Engaged input and output it to the REGEN Allow Relay hardware and to the display
    get_inputFaultStatus("PARK_BRAKE", &u8_in_fault);
    get_outputFaultStatus("REGEN_ALLOW", &u8_out_fault);

    if((u8_in_fault == FALSE) && (u8_out_fault == FALSE))
    {
        get_inputValue("PARK_BRAKE", &f32_value);

        f32_value = (f32_value != FALSE) ? TRUE : FALSE;

        set_outputValue("REGEN_ALLOW", f32_value);
        *(mt_misc.pu8_brakes_engaged) = f32_value;
    }
    else
    {
        *(mt_misc.pu8_brakes_engaged) = FALSE;
        s16_error += C_WARN;
    }
    mt_misc.pt_cp_misc->u8_brakes_engaged = (*(mt_misc.pu8_brakes_engaged));

    // FR-23.12 Transmit current Major and Minor Software Revision to display
    *(mt_misc.pu8_sw_major_revision) = MISC_SW_MAJOR_REV;
    *(mt_misc.pu8_sw_minor_revision) = MISC_SW_MINOR_REV;

    mt_misc.pt_cp_misc->u8_sw_major_revision = MISC_SW_MAJOR_REV;
    mt_misc.pt_cp_misc->u8_sw_minor_revision = MISC_SW_MINOR_REV;

    // FR-23.13 Read Clear Machine Faults Command from display and clear associated faults
    if(*(mt_misc.pu8_clear_faults_cmd) == TRUE &&
       mt_misc.u8_prev_clear_cmd != *(mt_misc.pu8_clear_faults_cmd))
    {
        clear_machineFaults();
    }
    mt_misc.u8_prev_clear_cmd = *(mt_misc.pu8_clear_faults_cmd);


    return s16_error;
}

//EOF

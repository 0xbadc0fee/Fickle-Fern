//-----------------------------------------------------------------------------
/*! \file       output_handler_lib.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 4, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "output_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 check_outputFaultStatus(uint8 u8_output);
sint16 findOutputByName(const char *targetName, uint8 *opu8_Index);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
uint8 u8_numOutputs = 0;                         //!<Number of inputs in Input List
T_VehicleOutput at_vehicleOutputs[X_OUT_COUNT];    //!< Array of configured vehicle inputs

const T_x_out_pid_parameters t_PID_PARAMETERS =
        {
            .s32_Setpoint = DEFAULT_PID_SETPOINT,
            .s32_P_Value  = DEFAULT_PID_P,
            .s16_I_Value  = DEFAULT_PID_I,
            .s16_D_Value  = DEFAULT_PID_D,
            .s16_SampleTime = DEFAULT_PID_SAMPLETIME,
            .s16_OutputLimitMin = DEFAULT_PID_DUTYCYCLE_MIN,
            .s16_OutputLimitMax = DEFAULT_PID_DUTYCYCLE_MAX
        };                                          //!<Default PID Parameters for Current Controlled Outputs

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/*! \brief   Initializes Output Handler Parameters

    Initializes all configured vehicle outputs, sets up hardware allocations, and configures diagnostics.

    \param  None
    \retval C_NO_ERR(0)   Input Handler Successfully Initialized
    \retval C_NOACT(-8)   Invalid Output Diagnostic Configuration - Initialization Failed
    \retval C_CONFIG(-10) Invalid Output Configuration - Initialization Failed


    \ingroup system_io_group
*/
sint16 init_outputHandler(void)
{
    sint16 s16_error = C_NO_ERR;
    sint16 s16_initError = C_NO_ERR;
    sint16 s16_diagError = C_NO_ERR;

    //wait for safety core output allocation
    s16_error |= x_out_client_await_allocations(10000u);

    // Initialize Outputs
    // turn on the main output switch(es)
    s16_error |= x_msw_set_state(X_MSW_01, X_ON);
    s16_error |= x_msw_set_state(X_MSW_02, X_ON);
    s16_error |= x_msw_set_state(X_MSW_03, X_ON);

    //Initialize Output Runner List
    // loop through all outputs and initialize
    for (uint8 i = 0; i < u8_numOutputs; i++)
    {
        // INIT OT_DIGITAL
        if (OT_DIGITAL == at_vehicleOutputs[i].e_outputType)
        {

            // Initialize Output
            s16_initError |= x_out_digital_init(at_vehicleOutputs[i].u16_hardwareID);
            s16_error |= x_out_set_digital(at_vehicleOutputs[i].u16_hardwareID, (uint8) at_vehicleOutputs[i].f32_outputValue);

            // check if diagnostic required
            if (at_vehicleOutputs[i].u8_diagEnabled)
            {
                s16_diagError |= x_out_digital_diag(at_vehicleOutputs[i].u16_hardwareID, at_vehicleOutputs[i].u16_dti);
            }
        }

        // INIT OT_PWM
        if (OT_PWM == at_vehicleOutputs[i].e_outputType)
        {

            // Initialize Output
            s16_initError |= x_out_pwm_init(at_vehicleOutputs[i].u16_hardwareID);
            s16_error |= x_out_set_frequency(at_vehicleOutputs[i].u16_hardwareID, DEFAULT_PWM_CC_FREQ);
            s16_error |= x_out_set_duty_cycle(at_vehicleOutputs[i].u16_hardwareID, (uint32) at_vehicleOutputs[i].f32_outputValue);


            if (at_vehicleOutputs[i].u8_diagEnabled)
            {
                s16_diagError |= x_out_pwm_diag(at_vehicleOutputs[i].u16_hardwareID, at_vehicleOutputs[i].u16_dti);
            }
        }

        // INIT OT_CC
        if (OT_CC == at_vehicleOutputs[i].e_outputType)
        {
            // Initialize Output
            s16_initError |= x_out_cc_init(at_vehicleOutputs[i].u16_hardwareID);
            s16_error |= x_out_set_frequency(at_vehicleOutputs[i].u16_hardwareID, DEFAULT_PWM_CC_FREQ);
            s16_error |= x_out_set_control_parameters(at_vehicleOutputs[i].u16_hardwareID, &t_PID_PARAMETERS);
            s16_error |= x_out_set_current_filter(at_vehicleOutputs[i].u16_hardwareID, DEFAULT_CC_FILTER);
            s16_error |= x_out_cc_set_dither(at_vehicleOutputs[i].u16_hardwareID, DEFAULT_CC_DITHER);


            if (at_vehicleOutputs[i].u8_diagEnabled)
            {
                (void) x_out_cc_diag_v2(at_vehicleOutputs[i].u16_hardwareID, at_vehicleOutputs[i].u16_dti,DEFAULT_CC_MAX_CURRENT, DEFAULT_CC_TOL_REL, DEFAULT_CC_TOL_ABS);
            }
        }
    }

    //check error status
    if(C_NO_ERR != s16_diagError)
        s16_error = s16_diagError;
    else if (C_NO_ERR != s16_initError)
        s16_error = s16_initError;


    return s16_error;
}

/** \brief Vehicle hardware input handler function

    Function to be called cyclically that reads all configured hardware inputs and updates
    their values. Also performs fault checking on all configured inputs.

    \internal Task function - called by OS scheduler
**/
sint16 update_outputHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    // loop through to check if output value needs to be updated
    for (uint8 j = 0; j < u8_numOutputs; j++)
    {
        // to test alarm handler
        check_outputFaultStatus(j);

        // check if output command has changed
        at_vehicleOutputs[j].mq_outputChanged = (fabsf(at_vehicleOutputs[j].f32_outputValue - at_vehicleOutputs[j].f32_prevOutputValue) > 1e-6f) ? TRUE : FALSE;

        // if changed, update output
        if (at_vehicleOutputs[j].mq_outputChanged)
        {
            switch (at_vehicleOutputs[j].e_outputType)
            {
                case OT_DIGITAL:
                    s16_error |= x_out_set_digital(at_vehicleOutputs[j].u16_hardwareID, (uint8) at_vehicleOutputs[j].f32_outputValue);
                    break;

                case OT_PWM:
                    s16_error |= x_out_set_duty_cycle(at_vehicleOutputs[j].u16_hardwareID, (uint32) at_vehicleOutputs[j].f32_outputValue);
                    break;


                default:
                    break;
            }

            at_vehicleOutputs[j].f32_prevOutputValue = at_vehicleOutputs[j].f32_outputValue;
        }
    }

    return s16_error;
}


/*! \brief Check output fault status
    Reads hardware fault status for specified output and updates
    the corresponding DTC fault flags in the output structure

    \param[in] u8_output Index of output to check (0 to u16_numOutputs-1)

    \return Error Return Value
    \retval C_NO_ERR
    \retval C_RANGE
    \retval C_COM Could not access specified output
 */
sint16 check_outputFaultStatus(uint8 u8_output)
{
    sint16 s16_Error = C_COM;
    uint32 u32_hwOutputFault = 0;

    //get the active faults
    s16_Error = x_out_get_active_faults(at_vehicleOutputs[u8_output].u16_hardwareID, &u32_hwOutputFault);

    if (u32_hwOutputFault)
    {
        // SHORT UB+ / OL
        if (((u32_hwOutputFault & X_OUT_FAULT_SHORT_UB) == X_OUT_FAULT_SHORT_UB))
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_SHORT_UB].u8_IsActive = TRUE;
        else
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_SHORT_UB].u8_IsActive = FALSE;

        // SHORT TO GND
        if (((u32_hwOutputFault & X_OUT_FAULT_SHORT_GND) == X_OUT_FAULT_SHORT_GND))
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_SHORT_GND].u8_IsActive = TRUE;
        else
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_SHORT_GND].u8_IsActive = FALSE;

        // OPEN LOAD
        if (((u32_hwOutputFault & X_OUT_FAULT_OPEN_LOAD) == X_OUT_FAULT_OPEN_LOAD))
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_OL].u8_IsActive = TRUE;
        else
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_OL].u8_IsActive = FALSE;

        // OVERCURRENT
        if (((u32_hwOutputFault & X_OUT_FAULT_OVERCURRENT) == X_OUT_FAULT_OVERCURRENT))
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_OC].u8_IsActive = TRUE;
        else
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_OC].u8_IsActive = FALSE;

        // PWM FAULT
        if (((u32_hwOutputFault & X_OUT_FAULT_DUTY_CYCLE_OOOB) == X_OUT_FAULT_DUTY_CYCLE_OOOB))
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_DC_OOB].u8_IsActive = TRUE;
        else
            at_vehicleOutputs[u8_output].t_dtc[e_OUTFAULT_DC_OOB].u8_IsActive = FALSE;
    }

    else if (s16_Error == C_NO_ERR && !u32_hwOutputFault)
    {
        for (uint8 i = 0; i < e_NUM_OUTFAULTS; i++)
            at_vehicleOutputs[u8_output].t_dtc[i].u8_IsActive = FALSE;
    }
    return s16_Error;
}

// Getter Functions ------------------------------------------------------------------------
/**\brief Get the current number of Initialized Outputs
 *
 * \param[out] opu16_Count Current Outputs
 *
 * \return Error Return Value
 * \retval C_NO_ERR(0) No Error
 **/
sint16 get_numOutputs(uint16 *const opu16_Count)
{
    *opu16_Count = u8_numOutputs;
    return C_NO_ERR;
}

// Setter Functions ------------------------------------------------------------------------
/*! \brief Set output value by name
    Searches for an output by name and sets its value

    \param[in] targetName Name of the output to find
    \param[in] value New output value to set

    \return s16_Error status
    \retval C_NO_ERR
    \retval C_RANGE
 */
sint16 set_outputValue(const char *targetName, float32 value)
{
    sint16 s16_Error = C_RANGE;
    uint8 u8_index = 255;

    s16_Error = findOutputByName(targetName, &u8_index);

    if (u8_index != 255)
    {
        at_vehicleOutputs[u8_index].f32_outputValue = value;
        s16_Error = C_NO_ERR;
    }

    return s16_Error;
}

// Helper Functions ------------------------------------------------------------------------
/** \brief Add a hardware output to the output list

    Adds the specified output configuration to the global output array
    and increments the output counter

    \param[in] output Output configuration structure to add

    \return Error Return Value
    \retval C_NO_ERR(0)  Output added to list
    \retval C_RANGE(-5)  Failed, output list already full
    \retval C_CONFIG(-10)  Failed, output pin already configured, duplicate
    \retval C_UNKNOWN_ERR(-1)  Failed, unknown error
 */
sint16 add_hwOutput(T_VehicleOutput output)
{
    sint16 s16_Error = C_UNKNOWN_ERR;

    //Check if array is full - C_RANGE
    if (u8_numOutputs >= X_OUT_COUNT)
    {
        s16_Error = C_RANGE;
    }
    else
    {
        // Check for duplicate hardware pins - C_CONFIG
        s16_Error = C_NO_ERR;  // Assume no duplicates
        for (uint16 i = 0; i < u8_numOutputs; i++)
        {
            if (at_vehicleOutputs[i].u16_hardwareID == output.u16_hardwareID)
            {
                s16_Error = C_CONFIG;
                break;
            }
        }

        //Complete add operation if all checks above pass
        if (s16_Error == C_NO_ERR)
        {
            at_vehicleOutputs[u8_numOutputs] = output;
            u8_numOutputs++;
        }
    }
    return s16_Error;
}

/**\brief Find output by name
 * Searches the output array for an output with the specified name
 *
 * \param[in] targetName Name of the output to find
 * \param[out] 0-254 Valid output index
 * \pre u16_numOutputs < MAX_NUM_OUTPUTS
 *
 * \return Error Return Value
 * \retval C_NO_ERR(0)  Output found
 * \retval C_RANGE(-5)  Output not found
 */
sint16 findOutputByName(const char *targetName, uint8 *opu8_Index)
{
    sint16 s16_Error = C_RANGE;

    for (uint8 i = 0; i < u8_numOutputs; i++)
    {
        if (strcmp(at_vehicleOutputs[i].Name_Description, targetName) == 0)
        {
            *opu8_Index = i;  // Return index of matching item
            s16_Error = C_NO_ERR;
        }
    }
    *opu8_Index = 255;  // Not found
    return s16_Error;
}
//EOF

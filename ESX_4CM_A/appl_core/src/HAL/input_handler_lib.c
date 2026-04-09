//-----------------------------------------------------------------------------
/**
 * \file       input_handler_lib.c
 * \brief      HAL - Input Handler Library Implementation
 *
 * \addtogroup HAL
 * @{
 * \addtogroup InputHandlerLib
 * @{
 */

/**
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
 * Dec 8, 2025 STW Technic
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "input_handler_lib.h"
#include "math.h"

#include "dashboard_data_pool.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
sint16 findInputByName(const char *targetName, uint8 *opu8_Index);
sint16 check_inputFaultStatus(uint8 input);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
uint8 u8_numInputs = 0;                         //!<Number of inputs in Input List
T_VehicleInput at_vehicleInputs[X_IN_COUNT];    //!< Array of configured vehicle inputs

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/*! \brief   Initializes Input Handler Parameters

    Initializes all configured vehicle inputs, sets up hardware allocations, and configures diagnostics.

    \param  None
    \retval C_NO_ERR(0)   Input Handler Successfully Initialized
    \retval C_NOACT(-8)   Invalid Input Diagnostic Configuration - Initialization Failed
    \retval C_CONFIG(-10) Invalid Input Configuration - Initialization Failed


    \ingroup system_io_group
*/
sint16 init_inputHandler(void)
{
    sint16 s16_error = C_NO_ERR;
    sint16 s16_initError = C_NO_ERR;
    sint16 s16_diagError = C_NO_ERR;

    //wait for safety core input allocation
    s16_error |= x_in_client_await_allocations(10000u);

    //Initialize Input Runner List
    //loop through all inputs and initialize
    for(uint8 i = 0; i < u8_numInputs; i++)
    {
        // switch case based on intended input type
        switch(at_vehicleInputs[i].e_inputType)
        {
            //TODO_STW: Make the pull circuitry configurable
            //TODO_STW: Make the diagnostic min and max NON configurable but auto detected based off Type and hardware id.
            //TODO_STW: Make the return errors distinguishable between diagnostic init error and hardware init error
            case IT_VOLTAGE:
                s16_initError = x_in_voltage_init( at_vehicleInputs[i].u16_hardwareID, DEFAULT_ADCINPUT_CIRCUIT,  DEFAULT_ADCINPUT_FILTER);

                if(at_vehicleInputs[i].u8_diagEnabled && s16_initError == C_NO_ERR)
                {
                    s16_diagError = x_in_voltage_diag (at_vehicleInputs[i].u16_hardwareID, at_vehicleInputs[i].u16_dti, at_vehicleInputs[i].s32_diagMin, at_vehicleInputs[i].s32_diagMax);
                }
                break;

            case IT_CURRENT:
                s16_initError = x_in_current_init(at_vehicleInputs[i].u16_hardwareID, DEFAULT_ADCINPUT_CIRCUIT, DEFAULT_ADCINPUT_FILTER);

                if(at_vehicleInputs[i].u8_diagEnabled && s16_initError == C_NO_ERR)
                {
                    s16_diagError = x_in_current_diag (at_vehicleInputs[i].u16_hardwareID, at_vehicleInputs[i].u16_dti, at_vehicleInputs[i].s32_diagMin, at_vehicleInputs[i].s32_diagMax);
                }
                break;

            case IT_DIGITAL:
                s16_initError = x_in_digital_init(at_vehicleInputs[i].u16_hardwareID, DEFAULT_DIG_CIRCUIT, X_IN_LOGIC_POSITIVE, 10000);

                if(at_vehicleInputs[i].u8_diagEnabled && s16_initError == C_NO_ERR)
                {
                    s16_diagError = x_in_digital_diag (at_vehicleInputs[i].u16_hardwareID, at_vehicleInputs[i].u16_dti, at_vehicleInputs[i].s32_diagMin, at_vehicleInputs[i].s32_diagMax);
                }
                break;

            case IT_FREQ:

                s16_initError = x_in_frequency_init(at_vehicleInputs[i].u16_hardwareID, DEFAULT_DIG_CIRCUIT, X_IN_LOGIC_POSITIVE, 100);
                if(at_vehicleInputs[i].u8_diagEnabled && s16_initError == C_NO_ERR)
                {
                    s16_diagError = x_in_frequency_diag (at_vehicleInputs[i].u16_hardwareID, at_vehicleInputs[i].u16_dti, (uint32)at_vehicleInputs[i].s32_diagMin, (uint32)at_vehicleInputs[i].s32_diagMax, 100, 9900);
                }
                break;

            default:
                break;
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

**/
sint16 update_inputHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //temporary value variables
    sint32 s32_temp = 0;
    uint8 u8_temp = 0;
    uint32 u32_tempFreq = 0;
    uint32 u32_tempDC = 0;

    // Loop through inputs and read values
    for (uint8 j = 0; j < u8_numInputs; j++)
    {
        //check for any input faults
        if(at_vehicleInputs[j].u8_diagEnabled)
            check_inputFaultStatus(j);

        switch (at_vehicleInputs[j].e_inputType)
        {
            case IT_VOLTAGE:
            {
                x_in_get_voltage_raw(at_vehicleInputs[j].u16_hardwareID, &s32_temp);
                at_vehicleInputs[j].f32_inputValue = (C_NO_ERR == s16_error) ? (float32)s32_temp : at_vehicleInputs[j].f32_prevInputValue;
                break;
            }

            case IT_CURRENT:
            {
                s16_error = x_in_get_current_raw(at_vehicleInputs[j].u16_hardwareID, &s32_temp);
                at_vehicleInputs[j].f32_inputValue = (C_NO_ERR == s16_error) ? (float32)s32_temp : at_vehicleInputs[j].f32_prevInputValue;
                break;
            }

            case IT_DIGITAL:
            {
                s16_error = x_in_get_digital_debounced(at_vehicleInputs[j].u16_hardwareID, &u8_temp);
                at_vehicleInputs[j].f32_inputValue = (C_NO_ERR == s16_error) ? (float32)u8_temp : at_vehicleInputs[j].f32_prevInputValue;
                break;
            }

            case IT_FREQ:
            {
                s16_error = x_in_get_frequency(at_vehicleInputs[j].u16_hardwareID,  &u32_tempFreq, &u32_tempDC);
                at_vehicleInputs[j].f32_inputValue = (C_NO_ERR == s16_error) ? (float32)u32_tempFreq : at_vehicleInputs[j].f32_prevInputValue;
                break;
            }

            default:
                break;
        }

        //check if input value has changed
        at_vehicleInputs[j].mq_inputChanged = (fabsf(at_vehicleInputs[j].f32_inputValue - at_vehicleInputs[j].f32_prevInputValue) > 1e-6f) ? TRUE : FALSE;

        //update previous input value
        at_vehicleInputs[j].f32_prevInputValue = at_vehicleInputs[j].f32_inputValue;

    }

    return s16_error;
}

/** \brief Check the input fault status of an Input

    Function to be called to check if there is a diagnostic fault present on the input requested.

    \internal Task function - called by OS scheduler
**/
sint16 check_inputFaultStatus(uint8 u8_input)
{
    sint16 s16_Error = C_COM;
    uint32 u32_hwInputFault = 0;

    //NOTE: overwriting default C_COM with result of x_in_get_active_faults()
    s16_Error = x_in_get_active_faults(at_vehicleInputs[u8_input].u16_hardwareID, &u32_hwInputFault);


    if (u32_hwInputFault)
    {
        at_vehicleInputs[u8_input].t_fault.u8_fault_status = TRUE;

        // SHORT UB+ / OL
        if (((u32_hwInputFault & X_IN_FAULT_SHORT_TO_UB) == X_IN_FAULT_SHORT_TO_UB))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_SHORT_UB].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_SHORT_UB].u8_is_active = FALSE;

        // SHORT TO GND
        if (((u32_hwInputFault & X_IN_FAULT_SHORT_TO_GND) == X_IN_FAULT_SHORT_TO_GND))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_SHORT_GND].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_SHORT_GND].u8_is_active = FALSE;

        // OPEN LOAD
        if (((u32_hwInputFault & X_IN_FAULT_OPEN_LOAD) == X_IN_FAULT_OPEN_LOAD))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_OL].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_OL].u8_is_active = FALSE;

        // HIGH FREQ
        if (((u32_hwInputFault & X_IN_FAULT_FREQUENCY_UPPER_LIMIT) == X_IN_FAULT_FREQUENCY_UPPER_LIMIT))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_HIGH_FREQ].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_HIGH_FREQ].u8_is_active = FALSE;

        // LOW FREQ
        if (((u32_hwInputFault & X_IN_FAULT_FREQUENCY_LOWER_LIMIT) == X_IN_FAULT_FREQUENCY_LOWER_LIMIT))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_LOW_FREQ].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_LOW_FREQ].u8_is_active = FALSE;

        // LOW DC
        if (((u32_hwInputFault & X_IN_FAULT_DUTY_CYCLE_LOWER_LIMIT) == X_IN_FAULT_DUTY_CYCLE_LOWER_LIMIT))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_LOW_DC].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_LOW_DC].u8_is_active = FALSE;

        // HIGH DC
        if (((u32_hwInputFault & X_IN_FAULT_DUTY_CYCLE_UPPER_LIMIT) == X_IN_FAULT_DUTY_CYCLE_UPPER_LIMIT))
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_HIGH_DC].u8_is_active = TRUE;
        else
            at_vehicleInputs[u8_input].t_fault.t_fmi[e_INFAULT_HIGH_DC].u8_is_active = FALSE;
    }

    else if (s16_Error == C_NO_ERR && !u32_hwInputFault) //!< If no errors and no returned faults: set fault fields to FALSE
    {
        at_vehicleInputs[u8_input].t_fault.u8_fault_status = FALSE;

        for (uint8 i = 0; i < e_NUM_INFAULTS; i++)
        {
            at_vehicleInputs[u8_input].t_fault.t_fmi[i].u8_is_active = FALSE;
        }
    }

    return s16_Error;
}


//Getter Functions  ------------------------------------------------------------------------

/** \brief Get the recorded input value for a specific input

    \param[out] opf32_value Input Value

    \return Error Return Value
    \retval C_NO_ERR(0) No Error
    \retval C_RANGE(-5) Input Not Found
**/
sint16 get_inputValue(const char *targetName, float32 *opf32_value)
{
    sint16 s16_error;
    uint8 u8_index = 0;

    s16_error = findInputByName(targetName, &u8_index);

    if (C_NO_ERR == s16_error)
        *opf32_value = at_vehicleInputs[u8_index].f32_inputValue;
    else
        *opf32_value = 0xFFFFFFFFFFFFFFFF;

    return s16_error;
}


/** \brief Get the number of initialized Inputs

    \param[out] opu16_Count Current Inputs

    \return Error Return Value
    \retval C_NO_ERR(0) No Error
**/
sint16 get_numInputs(uint8 *const opu8_Count)
{
    *opu8_Count = u8_numInputs;
    return C_NO_ERR;
}

/** \brief Get the input fault status for a specific input

    \param[out] opu8_status Fault status of input

    \return Error Return Value
    \retval C_NO_ERR(0) No Error
    \retval C_RANGE(-5) Input Not Found
**/
sint16 get_inputFaultStatus(const char *targetName, uint8 *opu8_status)
{
    sint16 s16_error;
    uint8 u8_index = 0;

    s16_error = findInputByName(targetName, &u8_index);

    if (C_NO_ERR == s16_error)
        *opu8_status = at_vehicleInputs[u8_index].t_fault.u8_fault_status;
    else
        *opu8_status = 255;

    return s16_error;
}

// Helper Functions ------------------------------------------------------------------------

/** \brief Add a hardware input to the input list
    Adds the specified input configuration to the global output array
    and increments the input counter

    \param[in] input Hardware Input configuration structure to add

    \return Error Return Value
    \retval C_NO_ERR(0)         Input added to list
    \retval C_RANGE(-5)         Failed, input list already full
    \retval C_CONFIG(-10)       Failed, input pin already configured

    \ingroup vehicleIO_funcs
 */
sint16 add_hwInput(T_VehicleInput input)
{
    sint16 s16_Error = C_NO_ERR;

    if (u8_numInputs == X_IN_COUNT)
    {
        s16_Error = C_RANGE;
    }
    else
    {
        for (uint8 i = 0; i < u8_numInputs; i++)
        {
            if (at_vehicleInputs[i].u16_hardwareID == input.u16_hardwareID)
            {
                s16_Error = C_CONFIG;
                break;
            }
        }

        // (3) Complete add operation if all checks above pass - C_NO_ERR
        if (s16_Error == C_NO_ERR)
        {
            at_vehicleInputs[u8_numInputs] = input;
            u8_numInputs++;
        }
    }
    return s16_Error;
}

/** \brief Find input by name
    Searches the input array for an input with the specified name

    \param[in] targetName Name of the input to find
    \param[out] 0-254 Valid input index

    \return Error Return Value
    \retval C_NO_ERR(0)  Input found
    \retval C_RANGE(-5)  Input not found
**/
sint16 findInputByName(const char *targetName, uint8 *opu8_Index)
{
    for (uint8 i = 0; i < u8_numInputs; i++)
    {
        if (strcmp(at_vehicleInputs[i].Name_Description, targetName) == 0)
        {
            *opu8_Index = i;  // Return index of matching item
            return C_NO_ERR;
        }
    }

    *opu8_Index = 255;  // Not found
    return C_RANGE;
}

/** \brief Clear all active input faults
    Searches the input array for any input that has an active fault and sets
    it to FALSE

    \return Error Return Value
    \retval C_NO_ERR(0)  All Input Faults Reset
**/
sint16 clear_inputFaults(void)
{
    sint16 s16_error = C_NO_ERR;

    for (uint8 i = 0; i < u8_numInputs; i++)
    {
        if (at_vehicleInputs[i].u8_diagEnabled)
        {
            at_vehicleInputs[i].t_fault.u8_fault_status = FALSE;
            for(uint8 j = 0; j< MAX_NUM_FMI; j++)
            {
                at_vehicleInputs[i].t_fault.t_fmi[j].u8_is_active = FALSE;
            }
        }
    }

    return s16_error;
}


//EOF

//-----------------------------------------------------------------------------
/**
 * \file       fault_handler.c
 * \brief      AgvDiag - Fault Handler
 *
 * \addtogroup AgvDiag
 * @{
 * \addtogroup FaultHandler Fault Handler
 *
 * The Fault Handler manages the detection, logging, and state management of
 * system-wide faults and errors. It interfaces with the diagnostic systems
 * to ensure the machine enters safe operational states when critical
 * issues arise and provides data for troubleshooting.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 20, 2026 kyle.boch
 *
 * @{
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
#include "stwtypes.h"
#include "stwerrors.h"
//PROJECT
#include "fault_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/**
 * \brief Logic fault definition for the Elevator system.
 * This structure defines the J1939 diagnostic parameters (SPN/FMI) for
 * elevator-related logic faults, specifically handling current/voltage
 * monitoring via FMI 5 and 6.
 */
T_FloryFault elevatorLogicFault1 =
{
    .u8_dm1_enable = TRUE,
    .u8_fault_status = FALSE,
    .u32_spn = 520999,
    .t_fmi = {
        [0] = {.u8_is_active = FALSE, .u8_fmi_value = 5},
        [1] = {.u8_is_active = FALSE, .u8_fmi_value = 6}
    }
};

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/**
 * \brief Initializes the system fault handling and alarm management.
 *
 * This function registers logic-based faults (such as elevator faults) into the
 * global fault list and initializes the J1939 DM1 alarm handler for
 * diagnostic reporting.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_faultHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //add logic faults to the logic fault list
    add_logicFault(&elevatorLogicFault1);

    //initialize DM1 Alarm Handler
    init_alarmHandler();

    return s16_error;
}

/**
 * \brief Updates the active state of diagnostic alarms and faults.
 *
 * This function cyclically updates the DM1 alarm handler to process
 * active diagnostic trouble codes (DTCs) and maintain J1939 communication status.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 update_faultHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //Update DM1 Alarm Handler
    update_alarmHandler();

    return s16_error;
}

/**
 * \brief Resets all active faults and diagnostic counters across the machine.
 *
 * This function performs a comprehensive clear of all input, output, and
 * logic-based faults. Additionally, it resets the J1939 DM1 occurrence
 * counters and clears active dashboard lamps.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 clear_machineFaults(void)
{
    sint16 s16_error = C_NO_ERR;

    clear_inputFaults();
    clear_logicFaults();
    clear_outputFaults();

    clear_dm1OccurCounts();
    clear_dm1Lamps();

    return s16_error;
}

//EOF

//-----------------------------------------------------------------------------
/*! \file       fault_handler.c
    \brief      <description>

    project     Flory_8772-4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 02, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "stwtypes.h"
#include "stwerrors.h"
#include "fault_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */



/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_FloryFault elevatorLogicFault1;    //!<Elevator Control Logic Fault 1

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
sint16 init_faultHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //add logic alarms to the logic fault list
    add_logicFault(&elevatorLogicFault1);

    //register logic faults to dm1s if required
    add_dm1LogicAlarm(&elevatorLogicFault1);

    //(all input and ouput DM1 are auto detected)
    //initialize DM1 Alarm Handler
    init_alarmHandler();

    return s16_error;
}

sint16 update_faultHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //Update DM1 Alarm Handler
    update_alarmHandler();

    return s16_error;
}



sint16 clear_machineFaults(void)
{
    sint16 s16_error = C_NO_ERR;

    clear_inputFaults();
    clear_logicFaults();
    clear_outputFaults();
    clear_dm1Lamps();

    return s16_error;
}





//EOF

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
#include "alarm_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */

/* -- Types -------------------------------------------------------------------------------------------------------- */



/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_FloryFault elevatorLogicFault1;    //!<Elevator Control Logic Fault 1

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
sint16 init_faultHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //parse through logic faults and register DM1s
    add_dm1LogicAlarm(&elevatorLogicFault1);

    //(all input and ouput DM1 are auto detected by init_alarmHandler())
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

sint16 set_logicFaultStatus(uint32 u32_spn, uint16 u16_fmi, uint8 u8_state)
{
    sint16 s16_error = C_NO_ERR;


    return s16_error;
}

/*
sint16 clear_machineFaults(void);
{
    sint16 s16_error = C_NO_ERR;

    clear_inputFaults();
    clear_logicFaults();
    clear_outputFaults();

    return s16_error;
}
*/




//EOF

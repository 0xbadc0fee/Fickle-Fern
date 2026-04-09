//-----------------------------------------------------------------------------
/**
 * \file       alarm_handler_lib.c
 * \brief      System - Alarm Handler Library
 *
 * \addtogroup HAL
 * @{
 * \addtogroup AlarmHandler Alarm Handler
 *
 * The Alarm Handler Library manages the detection, prioritization, and
 * reporting of system-wide faults. It maintains an active alarm list and
 * coordinates diagnostic information for the operator display and
 * logging systems.
  *
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
 * Jan 6, 2026 STW Technic
 *
 * @{
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
#include "stwtypes.h"
#include "stwerrors.h"
#include "alarm_handler_lib.h"
#include "output_handler_lib.h"
#include "input_handler_lib.h"
#include "osy_com_j1939_dm1.h"
#include "osy_com_j1939_dm2.h"
#include "x_can.h"

#include "nvm_handler_lib.h"

//PROJECT

#include "dashboard_data_pool.h"


/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
void add_J1939dtc(T_FloryFault *_dtc);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_FaultNVM mat_nvmDtcs[445];


static uint16 u16_num_input_faults = 0;
static uint16 u16_num_output_faults = 0;
static uint16 u16_num_dtcs = 0;

static uint8 u8_num_logic_faults = 0;
static T_FloryFault mat_logic_faults[MAX_LOGIC_FAULTS];

static uint8 u8_num_outputs = 0;
static uint8 u8_num_inputs = 0;

static T_osy_com_j1939_dm_lamp_status mt_dm1_lamps;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/*!
   \brief  Initialize Alarm Handler

    This task is used to initialize all the required DM1 Alarms
    The alarm handler handles 3 different types of alarms:
    1. Output Alarms
    2. Input Alarms
    3. Logic Alarms

    The Funciton loops through a list of each input and output and checks to see if DM1s are enabled.  If they are
    the input's faults get added to the DTC list.
    This function loops through all registered Logic Faults and adds corresponding DTCs to the DTC list.

*/
sint16 init_alarmHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    //Get the number of inputs and outputs from accessors
    get_numOutputs(&u8_num_outputs);
    get_numInputs(&u8_num_inputs);

    //DTCs for Outputs
    for(uint8 i=0; i<u8_num_outputs; i++)
    {
        //check if diagnostics are wanted / enabled
        if(at_vehicleOutputs[i].t_fault.u8_dm1_enable)
        {
            //add fault to DTC runner list
            add_J1939dtc(&at_vehicleOutputs[i].t_fault);
            u16_num_output_faults++;
        }
    }

    //DTCs for Inputs
    for(uint8 i=0; i<u8_num_inputs; i++)
    {
        //check if J1939 DM1s are wanted / enabled
        if(at_vehicleInputs[i].t_fault.u8_dm1_enable)
        {
            //add fault to DTC runner list
            add_J1939dtc(&at_vehicleInputs[i].t_fault);
            u16_num_input_faults++;
        }
    }

    //DTCs for Logic Faults
    for(uint8 i=0; i< u8_num_logic_faults; i++)
    {
        if(mat_logic_faults[i].u8_dm1_enable)
        {
            //add fault to DTC runner list
            add_J1939dtc(&mat_logic_faults[i]);
        }
    }

    return s16_error;
}

/*!
   \brief  Update Alarm Handler

    This task is used to populate alarm messages based on the status of each type or possible alarm.
    The alarm handler handles 3 different types of alarms:
    1. Output Alarms
    2. Input Alarms
    3. Logic Alarms

    The Funciton loops through a list of each fault, checks the status as set by the applicaiton or Input
    /Output Handler then triggers the respective DM1 if the fault is detected to be active.

*/
sint16 update_alarmHandler(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_alarmFound = FALSE;

    osy_com_j1939_dm1_lock_tx(X_CAN_BUS_01, 0);

    //Set Lamps
    osy_j1939_set_lamps(mt_dm1_lamps);

    for(uint16 faults = 0; faults < u16_num_dtcs; faults++)
    {

        if(!u8_alarmFound)
        {
            //set active flags for output alarms
            for(uint16 k = 0; k<u8_num_outputs; k++)
            {
                if(at_vehicleOutputs[k].t_fault.u8_dm1_enable)
                {
                    for(uint8 p=0; p<MAX_NUM_FMI; p++)
                    {
                        //check if current dtc being checked
                        if(gat_DmDtcs[faults].u32_Spn == at_vehicleOutputs[k].t_fault.u32_spn &&
                           gat_DmDtcs[faults].u8_Fmi  == at_vehicleOutputs[k].t_fault.t_fmi[p].u8_fmi_value &&
                           at_vehicleOutputs[k].t_fault.t_fmi[p].u8_fmi_value != 0)
                        {
                            u8_alarmFound = TRUE;
                            gat_DmDtcs[faults].u8_IsActive = (at_vehicleOutputs[k].t_fault.t_fmi[p].u8_is_active) ? TRUE : FALSE;

                            if(at_vehicleOutputs[k].t_fault.t_fmi[p].u8_is_active && !at_vehicleOutputs[k].t_fault.t_fmi[p].u8_prev_active)
                            {
                                if (gat_DmDtcs[faults].u8_OccurrenceCounter < 126)
                                {
                                    gat_DmDtcs[faults].u8_OccurrenceCounter++;
                                    fault_nvm_write(&mat_nvmDtcs[faults], gat_DmDtcs[faults].u8_OccurrenceCounter);
                                }

                            }
                            at_vehicleOutputs[k].t_fault.t_fmi[p].u8_prev_active = at_vehicleOutputs[k].t_fault.t_fmi[p].u8_is_active;

                        }
                    }
                }
            }
        }

        if(!u8_alarmFound)
        {
            //set active flags for output alarms
            for(uint16 k = 0; k<u8_num_outputs; k++)
            {
                if(at_vehicleInputs[k].t_fault.u8_dm1_enable)
                {
                    for(uint8 p=0; p<MAX_NUM_FMI; p++)
                    {
                        //check if current dtc being checked
                        if(gat_DmDtcs[faults].u32_Spn == at_vehicleInputs[k].t_fault.u32_spn &&
                           gat_DmDtcs[faults].u8_Fmi  == at_vehicleInputs[k].t_fault.t_fmi[p].u8_fmi_value &&
                           at_vehicleInputs[k].t_fault.t_fmi[p].u8_fmi_value != 0)
                        {
                            u8_alarmFound = TRUE;
                            gat_DmDtcs[faults].u8_IsActive = (at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active) ? TRUE : FALSE;

                            if(at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active && !at_vehicleInputs[k].t_fault.t_fmi[p].u8_prev_active)
                            {
                                if (gat_DmDtcs[faults].u8_OccurrenceCounter < 126)
                                {
                                    gat_DmDtcs[faults].u8_OccurrenceCounter++;
                                    fault_nvm_write(&mat_nvmDtcs[faults], gat_DmDtcs[faults].u8_OccurrenceCounter);
                                }

                            }
                            at_vehicleInputs[k].t_fault.t_fmi[p].u8_prev_active = at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active;
                        }
                    }
                }
            }
        }


        if(!u8_alarmFound)
        {
            //set active flags for logic alarms
            for(uint16 k = 0; k<u8_num_logic_faults; k++)
            {
                for(uint8 p=0; p<MAX_NUM_FMI; p++)
                {
                    //check if current dtc being checked
                    if(gat_DmDtcs[faults].u32_Spn == mat_logic_faults[k].u32_spn &&
                       gat_DmDtcs[faults].u8_Fmi  == mat_logic_faults[k].t_fmi[p].u8_fmi_value &&
                       mat_logic_faults[k].t_fmi[p].u8_fmi_value != 0)
                    {
                        u8_alarmFound = TRUE;
                        gat_DmDtcs[faults].u8_IsActive = (mat_logic_faults[k].t_fmi[p].u8_is_active) ? TRUE : FALSE;

                        if(mat_logic_faults[k].t_fmi[p].u8_is_active && !mat_logic_faults[k].t_fmi[p].u8_prev_active)
                        {
                            if (gat_DmDtcs[faults].u8_OccurrenceCounter < 126)
                            {
                                gat_DmDtcs[faults].u8_OccurrenceCounter++;
                                fault_nvm_write(&mat_nvmDtcs[faults], gat_DmDtcs[faults].u8_OccurrenceCounter);
                            }

                        }
                        mat_logic_faults[k].t_fmi[p].u8_prev_active = mat_logic_faults[k].t_fmi[p].u8_is_active;

                    }
                }
            }
        }

        //reset alarm found flag
        u8_alarmFound = FALSE;
    }



    osy_com_j1939_dm1_unlock_tx(X_CAN_BUS_01, 0);


    return s16_error;
}

/**
 * \brief Adds a Fault to the list of all J1939 DTCs.
 *
 * Takes a passed Machine Fault and adds it to the J1939 DTC runner list.
 *
 * \param[in] _dtc  Pointer to the Flory Defined Fault Structure.
 */
void add_J1939dtc(T_FloryFault *_dtc)
{

    for(uint8 i=0; i<MAX_NUM_FMI; i++)
    {
        if(_dtc->t_fmi[i].u8_fmi_value != 0)
        {
            fault_nvm_init(&mat_nvmDtcs[u16_num_dtcs], _dtc->u32_spn, _dtc->t_fmi[i].u8_fmi_value, 0);

            gat_DmDtcs[u16_num_dtcs].u32_Spn = _dtc->u32_spn;
            gat_DmDtcs[u16_num_dtcs].u8_Fmi  = _dtc->t_fmi[i].u8_fmi_value;
            gat_DmDtcs[u16_num_dtcs].u8_IsActive = _dtc->t_fmi[i].u8_is_active;
            gat_DmDtcs[u16_num_dtcs].u8_SpnConvMode = 0;

            gat_DmDtcs[u16_num_dtcs].u8_OccurrenceCounter = mat_nvmDtcs[u16_num_dtcs].u8_occurenceCount;

            u16_num_dtcs++;
        }
    }
}


/*!
   \brief  Add Logic Fault to List of Logic Faults

   \param[in] _dtc Fault to add to fault list

*/
void add_logicFault(T_FloryFault *_dtc)
{

    mat_logic_faults[u8_num_logic_faults] = *_dtc;
    u8_num_logic_faults++;
}

/*!
   \brief  Set the status of all the DM1 Alarm Lamps

   Individually set the status of each DM1 Lamp

   \param[in] E_LampID _lamp The lamp ID that is to be set
   \param[in] _state State of the Lamp (TRUE = ON, FALSE = OFF)

*/
sint16 set_dm1Lamp(E_LampID _lamp, uint8 _state)
{
    sint16 s16_error = C_NO_ERR;

    switch(_lamp)
    {
        case e_AMBER_WARN:
            mt_dm1_lamps.u8_AmberWarnLamp = _state;
            break;
        case e_AMBER_FLASH:
            mt_dm1_lamps.u8_FlashAmberWarnLamp = _state;
            break;
        case e_MALF_IND:
            mt_dm1_lamps.u8_MalfIndLamp = _state;
            break;
        case e_MALF_FLASH:
            mt_dm1_lamps.u8_FlashMalfIndLamp = _state;
            break;
        case e_PROTECT:
            mt_dm1_lamps.u8_ProtectLamp = _state;
            break;
        case e_PROTECT_FLASH:
            mt_dm1_lamps.u8_FlashProtectLamp = _state;
            break;
        case e_RED_STOP:
            mt_dm1_lamps.u8_RedStopLamp = _state;
            break;
        case e_RED_STOP_FLASH:
            mt_dm1_lamps.u8_FlashRedStopLamp = _state;
            break;
        default:
            s16_error = -1;
            break;
    }

    return s16_error;
}

/*!
   \brief  Clear the status of all the DM1 Alarm Lamps

*/
sint16 clear_dm1Lamps(void)
{
    sint16 s16_error = C_NO_ERR;

    mt_dm1_lamps.u8_AmberWarnLamp = FALSE;
    mt_dm1_lamps.u8_FlashAmberWarnLamp = FALSE;
    mt_dm1_lamps.u8_MalfIndLamp = FALSE;
    mt_dm1_lamps.u8_FlashMalfIndLamp = FALSE;
    mt_dm1_lamps.u8_ProtectLamp = FALSE;
    mt_dm1_lamps.u8_FlashProtectLamp = FALSE;
    mt_dm1_lamps.u8_RedStopLamp = FALSE;
    mt_dm1_lamps.u8_FlashRedStopLamp = FALSE;
    return s16_error;
}

/*!
   \brief  Set all J1939 Occurence Counts to 0

*/
sint16 clear_dm1OccurCounts(void)
{
    sint16 s16_error = C_NO_ERR;

    //loop through all DTCs and set NVM and RAM Occurence Counters to 0
    for(uint16 faults = 0; faults < u16_num_dtcs; faults++)
    {
        gat_DmDtcs[faults].u8_OccurrenceCounter = 0;
        fault_nvm_write(&mat_nvmDtcs[faults], 0);
    }

    return s16_error;
}

/*!
   \brief  Set the Fault/Alarm status of a Logic Fault

    Loop through all faults and alarms and clear any "Is Active" status to FALSE

    \param[in] u32_spn SPN of the Fault that is to be set
    \param[in] u16_fmi FMI of the Fault that is to be set
    \param[in] u8_state State of the Fault that is to be set

    \retval C_NO_ERR Fault status set successfully
    \retval C_RANGE SPN Not Found
    \retval C_NOACT FMI Not found for passed SPN

*/
sint16 set_logicFaultStatus(uint32 u32_spn, uint16 u16_fmi, uint8 u8_state)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u8_spn_found = FALSE;
    uint8 u8_fmi_found = FALSE;

    //Update the fault list
    for(uint8 i = 0;i<u8_num_logic_faults; i++)
    {
        if(u32_spn == mat_logic_faults[i].u32_spn && !u8_spn_found)
        {
            u8_spn_found = TRUE;

            for(uint8 k = 0; k<MAX_NUM_FMI; k++)
            {
                if(u16_fmi == mat_logic_faults[i].t_fmi[k].u8_fmi_value && !u8_fmi_found)
                {
                    u8_fmi_found = TRUE;
                    mat_logic_faults[i].t_fmi[k].u8_is_active = u8_state;
                }
            }
        }
    }

    //return the correct return code
    if(!u8_spn_found)
        s16_error = C_RANGE;
    else if(!u8_fmi_found)
        s16_error = C_NOACT;

    return s16_error;
}

/*!
   \brief  Clear all Logic Faults and Alarms

    Loop through all faults and alarms and clear any "Is Active" status to FALSE

*/
sint16 clear_logicFaults(void)
{
    sint16 s16_error = C_NO_ERR;

    //Clear the fault list
    for(uint8 i = 0;i<u8_num_logic_faults; i++)
    {
        for(uint8 k = 0; k<MAX_NUM_FMI; k++)
        {
            mat_logic_faults[i].t_fmi[k].u8_is_active = FALSE;
        }
    }

    return s16_error;
}

//EOF

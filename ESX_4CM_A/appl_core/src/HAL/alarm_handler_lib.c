//-----------------------------------------------------------------------------
/*! \file       alarm_handler_lib.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 STW Technic
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
#include "x_can.h"
//PROJECT



/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
void add_J1939dtc(T_FloryFault *_dtc);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */

static uint16 u16_num_input_dtcs = 0;
static uint16 u16_num_output_dtcs = 0;
static uint16 u16_num_logic_dtcs = 0;

static uint16 u16_num_dtcs = 0;

static uint8 u8_num_logic_faults = 0;
static T_FloryFault mat_logic_faults[MAX_LOGIC_FAULTS];

static uint8 u8_num_outputs = 0;
static uint8 u8_num_inputs = 0;

static T_osy_com_j1939_dm_lamp_status mt_dm1_lamps;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
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
            u16_num_output_dtcs+=10;
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
            u16_num_input_dtcs+=10;
        }
    }

    //DTCs for Logic Faults
    for(uint8 i=0; i< u8_num_logic_faults; i++)
    {
        add_J1939dtc(&mat_logic_faults[i]);
        u16_num_logic_dtcs+=10;
    }

    return s16_error;
}


sint16 update_alarmHandler(void)
{
    sint16 s16_error = C_NO_ERR;

    osy_com_j1939_dm1_lock_tx(X_CAN_BUS_01, 0);

    //Set Lamps
    osy_j1939_set_lamps(mt_dm1_lamps);

    //set is active flags for outputs
    for(uint16 k = 0; k<u8_num_outputs; k++)
    {
        if(gat_DmDtcs[u16_num_dtcs].u32_Spn == at_vehicleOutputs[k].t_fault.u32_spn)
        {
            for(uint8 p=0; p<MAX_NUM_FMI; p++)
            {
                if(at_vehicleOutputs[k].t_fault.t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p].u8_IsActive = TRUE;
                else if (!at_vehicleOutputs[k].t_fault.t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p].u8_IsActive = FALSE;
            }
        }
    }

    //set is active flags for inputs
    for(uint16 k = 0; k<u8_num_inputs; k++)
    {
        if(gat_DmDtcs[u16_num_dtcs].u32_Spn == at_vehicleInputs[k].t_fault.u32_spn)
        {
            for(uint8 p=0; p<MAX_NUM_FMI; p++)
            {
                if(at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p+u16_num_output_dtcs].u8_IsActive = TRUE;
                else if (!at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p+u16_num_output_dtcs].u8_IsActive = FALSE;
            }
        }
    }

    //set active flags for logic alarms
    for(uint8 k = 0; k<u8_num_logic_faults; k++)
    {
        if(gat_DmDtcs[u16_num_dtcs].u32_Spn == mat_logic_faults[k].u32_spn)
        {
            for(uint8 p=0; p<MAX_NUM_FMI; p++)
            {
                if(mat_logic_faults[k].t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p+u16_num_output_dtcs+u16_num_input_dtcs].u8_IsActive = TRUE;
                else if (!at_vehicleInputs[k].t_fault.t_fmi[p].u8_is_active)
                    gat_DmDtcs[(k*MAX_NUM_FMI)+p+u16_num_output_dtcs+u16_num_input_dtcs].u8_IsActive = FALSE;
            }
        }
    }

    osy_com_j1939_dm1_unlock_tx(X_CAN_BUS_01, 0);


    return s16_error;
}


/*!
   \brief  Add a J1939 DTC to list of all J1939 DTCs

    Take a passed J1939 DTC and add it to the J1939 DTC runner list

   \param    T_J1939_DM_tx_dtc _dtc   DTC TX

*/
void add_J1939dtc(T_FloryFault *_dtc)
{
    for(uint8 i=0; i<MAX_NUM_FMI; i++)
    {
        gat_DmDtcs[u16_num_dtcs].u32_Spn = _dtc->u32_spn;
        gat_DmDtcs[u16_num_dtcs].u8_Fmi  = _dtc->t_fmi[i].u8_fmi_value;
        gat_DmDtcs[u16_num_dtcs].u8_IsActive = _dtc->t_fmi[i].u8_is_active;
        gat_DmDtcs[u16_num_dtcs].u8_OccurrenceCounter = 0;
        gat_DmDtcs[u16_num_dtcs].u8_SpnConvMode = 0;
        u16_num_dtcs++;
    }
}

void add_dm1LogicAlarm(T_FloryFault *_dtc)
{
    memcpy(&mat_logic_faults[u8_num_logic_faults], &_dtc, sizeof(mat_logic_faults[u8_num_logic_faults]));
    u8_num_logic_faults++;
}


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
//EOF

//-----------------------------------------------------------------------------
/*! \file       nvm_handler.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 7, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW

//PROJECT
#include "nvm_handler.h"

//Include OSY NVM Datapool headers
#include "configuration_data_pool.h"

//Include Controls that have checkpoints
#include "elevator_control.h"

//Include SPNS (current location for DP Assignment MACRO)
#include "SPN_definitions.h"


/* -- Defines ------------------------------------------------------------------------------------------------------ */
#define NUM_OSY_NVM_DATAPOOLS (1u)
/* -- Types -------------------------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
T_Config_Elevator gt_elevatorConfig;    //!<Structure that holds all agVWork - Elevator Control NVM Config

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
sint16 init_nvmParameters(void)
{

    sint16 s16_error = C_NO_ERR;

    //Initialize all openSYDE NVM Lists for each datapool
    init_osyNVMDatapools(CONFIGURATION_DATA_POOL_INDEX, CONFIGURATION_NUMBER_OF_LISTS);

    //populate all control nvm structs with globals
    #define NVM_FIELD(name, DPL_LIST, DPL_VALUE, CNTRL_VALUE) DP_ASSIGN((CNTRL_VALUE), (DPL_VALUE));
    #include "nvm_map.def"
    #undef NVM_FIELD

    return s16_error;
}

/** \brief Write NVM Parameters into NVM
 *
 *  Check to see if the RAM copy of the NVM parameter does not match the OSY version.
 *  If the RAM/Control copy is different than the OSY, write the corresponding OSY List
 *  to NVM.
 *
 * @return
 */
sint16 write_nvmParameters(void)
{
    sint16 s16_error = C_NO_ERR;
    uint8 u32_syncStatus = 0;

    //Update all datapool varialbes with control struct variables
    #define NVM_FIELD(name, DPL_LIST, DPL_VALUE, CNTRL_VALUE) \
        NVM_SYNC_ITEM((DPL_LIST), (DPL_VALUE), (CNTRL_VALUE), u32_syncStatus);
    #include "nvm_map.def"
    #undef NVM_FIELD

    //loop through all lists in the NVM Configuration Datapool
    for(uint8 i=0; i<CONFIGURATION_NUMBER_OF_LISTS; i++)
    {
        //if the nvm is out of sync with ram, write list to NVM
        if (u32_syncStatus & NVM_LIST_BIT(i))
        {
            s16_error = write_osyNVMDatapool(CONFIGURATION_DATA_POOL_INDEX, i);
        }
    }


    return s16_error;
}

/** \brief Set the NVM Parameters to their default values
 *
 * Reset all NVM parameters to their default values (stored and
 * defined in openSYDE dataset 1.
 *
 * It is assumed that every nvm list defined in openSYDE has a corresponding default
 * list defined.
 *
 * @return
 */
sint16 reset_nvmParameters(void)
{
    sint16 s16_error = C_NO_ERR;
    const uint8 u8_defaultDataset = 0;

    for(uint8 i = 0; i< CONFIGURATION_NUMBER_OF_LISTS; i++)
    {
        s16_error |= apply_osyNVMDataset(CONFIGURATION_DATA_POOL_INDEX, i, u8_defaultDataset);
    }

    return s16_error;
}


//EOF

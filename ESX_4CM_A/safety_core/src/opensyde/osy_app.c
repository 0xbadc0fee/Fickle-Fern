//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Application Initialization
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- (Module) Global Variables ------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about new value written to element.         */
//-----------------------------------------------------------------------------
void osy_app_write_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex, const uint16 ou16_ElementIndex)
{
}

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about attempt to read from element.         */
//-----------------------------------------------------------------------------
sint16 osy_app_read_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex, const uint16 ou16_ElementIndex)
{
   return -1;
}

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about attempt to write to element.          */
//-----------------------------------------------------------------------------
sint16 osy_app_pre_write_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                               const uint16 ou16_ElementIndex, const void * const opv_Data,
                               const uint16 ou16_DataSizeBytes)
{
   return -1;
}

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about changed NVM content.                  */
//-----------------------------------------------------------------------------
sint16 osy_app_nvm_data_was_changed_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex)
{
   return -1;
}

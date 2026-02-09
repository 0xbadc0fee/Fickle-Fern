//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Application Handler-Functions
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "osy_dpa_application_event_handler_if.h"
#include "osy_init.h"

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
   return C_NO_ERR;
}

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about attempt to write to element.          */
//-----------------------------------------------------------------------------
sint16 osy_app_pre_write_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                               const uint16 ou16_ElementIndex, const void * const opv_Data,
                               const uint16 ou16_DataSizeBytes)
{
   return C_NO_ERR;
}

//-----------------------------------------------------------------------------
/*! \brief   Handle notification about changed NVM content.                  */
//-----------------------------------------------------------------------------
sint16 osy_app_nvm_data_was_changed_event(const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex)
{
   return C_NO_ERR;
}

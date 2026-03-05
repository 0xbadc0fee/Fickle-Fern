//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       openSYDE example application: j1939 stack (Header file with interface)
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_J1939H
#define OSY_J1939H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_com_j1939_dm_generic.h"
#include "osy_com_j1939_dm1.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef enum {
    e_OSY_AMBER_WARN = 0,
    e_OSY_AMBER_FLASH,
    e_OSY_MALF_IND,
    e_OSY_MALF_FLASH,
    e_OSY_PROTECT,
    e_OSY_PROTECT_FLASH,
    e_OSY_RED_STOP,
    e_OSY_RED_STOP_FLASH,
} E_OSY_LAMP_ID;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_osy_com_j1939_dtc gat_DmDtcs[445];

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 osy_app_j1939_init(void);
sint16 osy_app_j1939_cycle(void);
void osy_app_j1939_restart_reception(void);
sint16 osy_j1939_set_lamps(T_osy_com_j1939_dm_lamp_status _lampID);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // OSY_J1939H

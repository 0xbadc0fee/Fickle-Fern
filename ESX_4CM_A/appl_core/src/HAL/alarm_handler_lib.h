/*! \file       alarm_handler_lib.h.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 5, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_HAL_ALARM_HANDLER_LIB_H_
#define APPL_CORE_SRC_HAL_ALARM_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "osy_dph_nvm_handler.h"
#include "osy_app_j1939.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define MAX_LOGIC_FAULTS 200
#define MAX_NUM_FMI 10
/* -- Types --------------------------------------------------------------------------------------------------------- */
/*! \brief List of all DM1 Lamp Types **/
typedef enum {
    e_AMBER_WARN = 0,
    e_AMBER_FLASH,
    e_MALF_IND,
    e_MALF_FLASH,
    e_PROTECT,
    e_PROTECT_FLASH,
    e_RED_STOP,
    e_RED_STOP_FLASH,
    e_OSY_NUM_LAMPS
} E_LampID;

typedef struct
{
    uint8 u8_is_active;
    uint8 u8_prev_active;
    uint8 u8_fmi_value;
}T_FMI;

typedef struct
{
    uint8  u8_dm1_enable;
    uint8  u8_fault_status;
    uint32 u32_spn;
    T_FMI  t_fmi[MAX_NUM_FMI];
}T_FloryFault;


/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_alarmHandler(void);
sint16 update_alarmHandler(void);

void add_logicFault(T_FloryFault *_dtc);
sint16 set_logicFaultStatus(uint32 u32_spn, uint16 u16_fmi, uint8 u8_state);

sint16 clear_dm1Lamps(void);
sint16 clear_logicFaults(void);
sint16 clear_dm1OccurCounts(void);

sint16 set_dm1Lamp(E_LampID _lamp, uint8 _state);

#endif /* APPL_CORE_SRC_HAL_ALARM_HANDLER_LIB_H_ */


//-----------------------------------------------------------------------------
/**
 * \file       alarm_handler_lib.h
 * \brief      HAL - Alarm Handler Library
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
#ifndef APPL_CORE_SRC_HAL_ALARM_HANDLER_LIB_H_
#define APPL_CORE_SRC_HAL_ALARM_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "osy_dph_nvm_handler.h"
#include "osy_app_j1939.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define MAX_LOGIC_FAULTS                (200u)      //!< Maximum number of tracked logic faults in the system
#define MAX_NUM_FMI                     (10u)       //!< Maximum number of FMIs allowed per Suspect Parameter Number (SPN)
/* -- Types --------------------------------------------------------------------------------------------------------- */
/**
 * \enum E_LampID
 * \brief List of all J1939 DM1 Lamp Types and flash states.
 */
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

/**
 * \struct T_FMI
 * \brief Failure Mode Identifier (FMI) status tracking.
 * * This structure tracks the individual active states and history
 * of a specific FMI associated with an SPN.
 */
typedef struct
{
    uint8 u8_is_active;
    uint8 u8_prev_active;
    uint8 u8_fmi_value;
}T_FMI;

/**
 * \struct T_FloryFault
 * \brief Global Fault Definition Structure.
 * * This structure contains all necessary J1939 parameters to report
 * a fault via DM1, including the SPN and a list of possible FMIs.
 */
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


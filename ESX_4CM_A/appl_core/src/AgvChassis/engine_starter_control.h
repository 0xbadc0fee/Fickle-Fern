//-----------------------------------------------------------------------------
/*! \file       engine_starter_control.h
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 6, 2026 Tiffany.Gohnert
 */

#ifndef APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_


/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "hmi_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define ENGINE_START_CMD_OFF       (0u)
#define ENGINE_START_CMD_ON        (1u)

#define ENGINE_OFF       (1u)
#define ENGINE_ON        (0u)

#define NEUTRAL_SAFE_FALSE         (0u)
#define NEUTRAL_SAFE_TRUE          (1u)

#define ENGINE_START_SAFE_OUTPUT   (0u)
/* -- Types --------------------------------------------------------------------------------------------------------- */

/** \brief Checkpoints Structure - Engine Starter Control
 *
 * This structure represents all checkpoints that are relevant
 * to shaft drive control
 */
typedef struct
{
        uint8 u8_eng_off;//!<Engine Off Indicator
        uint8 u8_start_key;//!<Key Ignition On/Off Indicator
        uint8 u8_start_suction_fan_off;//!<Suction Fan On/Off Indicator
        uint8 u8_start_shaft_drive_off;//!<Shaft Drive On/Off Indicator
        uint8 u8_start_neutral;//!<Joystick Neutral Indicator
} T_ChkPoints_EngineStarter;

/** \brief Control Structure - Engine Starter Control
 *
 * This structure represents all variables and pointers that
 * are utilized and tracked for engine starter control that need to
 * persist through cyclic calls (static).
 *
 * This structure does not include any variables that are considered
 * temporary.
 */
typedef struct
{
        //RX CAN Variables
        uint8 *pu8_engine_speed;//!<RX Engine Speed

        //TX CAN Variables
        uint8 *pu8_neutral_safe_status;//!<TX Neutral Safe Status

        //Local Variables
        uint8 u8_engine_start_cmd; //!<Engine On/Off Status

        //Control Checkpoints
        T_ChkPoints_EngineStarter *pt_chk;//!<Engine Start Checkpoint

} T_EngineStarterControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_engineStarterControl(T_UserInterface *_ui,T_ChkPoints_EngineStarter *_chkEngineStarter);
sint16 update_engineStarterControl(void);
void getEngineStartStatus(uint8 *pu8_engine_start_status);
void getEngineOffStatus(uint8 *pu8_engine_off_status);

#endif /* APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_ */

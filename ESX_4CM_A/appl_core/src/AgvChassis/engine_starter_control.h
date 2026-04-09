//-----------------------------------------------------------------------------
/**
 * \file       engine_starter_control.h
 * \brief      AgvChassis - Engine Starter Control
 *
 * \addtogroup AgvChassis
 * @{
 * \addtogroup EngineStarterControl Engine Starter Control
 *
 * The Engine Starter Control Module manages the engine starting sequence,
 * crank timing, and safety interlock logic based on operator inputs and
 * system conditions.
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
 * Jan 6, 2026 Tiffany.Gohnert
 *
 * @{
 */

#ifndef APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"
//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define NEUTRAL_SAFE_FALSE         (0u) /**< Indicates the machine is not in a safe neutral state */
#define NEUTRAL_SAFE_TRUE          (1u) /**< Indicates the machine is in a safe neutral state */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct ChkPoints_EngineStarter
 * \brief Checkpoints Structure - Engine Starter Control
 *
 * This structure represents all checkpoints that are relevant
 * to shaft drive control
 */
typedef struct
{
        uint8 u8_eng_status;         //!<Engine Off Indicator
        uint8 u8_start_key;          //!<Key Ignition On/Off Indicator
        uint8 u8_suction_fan_status; //!<Suction Fan On/Off Indicator
        uint8 u8_shaft_drive_status; //!<Shaft Drive On/Off Indicator
        uint8 u8_js_neutral;         //!<Joystick Neutral Indicator
} T_ChkPoints_EngineStarter;

/**
 * \struct EngineControl
 * \brief Control Structure - Engine Starter Control
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
        // RX CAN Variables
        uint16 *pu16_engine_speed;         //!< RX Engine Speed

        // TX CAN Variables
        uint8 *pu8_neutral_safe_status;    //!< TX Neutral Safe Status

        // Local Variables
        uint8 u8_engine_start_cmd;         //!< Engine Start Command
        uint32 u32_engine_start_time;      //!< Engine Start Time
        uint8 u8_engine_status;            //!< Current Engine Status
        uint8 u8_prev_engine_status;       //!< Previous Engine Status

        // Control Checkpoints
        T_ChkPoints_EngineStarter *pt_chk; //!< Engine Start Checkpoint

} T_EngineControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_engineStarterControl(T_CANDevices *_can_devs,T_ChkPoints_EngineStarter *_chkEngineStarter);
sint16 update_engineStarterControl(void);
void get_engineStatus(uint8 *pu8_engine_status);
void get_engineRuntime(uint32 *pu32_engine_runtime);

#endif /* APPL_CORE_SRC_AGVCHASSIS_ENGINE_STARTER_CONTROL_H_ */

//-----------------------------------------------------------------------------
/**
 * \file       can_engine.h
 * \brief      AgvComms - CAN Engine Module
 *
 * \addtogroup System
 * @{
 * \addtogroup CanEngine CAN Engine
 *
 * The CAN Engine module manages the CAN communication between the main
 * controller and the Engine Control Unit (ECU). It handles parsing incoming
 * engine telemetry (e.g., speed, temperature, status) and formatting
 * outgoing commands (e.g., throttle requests) to ensure reliable integration.
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
 * Feb 5, 2026 kyle.boch
 *
 * @{
 */
#ifndef APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_
#define APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \struct T_Engine
 * \brief  Structure to contain all Engine Parameters.
 */
typedef struct
{
    //RX Values
    uint8 u8_engineStatus;          //!<On/Off Status of the Engine
    uint8 u8_engineSpeed;           //!<Speed (RPM) of the Engine
    sint16 s16_engineCoolantTemp;   //!<Engine Coolant Temperature
    sint16 s16_engineIntakeTemp;    //!<Engine Intake Manifold Temperature

    //TX
    uint16 u16_rpm_command;          //!<Throttle command for engine

}T_Engine;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_ */


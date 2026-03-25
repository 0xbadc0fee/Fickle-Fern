/*! \file       can_engine.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 5, 2026 kyle.boch
   	\endimplementation
 */
#ifndef APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_
#define APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/** \brief Structure to contain all Engine Parameters **/
typedef struct
{
        //RX Values
        uint8 u8_engineStatus;          //!<On/Off Status of the Engine
        uint8 u8_engineSpeed;           //!<Speed (RPM) of the Engine
        sint16 s16_engineCoolantTemp;   //!<Engine Coolant Temperature
        sint16 s16_engineIntakeTemp;    //!<Engine Intake Manifold Temperature

        //TX
        uint8 u8_eng_ovrrd_ctrl_mode; //SPN 695
        uint8 u8_engine_speed_ctrl_req; //SPN 696
        uint8 u8_engine_override_ctrl_pri; //SPN 897
        uint16 u16_engine_req_speed;//SPN 898    //!<Throttle command for engine
        uint8 u8_eng_req_torq_limit; //SPN 518
        uint8 u8_ctrl_purpose; //SPN 3350
        uint8 u8_eng_req_torq_hires; //SPN 4191

}T_Engine;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_SYSTEM_CAN_DEVICES_CAN_ENGINE_H_ */


//-----------------------------------------------------------------------------
/*! \file       can_device_interface.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Jan 7, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdbool.h>

//STW
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_can.h"
#include "x_osf.h"
#include "osy_dph_data_pool_protector.h"

//PROJECT
#include "j1939_data_pool.h"
#include "can_handler.h"

#include "hmi_definition.h"
#include "can_engine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static bool maq_CanAvailable[X_CAN_COUNT];

T_UserInterface gt_ui;
T_Engine gt_engine;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */
/** \brief Initialize CAN Interfaces
 *
 *  This funciton looks at all can interface settings described in openSYDE project and initializes
 *  said interfaces.
 *
 *  \return s16_error Error Code
 */
sint16 init_canInterfaces(void)
{
    sint16 s16_Error = C_NO_ERR;
    T_x_osf_can t_CanSettings;
    T_x_can_bus_config t_CanBusConfig =
    {
       .u32_DataBitrate = 0u,                  // Not used
       .u16_DataSamplePoint = 0u,              // Not used
       .u16_Protocol = X_CAN_BUS_PROTOCOL_CAN, // Classic CAN protocol
       .pr_FailureCallback = NULL,             // Not used
       .pv_CallbackParameter = NULL            // Not used
    };

    for (uint16 u16_Channel = 0u; (u16_Channel < X_CAN_COUNT) && (s16_Error == C_NO_ERR); u16_Channel++)
    {
       s16_Error = x_osf_can_get_param(u16_Channel, &t_CanSettings);
       if (s16_Error == C_NO_ERR)
       {
          t_CanBusConfig.u32_Bitrate = t_CanSettings.u32_Bitrate;
          t_CanBusConfig.u16_SamplePoint = t_CanSettings.u16_SamplePoint;
          s16_Error = x_can_bus_init(u16_Channel, &t_CanBusConfig);
          if (s16_Error == C_NOACT)
          {
             // Channel is not available for this HW variant
             s16_Error = C_NO_ERR;
          }
          else
          {
             maq_CanAvailable[u16_Channel] = true;
          }
       }
    }

    return s16_Error;

}

/** \brief Update CAN Inputs
 *
 *  This function utilizes the "can_in_map" file to populate all control structures with openSYDE CAN variables.
 *  This function runs cyclically in the main control loop while the actual CAN values are captured in the respective
 *  globals by the openSYDE Task running concurrently.
 *
 *  \return s16_error Error Code
 */
sint16 update_canInputs(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(J1939_DATA_POOL_INDEX);

    #define DP2CNTRL(name, DST_LVALUE, SRC_RVALUE) DP_ASSIGN((DST_LVALUE), (SRC_RVALUE));
    #include "can_in_map.def"
    #undef DP2CNTRL

    (void)osy_dph_unlock_data_pool(J1939_DATA_POOL_INDEX);

    return s16_error;

}

/** \brief Update CAN Output
 *
 *  This function utilizes the "can_out_map" file to populate all openSYDE CAN variables with control structure variables .
 *  This function runs cyclically in the main control loop while the actual CAN values are sent in the respective
 *  globals by the openSYDE Task running concurrently.
 *
 *  \return s16_error Error Code
 */
sint16 update_canOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(J1939_DATA_POOL_INDEX);

    #define CNTRL2DP(name, DP_LVALUE, SRC_RVALUE) DP_ASSIGN((DP_LVALUE), (SRC_RVALUE));
    #include "can_out_map.def"
    #undef CNTRL2DP

    (void)osy_dph_unlock_data_pool(J1939_DATA_POOL_INDEX);

    return s16_error;

}

/** \brief Check if CAN bus is available
 *
 *  This function checks to determine if a CAN bus is initialized and available on the given hardware
 *
 *  \param ou16_Channel CAN bus to check availability
 *
 *  \retval TRUE Requested CAN bus is available
 *  \retval FALSE Requested CAN bus is NOT available
 */
bool can_get_availability_state(const uint16 ou16_Channel)
{
   return maq_CanAvailable[ou16_Channel];
}


//EOF

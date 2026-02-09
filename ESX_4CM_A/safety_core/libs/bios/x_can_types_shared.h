//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       CAN definitions/types shared between CAN kernel and CAN client
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_CAN_TYPES_SHARED_H___
#define X_CAN_TYPES_SHARED_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_CAN_OBJ_TYPE_RX           (1u) /*!< Message object, that can receive data frames */
#define X_CAN_OBJ_TYPE_TX           (2u) /*!< Message object, that can send data frames */
#define X_CAN_OBJ_TYPE_TX_RTR       (3u) /*!< Message object, that can receive RTR frames (requests) AND transmit
                                              response (data frame) automatically */
#define X_CAN_OBJ_TYPE_RTR_SENDER   (4u) /*!< This message object can only send RTR frames.
                                              It may be used to send RTR requests */
#define X_CAN_OBJ_TYPE_RTR_RECEIVER (5u) /*!< This message object is only able to receive RTR frames (RTR requests).
                                              Data frames can not be received. */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief CAN ID structure */
typedef struct
{
   uint32 u32_Id; //!< CAN identifier 11/29bit
   uint8 u8_Xtd;  //!< bXtd == true: 29bit ID, bXtd == false: 11bit ID
} T_x_can_id;

/*! \brief CAN message */
typedef struct
{
   T_x_can_id t_Id;   //!< CAN message ID
   uint8 u8_Dlc;      //!< Data Length [0..8]
   uint32 u32_Time;   //!< RX timestamp (not used for TX messages)
   uint8 au8_Data[8]; //!< Message data bytes (aligned to 32bit boundary)
} T_x_can_msg;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_CAN_TYPES_SHARED_H___

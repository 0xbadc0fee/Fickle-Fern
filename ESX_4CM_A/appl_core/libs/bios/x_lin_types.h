//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Generic x_lin types
   \copyright  Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_LIN_TYPES_H
#define X_LIN_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

// LIN Service Types (T_x_lin_service::u8_ServiceType)
#define X_LIN_SERVICE_TYPE_RX          (0u) //!< LIN service to requests data from slaves
#define X_LIN_SERVICE_TYPE_TX          (1u) //!< LIN service to sends data to slaves
#define X_LIN_SERVICE_TYPE_TX_WAKEUP   (2u) //!< LIN service to wake up the slaves

// LIN Checksum Types (T_x_lin_service::u8_ChecksumType)
#define X_LIN_CHECKSUM_TYPE_CLASSIC    (0u) //!< Checksum calculation is done over the data bytes only (LIN1.x)
#define X_LIN_CHECKSUM_TYPE_ENHANCED   (1u) /*!< Checksum calculation is done over the data bytes and the protected
                                                  identifier byte is called enhanced (LIN2.x) */

// LIN Service Status (T_x_lin_service::u16_Status)
#define X_LIN_SERVICE_STATUS_OK              (0u) //!< Transmission was successful
#define X_LIN_SERVICE_STATUS_TIMEOUT_ERR     (1u) //!< Service timed out
#define X_LIN_SERVICE_STATUS_CHECKSUM_ERR    (2u) //!< Wrong Checksum
#define X_LIN_SERVICE_STATUS_HARDWARE_ERR    (3u) //!< Communication error occurred

// LIN Bus States
#define X_LIN_BUS_STATE_POWER_OFF            (0x00u) //!< LIN Bus is in power off mode
#define X_LIN_BUS_STATE_UNINITIALIZED        (0x01u) //!< uninitialized LIN Bus
#define X_LIN_BUS_STATE_ACTIVE               (0x02u) //!< LIN Bus is in active mode
#define X_LIN_BUS_STATE_SLEEP                (0x03u) //!< LIN Bus is in sleep mode

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! \brief Structure of a LIN message
typedef struct
{
   uint8 u8_Id;       //!< Identifier of the LIN message (0...0x3F, without paritybit)
   uint8 u8_Dlc;      //!< Count of data in au8_Data (0...8)
   uint32 u32_Time;   //!< Timestamp of the message
   uint8 au8_Data[8]; //!< Data of the LIN message
}T_x_lin_msg;

//! \brief Structure to configure the service of a LIN message
typedef struct
{
   T_x_lin_msg t_Msg;        //!< Payload of the LIN message
   uint8 u8_ServiceType;     //!< Type of the LIN transmission (X_LIN_SERVICE_TYPE_..)
   uint8 u8_ChecksumType;    //!< Type of Checksum (X_LIN_CHECKSUM_TYPE_..)
   uint16 u16_ServiceStatus; //!< Status of the LIN service (X_LIN_SERVICE_STATUS_..)
}T_x_lin_service;

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Signature of the callback function to handle LIN services.

   Event callback function is intended to implement an reaction due to an transmission or reception of an LIN service.

   \param[in]        ou16_Channel                  Interface channel number [0 .. X_LIN_COUNT[
   \param[in]        ou32_CallbackParameter        A user defined callback parameter
   \param[in]        opt_Service                   Structure of the transmit or receive LIN service

   \return  C_NO_ERR    the LIN service was successfully transmitted/received
   \return  others      an error occurred
*/
//----------------------------------------------------------------------------------------------------------------------
typedef sint16 (* PR_x_lin_EventCallback)(const uint16 ou16_Channel, const uint32 ou32_CallbackParameter,
                                          const T_x_lin_service * const opt_Service);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Signature of the callback function to handle wakeup services.

   The wakeup callback function is called after an wakeup service was called.

   \param[in]        ou16_Channel                  Interface channel number [0 .. X_LIN_COUNT[
   \param[in]        ou32_CallbackParameter        A user defined callback parameter
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_lin_WakeupCallback)(const uint16 ou16_Channel, const uint32 ou32_CallbackParameter);

//! \brief Structure to configure the LIN Interface
typedef struct
{
   uint16 u16_Bitrate;                         //!< Bitrate (bits/s)
   uint16 u16_RequestBufSize;                  //!< LIN-Service count inside the request FIFO buffer
   uint16 u16_ResponseBufSize;                 //!< LIN-Service count inside the response FIFO buffer
   uint16 u16_ErrorBufSize;                    //!< LIN-Service count inside the error FIFO buffer
   uint32 u32_RequestCallbackParameter;        //!< Callback parameter of the request callback function
   PR_x_lin_EventCallback pr_RequestCallback;  //!< Request callback function (request filter)
   uint32 u32_ResponseCallbackParameter;       //!< Callback parameter of the response callback function
   PR_x_lin_EventCallback pr_ResponseCallback; //!< Response callback function
   uint32 u32_ErrorCallbackParameter;          //!< Callback parameter of the error callback function
   PR_x_lin_EventCallback pr_ErrorCallback;    //!< Error callback function
   uint32 u32_WakeupCallbackParameter;         //!< Callback parameter of the wakeup callback function
   PR_x_lin_WakeupCallback pr_WakeupCallback;  //!< Wakeup callback function
} T_x_lin_config;

#endif /* X_LIN_TYPES_H */

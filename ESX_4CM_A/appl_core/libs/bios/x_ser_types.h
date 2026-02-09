//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      generic x_ser types and defines for serial communication functions
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SER_TYPES_H
#define X_SER_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_SER_PARITY_NONE           (0u) //!< No parity bit
#define X_SER_PARITY_EVEN           (1u) //!< Parity bit is in even mode
#define X_SER_PARITY_ODD            (2u) //!< Parity bit is in odd mode

#define X_SER_FRAME_LEN_7_BIT       (7u) //!< Payload length is 7bit
#define X_SER_FRAME_LEN_8_BIT       (8u) //!< Payload length is 8bit

// status bits for x_ser_get_status
#define X_SER_STATUS_PARITY_ERR     (0x01u) //!< Parity error
#define X_SER_STATUS_FRAME_ERR      (0x02u) //!< Frame error
#define X_SER_STATUS_OVERRUN        (0x04u) //!< Hardware buffer overflow error
#define X_SER_STATUS_RXFIFO_OVERRUN (0x08u) //!< RX FIFO buffer overflow error
#define X_SER_STATUS_TXFIFO_OVERRUN (0x10u) //!< TX FIFO buffer overflow error

/* -- Types --------------------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Signature of the callback function to handle serial interface errors.

   Serial RS232 error callback is intended to implement an application specific error reaction.
   The callback is executed whenever a RS232 transfer related error occur.

   The error is passed to the callback function as a bit coded information in parameter \p ou32_Status.
   The status correspond to the status that can be found in T_x_ser_status::u32_Status.
   However \p ou32_Status contains only those flags that actually leads to the error. In contrast to
   T_x_ser_status::u32_Status that contains all error flags since the last time the status was read using function
   x_ser_get_status.

   The following error flags may be possible:
   - Bit0: X_SER_STATUS_PARITY_ERR        parity bit of the latest RX characters was incorrect
                                          -> character will not be saved!
   - Bit1: X_SER_STATUS_FRAME_ERR         the latest RX character had incorrect frame format
   - Bit2: X_SER_STATUS_OVERRUN           the pending character in the RX hardware buffer was overwritten with new data
   - Bit3: X_SER_STATUS_RXFIFO_OVERRUN    the character read from the RX hardware buffer was dropped as the
                                          RX software buffer is full

   \param[in]        ou32_CallbackParameter     A user defined callback parameter
                                                (can be used to take the instance pointer of a C++ object)
   \param[in]        ou16_Channel               RS232 interface channel number the error occurred
   \param[in]        ou32_Status                RS232 status involved into the callback - see: T_x_ser_status.
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_ser_ErrorCallback)(void * const opv_Parameter, const uint16 ou16_Channel,
                                        const uint32 ou32_Status);

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Signature of the callback function called when a TX or RX event occurres on the serial interface.

   Serial RS232 event callback is intended to implement an application specific TX and RX reaction.
   The callback may be executed for TX and RX events.

   \par TX Event
   Whenever the tranmission of an character has been started, the TX event callback will be executed. The user defined
   parameter (specified at callback installation) ,the channel number of the involved serial interface and the
   actually TX data will be passed back to the callback function.

   \note
   This event can be used by the application to reload the TX buffer!

   \par RX Event
   Whenever a character was received successfully the RX event will be executed. The user defined
   parameter (specified at callback installation) the channel number of the involved serial interface and the
   actually RX data will be passed back to the callback function.

   \note
   This event is executed before the RX data will be stored into the RX software buffer. Neverthelesl the data will
   be stored into the RX software buffer afterwards. \n
   If the RX data will be processed in the callback the user application is responsible to prefent an RX buffer
   overflow (e.g. by clearing the buffer cyclically).


   \param[in]        opv_Parameter           a user defined callback parameter
                                             (can be used to take the instance pointer of a C++ object)
   \param[in]        ou16_Channel            RS232 interface channel number the event occurred
   \param[in]        ou8_Data                the RX data resp. the TX data

   \return
   void
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_ser_EventCallback)(void * const opv_Parameter, const uint16 ou16_Channel, const uint8 ou8_Data);

/*! \brief  Serial interface configuration data. */
typedef struct
{
   uint32 u32_Bitrate;                        //!< Bitrate in bits per second
   uint8 u8_DataLen;                          //!< Number of Data bits (7..8)
   uint8 u8_Parity;                           //!< Parity Type (NONE/EVEN/ODD)
   uint8 u8_Stopbits;                         //!< Number of stopbits: 1, 2
   uint16 u16_RxBufsize;                      //!< RX buffer size in bytes
   uint16 u16_TxBufsize;                      //!< TX buffer size in bytes
   void *  pv_RxCallbackParameter;            //!< Parameter passed to RX callback
   PR_x_ser_EventCallback pr_RxEventCallback; //!< Callback function pointer to handle RX events.
   void *  pv_TxCallbackParameter;            //!< Parameter passed to TX callback
   PR_x_ser_EventCallback pr_TxEventCallback; //!< Callback function pointer to handle TX events.
   void *  pv_ErrorCallbackParameter;         //!< Parameter passed to Error callback
   PR_x_ser_ErrorCallback pr_ErrorCallback;   //!< Callback function pointer to handle error events.
} T_x_ser_config;

/*! \brief  Serial interface communication configuration data. */
typedef struct
{
   uint32 u32_Bitrate; //!< Bitrate in bits per second
   uint8 u8_DataLen;   //!< Number of Data bits (7..8)
   uint8 u8_Parity;    //!< Parity Type (NONE/EVEN/ODD)
   uint8 u8_Stopbits;  //!< Number of stopbits: 1, 2
} T_x_ser_com_config;

/*! \var T_x_ser_status::u32_Status

   This variable contains bitcoded status information of the serial RS232 interface. The status variable is cleared
   whenever the status is read using x_ser_get_status. According to this the flags reflect the status since the last
   time the status was read. \n
   Each bit is represented by a define:
   - Bit0: X_SER_STATUS_PARITY_ERR        the parity bit of at least one of the received characters was incorrect
   - Bit1: X_SER_STATUS_FRAME_ERR         at least one of the received characters had incorrect frame format
   - Bit2: X_SER_STATUS_OVERRUN           at least one character was lost due to a  RX (hardware) buffer overflow
   - Bit3: X_SER_STATUS_RXFIFO_OVERRUN    at least one character was lost as RX (software) buffer was full

   \note
   The status X_SER_STATUS_TXFIFO_OVERRUN (bit 4) is intercepted by the BIOS. So this error flag will never occur!
*/
typedef struct
{
   uint32 u32_Bitrate;   //!< Configured bitrate (bits/s)
   uint32 u32_Status;    //!< Bit coded: X_SER_STATUS_...
   uint16 u16_RxBufSize; //!< RX buffer size in byte
   uint16 u16_TxBufSize; //!< TX buffer size in byte
   uint8 u8_DatLen;      //!< Number of Data Bits (7/8Bit)
   uint8 u8_Parity;      //!< Parity setting (NONE,EVEN,ODD)
   uint8 u8_Stopbits;    //!< Number of stop bits
} T_x_ser_status;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* _X_SER_TYPES_H */

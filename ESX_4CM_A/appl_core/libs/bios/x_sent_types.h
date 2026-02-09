//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Types and constants for Single Edge Nibble Transmission (SENT) API
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SENT_TYPES_H
#define X_SENT_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* SENT operation mode configuration (bit coded, options might be combined with OR operator) */
#define X_SENT_MODE_SHORT_SERIAL_MSG          (0x00u) // use short serial message format (slow channel)
#define X_SENT_MODE_ENHANCED_SERIAL_MSG       (0x01u) // use enhanced serial message format (slow channel)
#define X_SENT_MODE_IGNORE_END_PULSE          (0x02u) // ignore an end pulse (e.g. pause pulse)
#define X_SENT_MODE_IGNORE_DRIFT_ERROR        (0x04u) // ignore calibration pulse drift errors

/* CRC calculation configuration (bit coded, options might be combined with OR operator) */
#define X_SENT_CRC_STANDARD                   (0x0000u) // use J2716 standard CRC algorithm (default)
#define X_SENT_CRC_ALTERNATIVE                (0x0001u) // use alternative CRC algorithm
#define X_SENT_CRC_INCLUDE_STATUS_NIBBLE      (0x0002u) // include status nibble into CRC calculation
#define X_SENT_CRC_NO_TRAILING_ZERO_NIBBLE    (0x0004u) // no trailing zero nibble added in CRC calculation
#define X_SENT_CRC_FAST_CHANNEL_DISABLED      (0x0008u) // CRC calculation for fast channel data disabled
#define X_SENT_CRC_SLOW_CHANNEL_DISABLED      (0x0010u) // CRC calculation for slow channel data disabled

/* SENT channel status */
#define X_SENT_STATUS_STOP                    (0u) // SENT channel disabled
#define X_SENT_STATUS_INITIALIZED             (1u) // SENT channel waiting for first calibration pulse
#define X_SENT_STATUS_RUNNING                 (2u) // SENT channel lost synchronization
#define X_SENT_STATUS_SYNCHRONIZED            (3u) // SENT channel synchronized, receiving data

/* defines for error flags */
// SENT error: calibration pulse deviates more than 25% from nominal value
#define X_SENT_ERROR_FREQUENCY_RANGE          (0x0020u)
// SENT error: calibration pulse deviates more than 1.56% from last calibration
#define X_SENT_ERROR_FREQUENCY_DRIFT          (0x0040u)
// SENT error: more or to few nibbles than expected
#define X_SENT_ERROR_NUMBER_OF_NIBBLES        (0x0080u)
// SENT error: nibble value out of range (<0 or > 15)
#define X_SENT_ERROR_NIBBLE_VALUE             (0x0100u)
// SENT error: CRC error on fast channel data
#define X_SENT_ERROR_FAST_CHANNEL_CRC         (0x0200u)
// SENT error: slow channel frame format error (only valid for short message format)
#define X_SENT_ERROR_WRONG_STATUS_NIBBLE      (0x0400u)
// SENT error: CRC error on slow channel data
#define X_SENT_ERROR_SLOW_CHANNEL_CRC         (0x1000u)
// SENT error: watch dog error
#define X_SENT_ERROR_RX_TIMEOUT               (0x2000u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

typedef struct
{
   uint16 u16_Channel1;  // 1st channel data (e.g. nibble 1..3 for frames with 6 data nibbles)
   uint16 u16_Channel2;  // 2nd channel data (e.g. nibble 4..6 for frames with 6 data nibbles)
   uint8 u8_FrameLength; // Number of data nibbles (1..8)
   uint8 u8_Crc;         // CRC checksum Nibble
} T_x_sent_fast_channel_data;

/*
   SENT Encoding Scheme of Short and Enhanced Serial Message Format (aka "Slow Channel" data/signals):

   +---------------+------------------------------------+---------------------------------------+
   | Format        |       ..._SHORT_SERIAL_MSG         |       ..._ENHANCED_SERIAL_MSG         |
   +---------------+------------------------------------+---------------------------------------+
   | Data          | 8bit                               | 12bit             | 16bit             |
   +---------------+------------------------------------+---------------------------------------+
   | Configuration | Not used                           | 0                 | 1                 |
   +---------------+------------------------------------+---------------------------------------+
   | MessageId     | 4bit                               | 8bit              | 4bit              |
   +---------------+------------------------------------+---------------------------------------+
   | CRC           | 4bit                               | 6bit                                  |
   +---------------+------------------------------------+---------------------------------------+
*/
typedef struct
{
   uint16 u16_Data;        // Data (8bit, 12bit or 16bit)
   uint8 u8_Format;        // Serial message format (0 = Short or 1 = Enhanced Serial Message Format)
   uint8 u8_Configuration; // 0 -> 8bit MessageId and 12bit Data; 1 -> 4bit MessageId and 16bit Data
   uint8 u8_MessageId;     // Message identifier (4bit or 8bit)
   uint8 u8_Crc;           // CRC checksum (4bit or 6bit)
} T_x_sent_slow_channel_data;
typedef struct
{
   uint8 u8_SystemStatus;      // SENT System Status (see defines X_SENT_STATUS_...)
   uint8 u8_SlowChannelBufOvl; // slow channel buffer overflow counter
   uint16 u16_LastError;       // last error (see defines X_SENT_ERROR_...)
} T_x_sent_status;

// fast channel data receive callback function prototype
typedef void (* PR_x_sent_rx_fast_callback)(const uint32 ou32_CallbackParameter, const uint16 ou16_Channel,
                                            const T_x_sent_fast_channel_data * const opt_Data);
// slow channel data receive callback function prototype
typedef void (* PR_x_sent_rx_slow_callback)(const uint32 ou32_CallbackParameter, const uint16 ou16_Channel,
                                            const T_x_sent_slow_channel_data * const opt_Data);
// error callback function prototype
typedef void (* PR_x_sent_error_callback)(const uint32 ou32_CallbackParameter, const uint16 ou16_Channel,
                                          const uint16 ou16_Error);
typedef struct
{
   uint8 u8_TickTimeUs;           // Nominal tick time in micro seconds, range 1..90
   uint8 u8_FrameLength;          // Frame length in data nibbles (without status and CRC nibbles), range 0..8
   uint8 u8_InputFilter;          // Digital input filter, resolution 0.03*u8_TickTimeUs, range 0..15 (0 = filter off)
   uint8 u8_OperationMode;        // Configures the SENT operation mode, see defines XB_SENT_MODE_...
   uint16 u16_CrcMode;            // Configuration of CRC calculation, see defines XB_SENT_CRC_...
   uint16 u16_SlowChannelBufSize; // FIFO buffer size for serial message data (slow channel)

   uint32 u32_NibbleMapping; // Fast channel data nibble mapping:
   // each nibble of u32_NibbleMapping points to a destination nibble in
   // T_x_sent_fast_channel_data::u16_Channel1 and :: u16_Channel2
   // nibble 0 of u32_NibbleMapping remaps the first received nibble, nibble1 the 2nd, and so on.
   // Therefore up to 8 received data nibbles can be remapped to u16_Channel1 and u16_Channel2.
   // Remap nibble value 0 points to the LSN of u16_Channel1, remap value 3 points to MSN of u16_Channel1,
   // Remap nibble value 4 points to the LSN of u16_Channel2, remap value 7 points to MSN of u16_Channel2.
   // If a frame length is <8 nibbles e.g. 6 than the unused nibbles 6 and 7 are zero and might be used
   // to fill the destination u16_Channel1 and/or u16_Channel2 with nibbles of value zero.
   //
   // Example: sensor data frame length is 6 nibbles, two 12bit fast channel data are send with MSN first.
   //          The first 12bit data value should be located in u16_Channel1 the second in u16_Channel2
   //          u32_NibbleMapping = 0x73456012u
   //
   //          Destination:   u16_Channel1    u16_Channel2
   //                           _ _ _ _         _ _ _ _
   //                          |3|2|1|0|       |7|6|5|4|
   //                           | | | |         | | | |
   //    RX nibble 0 -------------+ | |         | | | |
   //    RX nibble 1 ---------------+ |         | | | |
   //    RX nibble 2 -----------------+         | | | |
   //    RX nibble 3 -----------------------------+ | |
   //    RX nibble 4 -------------------------------+ |
   //    RX nibble 5 ---------------------------------+
   //             [6] =0 -------+               |        note: nibble 6 and 7 are not used and have zero value
   //             [7] =0 -----------------------+              they are used to fill the MSN of the destination
   uint32 u32_RxSlowCallbackParameter;           // slow channel callback parameter
   PR_x_sent_rx_slow_callback pr_RxSlowCallback; // slow channel receive callback function pointer
   uint32 u32_RxFastCallbackParameter;           // fast channel callback parameter
   PR_x_sent_rx_fast_callback pr_RxFastCallback; // fast channel receive callback function pointer
   uint32 u32_ErrorCallbackParameter;            // error callback parameter
   PR_x_sent_error_callback pr_ErrorCallback;    // error callback function pointer
   uint16 u16_FastChRxTimeout;                   // fast channel receive timeout in 500us steps,
} T_x_sent_config;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* X_SENT_TYPES_H */

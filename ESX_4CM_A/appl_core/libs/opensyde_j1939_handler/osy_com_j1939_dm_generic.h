//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Generic part of Interface to openSYDE J1939 DM modules
   \copyright   Copyright 2023 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_J1939_DM_GENERICH
#define OSY_COM_J1939_DM_GENERICH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_dpa_trg_data_protector.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/// All information of one DTC
typedef struct
{
   uint8 u8_IsActive;          // Flag if DTC is currently active
   uint32 u32_Spn;             // Suspect Parameter Number
   uint8 u8_Fmi;               // Failure Mode Identifier
   uint8 u8_OccurrenceCounter; // Number of times the associated SPN occured (127 if not available)
   uint8 u8_SpnConvMode;       // SPN conversion mode (should be 0, version 4)
} T_osy_com_j1939_dtc;

//----------------------------------------------------------------------------------------------------------------------
/// All information of all DM lamp statuses
typedef struct
{
   uint8 u8_MalfIndLamp;        // malfunction indicator lamp status (2 bits)
   uint8 u8_RedStopLamp;        // red stop lamp status (2 bits)
   uint8 u8_AmberWarnLamp;      // amber warning lamp status (2 bits)
   uint8 u8_ProtectLamp;        // protect warning lamp status (2 bits)
   uint8 u8_FlashMalfIndLamp;   // malfunction indicator lamp flash status (2 bits)
   uint8 u8_FlashRedStopLamp;   // red stop lamp flash status (2 bits)
   uint8 u8_FlashAmberWarnLamp; // amber warning lamp flash status (2 bits)
   uint8 u8_FlashProtectLamp;   // protect warning lamp flash status (2 bits)
} T_osy_com_j1939_dm_lamp_status;

//----------------------------------------------------------------------------------------------------------------------
/// Receiving struct for a DTC with all information
typedef struct
{
   uint16 u16_DtcIndex;                   // Index of current DTC starting with 0
   uint16 u16_DtcTotalCount;              // Expected number of DTCs of current RX process
   T_osy_com_j1939_dtc t_Dtc;             // Concrete DTC information
   T_osy_com_j1939_dm_lamp_status t_Lamp; // Concrete lamp information
} T_osy_com_j1939_dtc_rx;

//----------------------------------------------------------------------------------------------------------------------
/// DM Tx configuration for one logical J1939 node (with one specific source address)
typedef struct
{
   T_osy_com_j1939_dm_lamp_status t_LampStatus;     // all J1939 lamp status
   T_osy_com_j1939_dtc * const pt_Dtcs;             // array of all possible DTCs
   const uint16 u16_MaxDtcs;                        // maximum number of DTCs (= length of arrays references by pt_Dtcs)
   const uint8 u8_SourceAddress;                    // sender address of logical J1939 node
   T_osy_dpa_trg_dp_thread_lock t_ThreadLockData;   // Target specific thread lock data for node TX config read/write
   T_osy_dpa_trg_dp_process_lock t_ProcessLockData; // Target specific process lock data for node TX config read/write
} T_osy_com_j1939_dm_logic_node_tx_config;

// Callback types
typedef void (* PR_osy_com_j1939_dm_rx_dtc_received)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                                     const T_osy_com_j1939_dtc_rx * const opt_ReceivedDtc);
typedef void (* PR_osy_com_j1939_dm_error)(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                           const uint8 ou8_ErrorType);

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif

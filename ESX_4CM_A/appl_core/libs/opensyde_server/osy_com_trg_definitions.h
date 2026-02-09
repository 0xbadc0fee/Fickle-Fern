//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       openSYDE server target layer: definitions for process data exchange stacks

   Type definitions for compilation of the library.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_COM_TRG_DEFINITIONSH
#define OSY_COM_TRG_DEFINITIONSH

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define OSY_COM_TRG_MAX_CAN_INTERFACES        (6U) //< number of CAN interfaces to support

#define OSY_COM_TRG_CAN_RX_MODE_APPL_CORE     (1U) //< only ApplCore can receive the message
#define OSY_COM_TRG_CAN_RX_MODE_SAFETY_CORE   (2U) //< only SafetyCore can receive the message
#define OSY_COM_TRG_CAN_RX_MODE_BOTH_CORES    (3U) //< both cores can receive the message

//########################################################################################
//# Parameters:                                                                          #
//# 1) Name of defined macro for COMM RX filters                                         #
//# 2) Name of RX filter variable which shall be created                                 #
//# 3) Number of RX filters (number of entries in RX filters macro)                      #
//########################################################################################
//PRQA S 3410 ++  //parameters cannot be enclosed in () for this creative construction
#define OSY_COM_TRG_CREATE_COMM_RX_FILTER_VARIABLE(RX_FILTERS_DEFINE_NAME, VARIABLE_NAME, NUM_RX_FILTERS) \
   const T_osy_com_rx_filter VARIABLE_NAME[NUM_RX_FILTERS] = \
   { \
      RX_FILTERS_DEFINE_NAME(_CONFIG) \
   };
#define OSY_COM_TRG_CREATE_COMM_RX_FILTER_MODE_VARIABLE(RX_FILTERS_DEFINE_NAME, VARIABLE_NAME, NUM_RX_FILTERS) \
   uint8 VARIABLE_NAME[NUM_RX_FILTERS] = \
   { \
      RX_FILTERS_DEFINE_NAME(_MODES) \
   };

#define OSY_COM_TRG_RX_FILTER_CONFIG(ID_TYPE_EXT, FILTER, MASK, RX_BUFFER_SIZE, RX_MODE) \
   { ID_TYPE_EXT, FILTER, MASK, RX_BUFFER_SIZE }

#define OSY_COM_TRG_RX_FILTER_MODES(ID_TYPE_EXT, FILTER, MASK, RX_BUFFER_SIZE, RX_MODE) \
   RX_MODE
//PRQA S 3410 --

// Example for defining COMM stack RX filters:

//########################################################################################
//# Parameters:                                                                          #
//# 1) IdTypeExt: 0 -> 11bit; 1 -> 29bit                                                 #
//# 2) Filter ID                                                                         #
//# 3) Filter Mask                                                                       #
//# 4) Buffer size in number of messages                                                 #
//# 5) Filter mode (Only ApplCore | Only SafetyCore | Both cores)                        #
//########################################################################################
//#                         | 1 |   2   |   3   |  4  |                 5                #
//########################################################################################
/*
#define COMM_STACK_RX_FILTERS(CFG) \
   OSY_COM_TRG_RX_FILTER##CFG(0U, 0x100U, 0x7FFU, 10U, OSY_COM_TRG_CAN_RX_MODE_APPL_CORE), \
   OSY_COM_TRG_RX_FILTER##CFG(0U, 0x300U, 0x7FFU, 10U, OSY_COM_TRG_CAN_RX_MODE_SAFETY_CORE)
*/

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   uint8 * pau8_ModeRx;               //!< Pointer to array with RX filter modes (OSY_COM_TRG_CAN_RX_MODE_*)
   uint16 u16_SafetyCoreTxBufferSize; //!< Tx buffer size for SafetyCore. Set to 0 to disable Tx for SafetyCore
   uint16 u16_CanClientServerStartId; //!< StartID for CAN Client/Server Tx/Rx objects. Set to 0 if SafetyCore is unused
} T_osy_com_trg_can_init_params;      //!< Target specific initialization parameters

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif

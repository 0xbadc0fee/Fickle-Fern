//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       CAN Message Router
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_CTP_ROUTERH
#define OSY_CTP_ROUTERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_ctp_trg_if.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

// Wrapper for CAN TP router initialization
// Call first for each channel
// The macro should be invoked within a function to reduce the scope of the elements and to result in correct scope
//  prefixes
/*STW Deviation Coding Rules v4*/
/*Violated Rule: 20.10*/
/*Circumstances: using glue operator ##*/
/*Reason: creating different variable names combined with the same parameter*/
/*Potential risks: none*/
/*Prevention of risks:  nothing to do*/
/*PRQA S 0342 ++*/
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: using more than one ## operator*/
/*Reason: there are more variables to create*/
/*Potential risks: invalid preprocessing token because of different evaluation order*/
/*Prevention of risks: use of only a single ## operator for each variable to create*/
/*PRQA S 0881 ++*/
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: macro parameter used more than once*/
/*Reason: there are more variables to create depending on the same parameter*/
/*Potential risks: possible side effects*/
/*Prevention of risks:  no unexpected side effects, parameter intentionally used more than once*/
/*PRQA S 3456 ++*/
#define OSY_CTP_CAN_ROUTER_INIT(STRUCT_NAME, CHANNEL, SIZE_OF_RX_ROUTING_FIFO) \
   static T_osy_ctp_trg_can_message hat_RxRoutingQueue ## CHANNEL[(SIZE_OF_RX_ROUTING_FIFO)]; \
   static const T_osy_ctp_ro_init_configuration(STRUCT_NAME) = \
   { \
      (SIZE_OF_RX_ROUTING_FIFO), \
      &hat_RxRoutingQueue ## CHANNEL[0] \
   };
/*PRQA S 0342,0881,3456 --*/

/* -- Types --------------------------------------------------------------------------------------------------------- */
/// The structure is placed in the header to allow the application to provide
/// configuration for the routing of CAN messages.
typedef struct
{
   uint16 u16_RxFifoSize;                        ///< size of FIFO for routing of message frames
   T_osy_ctp_trg_can_message * const pt_RxQueue; ///< provide buffer for CAN message RX routing FIFO
} T_osy_ctp_ro_init_configuration;               ///< CAN router initial set-up

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_ctp_ro_init(const uint8 ou8_BusNumber,
                              const T_osy_ctp_ro_init_configuration * const opt_CanRouterInitConfig,
                              const T_osy_ctp_trg_can_init_params * const opt_TargetInitParameters);
extern sint16 osy_ctp_ro_close(const uint8 ou8_BusNumber);
extern sint16 osy_ctp_ro_configure_filter(const uint8 ou8_BusNumber, const uint8 ou8_CanIdentifierExtended,
                                          const uint32 ou32_FilterIdentifier, const uint32 ou32_FilterIdentifierMask);
extern sint16 osy_ctp_ro_peek_rx_message(const uint8 ou8_BusNumber, T_osy_ctp_trg_can_message * const opt_CanMessage);
extern void osy_ctp_ro_pop_rx_message(const uint8 ou8_BusNumber);
extern sint16 osy_ctp_ro_handle_rx_messages(const uint8 ou8_BusNumber);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif

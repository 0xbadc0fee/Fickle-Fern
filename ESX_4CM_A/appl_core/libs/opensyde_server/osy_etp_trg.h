//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Ethernet-TP: target driver

   Ethernet-TP target definitions for AURIX plattform

   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_ETP_TRG_H
#define OSY_ETP_TRG_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "osy_etp_trg_product.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define OSY_ETP_TRG_MAX_ETH_INTERFACES    (OSY_ETP_TRG_PRODUCT_ETHERNET_COUNT) //< number of Ethernet interfaces

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   struct netif * pt_NetworkInterface;
} T_osy_etp_trg_eth_init_params;                    //!< target specific initialization parameters
typedef struct sockaddr T_osy_etp_trg_eth_sockaddr; //!<target specific structure to hold a socket address

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 osy_etp_trg_get_init_parameters(const uint8 ou8_BusNumber,
                                       const T_osy_etp_trg_eth_init_params ** const oppt_init_params);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // OSY_ETP_TRG_H

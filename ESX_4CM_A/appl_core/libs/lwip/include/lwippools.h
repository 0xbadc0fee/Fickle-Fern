//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Pool definition for pool based memory allocation
   \details     Header defines memory pools with different sizes.
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

// No header guards allowed for this file

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#if defined NETIF_X_ETHNC_NUM && NETIF_X_ETHNC_NUM > 0u
// It's a TC3xx target
/*
 * Define pools of differnet size for TX traffic (RX buffers are declared by netif_x_ethnc)
 * +44: EthHhr+IpHdr+UdpHdr size. If payload size is a 2'complement size equal to the pool
 *      size, this provides space for the header and payload in a single buffer.
 * +4:  Some extra space for ETH_PADDINg and memory alignment
 */
LWIP_MALLOC_MEMPOOL_START
LWIP_MALLOC_MEMPOOL(20, 176) // +44+4
LWIP_MALLOC_MEMPOOL(20, 560) // +44+4
LWIP_MALLOC_MEMPOOL(20, 1540)
LWIP_MALLOC_MEMPOOL_END
#else
// It's a TC2xx target
/*
 * Define pools of different size for TX and RX traffic.
 * +44: EthHhr+IpHdr+UdpHdr size. If payload size is a 2'complement size equal to the pool
 *      size, this provides space for the header and payload in a single buffer.
 * +4:  Some extra space for ETH_PADDINg and memory alignment
 */
LWIP_MALLOC_MEMPOOL_START
LWIP_MALLOC_MEMPOOL(30, 176)  // +44+4
LWIP_MALLOC_MEMPOOL(20, 560)  // +44+4
LWIP_MALLOC_MEMPOOL(45, 1540) // Enough to receive a 64k fragmented IP packet (44 frames)
LWIP_MALLOC_MEMPOOL_END
#endif
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Types and constants for the SJA1105 and SJA1110 switch API

   Header defines types and constants for the SJA1105 and SJA1110 switch API.
   For a description of the data structure members consult the SJA1105P/Q/R/S or SJA1110A/B/C/D software user manual.

   \copyright  Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ETH_SWI_SJA11XX_TYPES_H
#define X_ETH_SWI_SJA11XX_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

//!< Ethernet port status - MAC-level diagnostic counters
typedef struct
{
   uint8 u8_N_RUNT;
   uint8 u8_N_SOFERR;
   uint8 u8_N_ALIGNERR;
   uint8 u8_N_MIIERR;
} T_x_eth_swi_sja11xx_mac_level_counters;

//!< Ethernet port status - MAC-level diagnostic flags
typedef struct
{
   uint8 u8_TYPEERR;
   uint8 u8_SIZEERR;
   uint8 u8_TCTIMEOUT;
   uint8 u8_PRIORERR;
   uint8 u8_NOMASTER;
   uint8 u8_MEMOV;
   uint8 u8_MEMERR;
   uint8 u8_INVTYP;
   uint8 u8_INTCYOV;
   uint8 u8_DOMERR;
   uint8 u8_PCFBAGDROP;
   uint8 u8_SPCPRIOR;
   uint8 u8_AGEPRIOR;
   uint8 u8_PORTDROP;
   uint8 u8_LENDROP;
   uint8 u8_BAGDROP;
   uint8 u8_POLIECERR;
   uint8 u8_DRPNONA664ERR;
   uint8 u8_SPCERR;
   uint8 u8_AGEDRP;
} T_x_eth_swi_sja11xx_mac_level_flags;

//!< High level statistics part 1 registers
typedef struct
{
   uint32 u32_N_TXBYTE;
   uint32 u32_N_TXBYTESH;
   uint32 u32_N_TXFRM;
   uint32 u32_N_TXFRMSH;
   uint32 u32_N_RXBYTE;
   uint32 u32_N_RXBYTESH;
   uint32 u32_N_RXFRM;
   uint32 u32_N_RXFRMSH;
   uint32 u32_N_POLERR;
   uint32 u32_N_CTPOLERR;
   uint32 u32_N_VLNOTFOUND;
   uint32 u32_N_CRCERR;
   uint32 u32_N_SIZEERR;
   uint32 u32_N_UNRELEASED;
   uint32 u32_N_VLANERR;
   uint32 u32_N_N664ERR;
} T_x_eth_swi_sja11xx_highlevel_statistics_part1;

//!< High level statistics part 2 registers
typedef struct
{
   uint16 u16_QLEVEL_HWM_7;
   uint16 u16_QLEVEL_7;
   uint16 u16_QLEVEL_HWM_6;
   uint16 u16_QLEVEL_6;
   uint16 u16_QLEVEL_HWM_5;
   uint16 u16_QLEVEL_5;
   uint16 u16_QLEVEL_HWM_4;
   uint16 u16_QLEVEL_4;
   uint16 u16_QLEVEL_HWM_3;
   uint16 u16_QLEVEL_3;
   uint16 u16_QLEVEL_HWM_2;
   uint16 u16_QLEVEL_2;
   uint16 u16_QLEVEL_HWM_1;
   uint16 u16_QLEVEL_1;
   uint16 u16_QLEVEL_HWM_0;
   uint16 u16_QLEVEL_0;
   uint32 u32_N_QFULL;
   uint32 u32_N_PART_DROP;
   uint32 u32_N_EGR_DISABLED;
   uint32 u32_N_NOT_REACH;
} T_x_eth_swi_sja11xx_highlevel_statistics_part2;

//!< Ethernet statistics counter registers
typedef struct
{
   uint32 u32_N_DROPS_NOLEARN;
   uint32 u32_N_DROPS_EMPTY_RO;
   uint32 u32_N_DROPS_ILL_DTAG;
   uint32 u32_N_DROPS_DTAG;
   uint32 u32_N_DROPS_SOTAG;
   uint32 u32_N_DROPS_SITAG;
   uint32 u32_N_DROPS_UTAG;
   uint32 u32_N_TX_BYTES_1024_15;
   uint32 u32_N_TX_BYTES_512_1023;
   uint32 u32_N_TX_BYTES_256_511;
   uint32 u32_N_TX_BYTES_128_255;
   uint32 u32_N_TX_BYTES_65_127;
   uint32 u32_N_TX_BYTES_64;
   uint32 u32_N_TX_MCAST;
   uint32 u32_N_TX_BCAST;
   uint32 u32_N_RX_BYTES_1024_15;
   uint32 u32_N_RX_BYTES_512_1023;
   uint32 u32_N_RX_BYTES_256_511;
   uint32 u32_N_RX_BYTES_128_255;
   uint32 u32_N_RX_BYTES_65_127;
   uint32 u32_N_RX_BYTES_64;
   uint32 u32_N_RX_MCAST;
   uint32 u32_N_RX_BCAST;
} T_x_eth_swi_sja11xx_statistics_counters;

//!< SJA11xx port diagnostics information
typedef struct
{
   T_x_eth_swi_sja11xx_mac_level_counters t_MacLevelCounters;
   T_x_eth_swi_sja11xx_mac_level_flags t_MacLevelFlags;
   T_x_eth_swi_sja11xx_highlevel_statistics_part1 t_HighLevelStat1;
   T_x_eth_swi_sja11xx_highlevel_statistics_part2 t_HighLevelStat2;
   T_x_eth_swi_sja11xx_statistics_counters t_StatCounters;
} T_x_eth_swi_sja11xx_port_diagnostics;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ETH_SWI_SJA11XX_TYPES_H

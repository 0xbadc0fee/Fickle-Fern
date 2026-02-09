//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      System Parameter
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.

*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef X_OSF_H
#define X_OSF_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_OSF_BOOT_SW_ID_LENGTH    3u //<! number of bytes for boot software id
#define X_OSF_ETH_MAC_ADDR_LENGTH  6u //<! number of bytes for mac address
#define X_OSF_ETH_HOSTNAME_LENGTH 32u //<! number of bytes for ethernet hostnames

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
   uint8 au8_ProgDate[3];  //!< date of programming (format: yymmdd)
   uint8 au8_ProgTime[3];  //!< time of programming (format: hhmmss)
   charn acn_Username[21]; //!< user name (20 characters + zero-termination)
} T_x_osf_fingerprint;     //!< this structure provides the common fingerprint
typedef struct
{
   uint32 u32_Bitrate;     //!< CAN Bitrate (bit/s)
   uint16 u16_SamplePoint; //!< Sample point (0.1%)
} T_x_osf_can;             //!< CAN interface parameter
typedef struct
{
   uint8 u8_Active;   //!< OPENSYDE communication channel Status
   uint8 u8_BusId;    //!< OPENSYDE communication channel BusId
   uint8 u8_NodeId;   //!< OPENSYDE communication channel NodeId
   uint8 u8_Reserved; //!< reserved (used for 32bit alignment)
} T_x_osf_oscs;       //!< OPENSYDE communication settings
typedef struct
{
   uint8 au8_Ip[4];      //!< IPV4 Ip-Address (e.g. 192.168.1.2 -> db0=192 db1=168 db3=1 db4=2)
   uint8 au8_NetMask[4]; //!< IPV4 NetMask (e.g. 255.255.255.0 -> db0=255 db1=255 db2=255 db3=0)
   uint8 au8_Gateway[4]; //!< IPV4 Gateway Address  (e.g. 192.168.1.1 -> db0=192 db1=168 db2=1 db3=1)
} T_x_osf_eth_ipv4;      //!< Ethernet interface parameter for IPV4
typedef struct
{
   uint8 u8_AutoNeg;    //!< Auto-negotiation ON/OFF (X_ON, X_OFF)
   uint16 u16_LinkMode; //!< Requested link speed and duplex mode
   uint32 u32_Feature;  //!< Additional configuration option
   uint8 u8_Reserved;   //!< reserved (used for 32bit alignment)
} T_x_osf_phy;          //!< Ethernet PHY configuration

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

void x_osf_get_prog_req_flag(uint8 * const opu8_ProgramRequestFlag);
void x_osf_get_fingerprint(T_x_osf_fingerprint * const opt_Fingerprint);
void x_osf_get_download_cnt(uint32 * const opu32_DownloadCount);
void x_osf_get_boot_sw_id(uint8 opu8_Identification[X_OSF_BOOT_SW_ID_LENGTH]);
sint16 x_osf_set_prog_req_flag(const uint8 ou8_ProgramRequestFlag);
sint16 x_osf_can_get_param(const uint16 ou16_Interface, T_x_osf_can * const opt_Settings);
sint16 x_osf_can_set_param(const uint16 ou16_Interface, const T_x_osf_can * const opt_Settings);
sint16 x_osf_can_get_oscs(const uint16 ou16_Interface, T_x_osf_oscs * const opt_Settings);
sint16 x_osf_can_set_oscs(const uint16 ou16_Interface, const T_x_osf_oscs * const opt_Settings);
sint16 x_osf_eth_get_mac(const uint16 ou16_Interface, uint8 opu8_MacAddress[X_OSF_ETH_MAC_ADDR_LENGTH]);
sint16 x_osf_eth_set_mac(const uint16 ou16_Interface, const uint8 opu8_MacAddress[X_OSF_ETH_MAC_ADDR_LENGTH]);
sint16 x_osf_eth_get_hostname(const uint16 ou16_Interface, charn opcn_Hostname[X_OSF_ETH_HOSTNAME_LENGTH]);
sint16 x_osf_eth_set_hostname(const uint16 ou16_Interface, const charn opcn_Hostname[X_OSF_ETH_HOSTNAME_LENGTH]);
sint16 x_osf_eth_get_oscs(const uint16 ou16_Interface, T_x_osf_oscs * const opt_Settings);
sint16 x_osf_eth_set_oscs(const uint16 ou16_Interface, const T_x_osf_oscs * const opt_Settings);
sint16 x_osf_eth_get_ipv4(const uint16 ou16_Interface, T_x_osf_eth_ipv4 * const opt_Settings);
sint16 x_osf_eth_set_ipv4(const uint16 ou16_Interface, const T_x_osf_eth_ipv4 * const opt_Settings);
sint16 x_osf_eth_get_phy(const uint16 ou16_Interface, T_x_osf_phy * const opt_Settings);
sint16 x_osf_eth_set_phy(const uint16 ou16_Interface, const T_x_osf_phy * const opt_Settings);
sint16 x_osf_switch_get_phy(const uint16 ou16_SwitchIf, const uint16 ou16_Phy, T_x_osf_phy * const opt_Settings);
sint16 x_osf_switch_set_phy(const uint16 ou16_SwitchIf, const uint16 ou16_Phy, const T_x_osf_phy * const opt_Settings);
sint16 x_osf_get_target_cfg(uint32 * const opu32_TargetCfg);
sint16 x_osf_set_target_cfg(const uint32 ou32_TargetCfg);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif

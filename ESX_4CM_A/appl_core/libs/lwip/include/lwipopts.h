//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Aurix specific configuration of lwIP stack
   \details     Header defines macros that configure the stack
   \copyright   Copyright (c) Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  LWIPOPTS_H
#define  LWIPOPTS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_memtypes.h"
#include "stwtypes.h"
#include "lwipopts_trg.h"
#if defined NETIF_X_ETHNC_NUM && NETIF_X_ETHNC_NUM > 0u
#include "x_eth_ptp_types.h"
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#if !((defined NETIF_X_ETHNC_NUM && NETIF_X_ETHNC_NUM > 0u) || (defined NETIF_X_ETH_NUM && NETIF_X_ETH_NUM > 0u))
#error At least one interface must exist!
#endif

// --------------------
// Checksum options
// --------------------
// Checksum offloading is disabled in MAC driver (because it does not support checksum calculation for fragmented IP)
#define CHECKSUM_GEN_IP                1
#define CHECKSUM_GEN_UDP               1
#define CHECKSUM_GEN_TCP               1
#define CHECKSUM_GEN_ICMP              1
#define CHECKSUM_CHECK_IP              1
#define CHECKSUM_CHECK_UDP             1
#define CHECKSUM_CHECK_TCP             1
#define CHECKSUM_CHECK_ICMP            1

// -----------------------------------------
// Target/Architecture specific settings
// -----------------------------------------

// Target type: OS support (NO_SYS=0) or Bare Metal (NO_SYS=1)
#define NO_SYS                         0

extern void xb_common_fast_memcpy(void * const opv_Destination, const void * const opv_Source, const uint32 ou32_Size);
#define MEMCPY(dst,src,len)            xb_common_fast_memcpy(dst,src,len)

#define LWIP_ERRNO_STDINCLUDE          1

#define SYS_LIGHTWEIGHT_PROT           1
#define LWIP_TCPIP_CORE_LOCKING        1
#define LWIP_TCPIP_CORE_LOCKING_INPUT  1

extern void sys_mark_tcpip_thread(void);
extern void sys_check_core_locked(void);
#define LWIP_MARK_TCPIP_THREAD()       sys_mark_tcpip_thread()
#define LWIP_ASSERT_CORE_LOCKED()      sys_check_core_locked()
#define LOCK_TCPIP_CORE()              sys_tcpip_core_lock(&lock_tcpip_core)
#define UNLOCK_TCPIP_CORE()            sys_tcpip_core_unlock(&lock_tcpip_core)

#define LWIP_MPU_COMPATIBLE            1
extern int sys_arch_random(void);
#define LWIP_RAND()                    sys_arch_random()

#define LWIP_SO_RCVBUF                 1
#define RECV_BUFSIZE_DEFAULT           8096

// Task priority and stack size of TCP thread: dont't care for NO_SYS
#define TCPIP_THREAD_STACKSIZE         (8U * 1024U)

// Architecture specific settings
#define MEM_ALIGNMENT                  4
#define ETH_PAD_SIZE                   2
#define LWIP_CHKSUM_ALGORITHM          3

// Memory management: Using mem pools
#define MEM_LIBC_MALLOC                0
#define MEMP_MEM_MALLOC                0
#define MEM_USE_POOLS                  1
#define MEMP_USE_CUSTOM_POOLS          1
#define MEM_USE_POOLS_TRY_BIGGER_POOL  1

// Custom pbuf configuration
#if defined NETIF_X_ETHNC_NUM && NETIF_X_ETHNC_NUM > 0u
#define LWIP_PBUF_CUSTOM_DATA          T_x_eth_ptp_time  timestamp;
#define LWIP_PBUF_CUSTOM_DATA_INIT(p)  do { p->timestamp.u32_Sec = 0u; p->timestamp.u32_NanoSec = 0u; } while (0)
#define LWIP_SUPPORT_CUSTOM_PBUF       1

#define LWIP_MALLOC_MEMPOOL_MEM_ATTRIBUTE  X_MEM_EMEM_BSS_PUBLIC // Place malloc mempool in uncached EMEM (used in cc.h)
#define DMA_RX_BUFFER_SIZE             256u
#define DMA_RX_BUFFER_COUNT            270u // 68kB, ~45 full-size frames (required for 64kB fragmented UDP packet)
#endif

// Mem pool definition: don't care for lwip_malloc or libc malloc
#define MEMP_NUM_NETBUF                32
#define MEMP_NUM_PBUF                  32
#define PBUF_POOL_SIZE                 0
#define LWIP_DISABLE_TCP_SANITY_CHECKS 0

// Number of simultaneous connections
#define MEMP_NUM_RAW_PCB               4
#define MEMP_NUM_UDP_PCB               10
#define MEMP_NUM_TCP_PCB               10
#define MEMP_NUM_TCP_PCB_LISTEN        10
#define MEMP_NUM_TCP_SEG               32

// Number of connections when using sequential API (socket API)
#define MEMP_NUM_NETCONN               (MEMP_NUM_TCP_PCB + \
                                        MEMP_NUM_UDP_PCB + \
                                        MEMP_NUM_TCP_PCB_LISTEN + \
                                        MEMP_NUM_RAW_PCB)

// IP fragmentation settings
#define IP_FRAG                        1
#define MEMP_NUM_REASSDATA             10 // Num IP packets waiting to be reassemble
#define IP_REASS_MAX_PBUFS             45 // Num pbufs waiting to be reassembled (shall match the number of full
                                          // size RX pbufs (LWIP_MALLOC_MEMPOOL(45, 1540) or  DMA_RX_BUFFER_COUNT)
                                          // A maximum sized UDP paket (64kB) requires 44 full sized IP fragments.

#define MEMP_NUM_ARP_QUEUE             20

#define LWIP_NETIF_STATUS_CALLBACK     1
#define LWIP_NETIF_LINK_CALLBACK       1
#define LWIP_NETIF_HWADDRHINT          1

#define IP_SOF_BROADCAST               1
#define IP_SOF_BROADCAST_RECV          0 // 0: Always receive broadcast to comply with BSD socket API specification

#define LWIP_SO_RCVTIMEO               1 // Support receive timeout socket option
#define LWIP_SO_LINGER                 1 // Soport socket option SO_LINGER
#define SO_REUSE                       1 // Support socket option SO_REUSEADDR
#define SO_REUSE_RXTOALL               1 // Send RX broad-/multicast packets

// -----------------------------------------
// Select supported APIs
// -----------------------------------------
#define LWIP_NETCONN                   (NO_SYS == 0)
#define LWIP_SOCKET                    (NO_SYS == 0)
#define LWIP_COMPAT_SOCKETS            (NO_SYS == 0)
#define LWIP_POSIX_SOCKETS_IO_NAMES    (NO_SYS == 0)
#define LWIP_NETIF_API                 (NO_SYS == 0)
// -----------------------------------------

// -----------------------------------------
// Select supported protocols and interfaces
// -----------------------------------------
// Protocol selection
#define LWIP_BROADCAST_PING            1
#define LWIP_MULTICAST_PING            1
#define LWIP_ARP                       1
#define ETHARP_TRUST_IP_MAC            0
#define LWIP_ICMP                      1
#define LWIP_RAW                       1
#define LWIP_TCP                       1
#define LWIP_TCP_KEEPALIVE             1
#define LWIP_UDP                       1
#define LWIP_UDPLITE                   0
#define LWIP_DHCP                      1
#define LWIP_AUTOIP                    0
#define LWIP_SNMP                      0
#define LWIP_IGMP                      1
#define LWIP_DNS                       1

// TCP specific settings
#define TCP_WND                        (4 * TCP_MSS)
#define TCP_MSS                        1460
#define TCP_SND_BUF                    (8 * TCP_MSS)
#define TCP_LISTEN_BACKLOG             1
// Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing
#define TCP_KEEPIDLE_DEFAULT           10000UL // Default KEEPALIVE timer in milliseconds
#define TCP_KEEPINTVL_DEFAULT          2000UL  // Default Time between KEEPALIVE probes in milliseconds
#define TCP_KEEPCNT_DEFAULT            9U      // Default Counter for KEEPALIVE probes

// Interface settings
#define LWIP_HAVE_LOOPIF               0
#define LWIP_HAVE_SLIPIF               0
#define PPP_SUPPORT                    0
#define PPPOE_SUPPORT                  0
#define PAP_SUPPORT                    0
#define CHAP_SUPPORT                   0
#define LWIP_NETIF_HOSTNAME            1
#define LWIP_SINGLE_NETIF              0 // Use a single netif only (for small targets)

// -----------------------------------------
// VLAN Support
// -----------------------------------------
#define ETHARP_SUPPORT_VLAN            1

// ---------------------------------------------------
// MQTT configuration (default values see mqtt_opts.h)
// ---------------------------------------------------
// Output ring-buffer size, must be able to fit largest outgoing publish message topic+payloads
//#define MQTT_OUTPUT_RINGBUF_SIZE       (1u*1024)
// Number of bytes in receive buffer, must be at least the size of the longest incoming topic + 8
// If one wants to avoid fragmented incoming publish, set length to max incoming topic length + max payload length + 8
//#define MQTT_VAR_HEADER_BUFFER_LEN     (1u*1024)
// Maximum number of pending subscribe, unsubscribe and publish requests to server
//#define MQTT_REQ_MAX_IN_FLIGHT          16
// Use static memory for MQTT client data struct (only one client available)
//#define MQTT_STATIC_CLIENT              1

// Added 4 extra timers for app usage (TFTP requires one extra timer. 3 remain for application use, e.g. by y_mqtt)
#define MEMP_NUM_SYS_TIMEOUT            (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 4)

// -----------------------------------------
// Debug configuration
// -----------------------------------------
#define LWIP_STATS                     1
#define LWIP_STATS_DISPLAY             1
#define LINK_STATS                     1
#define ETHARP_STATS                   0
#define IP_STATS                       0
#define IPFRAG_STATS                   0
#define ICMP_STATS                     0
#define IGMP_STATS                     0
#define UDP_STATS                      0
#define TCP_STATS                      0
#define MEM_STATS                      1
#define MEMP_STATS                     1
#define SYS_STATS                      1

//#define LWIP_DEBUG                     1
#define LWIP_DBG_MIN_LEVEL             LWIP_DBG_LEVEL_WARNING
#define ETHARP_DEBUG                   LWIP_DBG_OFF
#define NETIF_DEBUG                    LWIP_DBG_OFF
#define PBUF_DEBUG                     LWIP_DBG_OFF
#define API_LIB_DEBUG                  LWIP_DBG_OFF
#define API_MSG_DEBUG                  LWIP_DBG_OFF
#define SOCKETS_DEBUG                  LWIP_DBG_OFF
#define ICMP_DEBUG                     LWIP_DBG_OFF
#define INET_DEBUG                     LWIP_DBG_OFF
#define IP_DEBUG                       LWIP_DBG_OFF
#define IP_REASS_DEBUG                 LWIP_DBG_OFF
#define RAW_DEBUG                      LWIP_DBG_OFF
#define MEM_DEBUG                      LWIP_DBG_OFF
#define MEMP_DEBUG                     LWIP_DBG_OFF
#define SYS_DEBUG                      LWIP_DBG_OFF
#define TCP_DEBUG                      LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG               LWIP_DBG_OFF
#define TCP_RTO_DEBUG                  LWIP_DBG_OFF
#define TCP_CWND_DEBUG                 LWIP_DBG_OFF
#define TCP_WND_DEBUG                  LWIP_DBG_OFF
#define TCP_FR_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                 LWIP_DBG_OFF
#define TCP_RST_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                      LWIP_DBG_OFF
#define TCPIP_DEBUG                    LWIP_DBG_OFF
#define PPP_DEBUG                      LWIP_DBG_OFF
#define SLIP_DEBUG                     LWIP_DBG_OFF
#define DHCP_DEBUG                     LWIP_DBG_OFF
#define MQTT_DEBUG                     LWIP_DBG_OFF
// -----------------------------------------

/*------------------- FS OPTIONS -------------------*/
#define LWIP_HTTPD_DYNAMIC_FILE_READ  1

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // LWIPOPTS_H

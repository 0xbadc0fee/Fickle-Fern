//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief TC2xx Ethernet interface / switch / lwip initialization
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_osf.h"
#include "x_eth_swi.h"
#include "x_eth.h"
#include "netif_x_eth.h"
#include "lwip_target.h"
#include "lwip/tcpip.h"
#include "ethernet_init.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Task priorities of the netif and tcpip tasks */
//----------------------------------------------------------------------------------------------------------------------
#define TASK_PRIO_NETIF    12u // Netif task priority
#define TASK_PRIO_TCPIP    13u // TCPIP task priority

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
static sint16 m_eth_switch_init(void);
static sint16 m_eth_netif_init(const uint16 ou16_Channel);
static sint16 m_eth_install_tasks(void);
static void m_tcpip_init_done(void * opv_Arg);

/* -- (Module) Global Variables ------------------------------------------------------------------------------------- */
static struct netif * mapt_NetIf[X_ETH_COUNT];
static volatile uint8 mu8_TcpInitDone = FALSE;

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Ethernet initialization

   This function initializes all components related to Ethernet:
   - Initialize the Ethernet switch
   - Initialize the lwip TCPIP stack

   \retval  C_NO_ERR       No error
   \retval  C_CONFIG       Some initialization error occurred
*/
//----------------------------------------------------------------------------------------------------------------------
sint16 ethernet_init(void)
{
   sint16 s16_Error;

   s16_Error = m_eth_switch_init(); // Initialize Ethernet switch
   if ((s16_Error == C_NO_ERR) || (s16_Error == C_CONFIG))
   {
      // Even when we get C_CONFIG in the previous function, single ports could be initialized successfully.

      tcpip_init(&m_tcpip_init_done, NULL);    // Initialize the lwIP stack (call before netif_add())
      s16_Error += m_eth_netif_init(X_ETH_01); // Initialize all network interfaces
      s16_Error += m_eth_install_tasks();      // Install tasks to handle Ethernet communication
      if (s16_Error != C_NO_ERR)
      {
         s16_Error = C_CONFIG;
      }
   }
   else
   {
      // The HW variant has no Ethernet
      s16_Error = C_NO_ERR;
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Ethernet switch initialization with configuration parameters from NVM

   \retval  C_NO_ERR       No error
   \retval  other          cf. x_osf_switch_get_phy(), x_eth_swi_init()
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_eth_switch_init(void)
{
   sint16 s16_Error;

   X_ETH_SWI_ETH1_DECLARE_PORT_LIST(hau16_PortIds);

   /* Only relevant for ECUs with multiple HW configurations
   {
      uint32 u32_HwConfig;

      s16_Error = x_osf_get_target_cfg(&u32_HwConfig);
      if (s16_Error == C_NO_ERR)
      {
         s16_Error = x_eth_swi_select_hw_config(X_ETH_SWI_ETH1_SWI1, u32_HwConfig);
      }
   }

   if (s16_Error == C_NO_ERR)*/
   {
      T_x_osf_phy t_phy;
      static T_x_eth_swi_config ht_SwitchConfig;

      ht_SwitchConfig.pt_SwitchConfig = NULL;

      // Set PHY configurations for switch
      for (uint8 u8_PhyIndex = 0u; u8_PhyIndex < X_ETH_SWI_PHY_COUNT; u8_PhyIndex++)
      {
         s16_Error = x_osf_switch_get_phy(X_ETH_SWI_ETH1_SWI1, u8_PhyIndex, &t_phy); // Read values from NVM
         if (s16_Error == C_NO_ERR)
         {
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].u16_PortId   = hau16_PortIds[u8_PhyIndex];
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].u8_AutoNeg   = t_phy.u8_AutoNeg;
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].u16_LinkMode = t_phy.u16_LinkMode;
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].u32_Feature  = t_phy.u32_Feature;
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].pr_StatusCallback = NULL;
            ht_SwitchConfig.at_PhyConfig[u8_PhyIndex].pv_StatusCallbackParameter = NULL;
         }
         else
         {
            break;
         }
      }

      if (s16_Error == C_NO_ERR)
      {
         // Initialize Ethernet switch
         s16_Error = x_eth_swi_init(X_ETH_SWI_ETH1_SWI1, &ht_SwitchConfig);
      }
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Ethernet network interface initialization */
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_eth_netif_init(const uint16 ou16_Channel)
{
   sint16 s16_Error;
   T_x_osf_eth_ipv4 t_Ip4Settings;
   T_netif_x_eth_config t_EthConfig =
   {
      .u16_Channel = ou16_Channel,
      .u8_PollIf = FALSE
   };
   // Handle for the lwip network interface
   static struct netif hat_NetIf[X_ETH_COUNT];

   // Read values from NVM
   s16_Error = x_osf_eth_get_ipv4(ou16_Channel, &t_Ip4Settings);
   s16_Error += x_osf_eth_get_mac(ou16_Channel, t_EthConfig.au8_MacAddress);
   s16_Error += x_osf_eth_get_hostname(ou16_Channel, t_EthConfig.acn_Hostname);

   if (s16_Error == C_NO_ERR)
   {
      struct ip4_addr t_IpAddress;
      struct ip4_addr t_Netmask;
      struct ip4_addr t_Gateway;

      IP4_ADDR(&t_IpAddress,
               t_Ip4Settings.au8_Ip[0], t_Ip4Settings.au8_Ip[1],
               t_Ip4Settings.au8_Ip[2], t_Ip4Settings.au8_Ip[3]);
      IP4_ADDR(&t_Netmask,
               t_Ip4Settings.au8_NetMask[0], t_Ip4Settings.au8_NetMask[1],
               t_Ip4Settings.au8_NetMask[2], t_Ip4Settings.au8_NetMask[3]);
      IP4_ADDR(&t_Gateway,
               t_Ip4Settings.au8_Gateway[0], t_Ip4Settings.au8_Gateway[1],
               t_Ip4Settings.au8_Gateway[2], t_Ip4Settings.au8_Gateway[3]);

      // Add network interface to list of lwIP network interfaces
      mapt_NetIf[ou16_Channel] = netif_add(&hat_NetIf[ou16_Channel], &t_IpAddress, &t_Netmask, &t_Gateway,
                                           &t_EthConfig, &netif_x_eth_init, &tcpip_input);
      if (mapt_NetIf[ou16_Channel] != NULL)
      {
         if (ou16_Channel == X_ETH_01)
         {
            netif_set_default(mapt_NetIf[ou16_Channel]); // 1st interface is default interface
         }
         netif_set_up(mapt_NetIf[ou16_Channel]); // Bring the interface up
      }
      else
      {
         s16_Error = C_CONFIG;
      }
   }
   else
   {
      s16_Error = C_CONFIG;
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Install tasks to handle Ethernet communication */
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_eth_install_tasks(void)
{
   sint16 s16_Error;
   T_x_os_task * pt_NetIfTask;
   T_x_os_task * pt_TcpIpTask;
   const T_x_os_task_config t_NetifTaskCfg =
   {
      .u16_Priority = TASK_PRIO_NETIF,
      .pr_Task = &netif_x_eth_task,
      .pv_Parameter = NULL,
      .u32_TimeDelay = 1UL,
      .u32_TimeInterval = 0UL,
      .u32_StackSize = NETIF_THREAD_STACKSIZE,
      .u32_MemClass = X_OS_HEAP_ID_FAST,
      .u32_ConfigFlags = 0U,
      .u16_SafetyLevel = X_OS_TASK_SAFETY_LEVEL_NON_INTERFERING
   };
   const T_x_os_task_config t_TcpipTaskCfg =
   {
      .u16_Priority = TASK_PRIO_TCPIP,
      .pr_Task = &tcpip_thread,
      .pv_Parameter = NULL,
      .u32_TimeDelay = 1UL,
      .u32_TimeInterval = 0UL,
      .u32_StackSize = TCPIP_THREAD_STACKSIZE,
      .u32_MemClass = X_OS_HEAP_ID_FAST,
      .u32_ConfigFlags = 0U,
      .u16_SafetyLevel = X_OS_TASK_SAFETY_LEVEL_NON_INTERFERING
   };

   s16_Error = x_os_install_task(&t_NetifTaskCfg, NULL, &pt_NetIfTask);
   s16_Error += x_os_install_task(&t_TcpipTaskCfg, NULL, &pt_TcpIpTask);

   if (s16_Error == C_NO_ERR)
   {
      // Wait until tcpip_thread is initialized and running
      while (mu8_TcpInitDone != TRUE)
      {
         x_os_sleep(1u);
      }
   }

   return (s16_Error == C_NO_ERR) ? C_NO_ERR : C_CONFIG;
}

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   TCP/IP callback function

   Callback function that is called when the tcpip_thread is initialized and running.
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_tcpip_init_done(void * opv_Arg)
{
   (void)opv_Arg;

   mu8_TcpInitDone = TRUE;
}

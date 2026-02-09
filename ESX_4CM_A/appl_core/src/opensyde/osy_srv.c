//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Server Initialization Wrapper
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <string.h>
#include "osy_dph_init.h"
#include "osy_dph_data_set_handler.h"
#include "osy_dpd_driver.h"
#include "osy_srv.h"
#include "osy_init.h"
#include "can_handler.h"
#include "x_sys.h"
#include "x_sys_rst.h"
#include "x_icc_barrier.h"
#include "x_os.h"
#include "osy_app_j1939.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define APPLICATION_NAME      "ESX-4CM-A openSYDE Server"
#define APPLICATION_VERSION   "v0.01r0"

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- (Module) Global Variables ------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
static sint16 m_osy_srv_init_1(void);
static sint16 m_osy_srv_init_2(void);
static void m_osy_srv_task(void * const opv_TaskParameter);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief Installs a new RTOS task, that executes the DPD.                                                           */
//----------------------------------------------------------------------------------------------------------------------
sint16 osy_srv_init(void)
{
   sint16 s16_Error;
   T_x_os_task * pt_TaskHandle;
   const T_x_os_task_config t_TaskConfig =
   {
      .u16_Priority = OSY_SRV_DPD_TASK_PRIO,
      .pr_Task = &m_osy_srv_task,
      .pv_Parameter = NULL,
      .u32_TimeDelay = OSY_SRV_DPD_TASK_DELAY | 1u, // 0 means suspended
      .u32_TimeInterval = OSY_SRV_DPD_TASK_CYCLE,
      .u32_StackSize = 16uL * 1024u,
      .u32_MemClass = X_OS_HEAP_ID_SRAM,
      .u32_ConfigFlags = 0uL
   };

   s16_Error  = x_icc_barrier_wait_for(1u, X_ICC_BARRIER_TIMEOUT_INFINITE);
   s16_Error += m_osy_srv_init_1();
   s16_Error += m_osy_srv_init_2();
   s16_Error += x_icc_barrier_wait_for(2u, X_ICC_BARRIER_TIMEOUT_INFINITE);
   s16_Error += x_os_install_task(&t_TaskConfig, NULL, &pt_TaskHandle);

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief 1st step of openSYDE server initialization.

   This function initialize the data pool access server.
   It exports its data pools to be imported by the diagnostic protocol driver.
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_osy_srv_init_1(void)
{
   sint16 s16_Error = C_NO_ERR;
   const T_osy_dpa_data_pool * const * const apt_DataPools = osy_dph_get_init_config();
   const uint8 u8_NumDP = osy_dph_get_num_data_pools();

   if (u8_NumDP > 0u)
   {
      // Init data pools
      s16_Error = osy_dph_init_data_pools(apt_DataPools, u8_NumDP);
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief 2nd step of openSYDE server initialization.

   This function is intended for the diagnostic protocol driver DPD, to import the exported data pools from other cores.
   In addition it initialize the DPD.
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_osy_srv_init_2(void)
{
   static T_osy_ctp_trg_can_init_params hat_TrgInitParamsCan[OSY_INIT_DPD_NUMBER_OF_CAN_CHANNELS];
   static T_osy_etp_trg_eth_init_params hat_TrgInitParamsEth[OSY_INIT_DPD_NUMBER_OF_ETHERNET_CHANNELS];
   static const T_osy_dpd_appl_data ht_ApplData =
   {
      hat_TrgInitParamsCan,
      hat_TrgInitParamsEth
   };
   uint8 u8_Channel;
   bool q_MissingCanIf = false;
   bool q_MissingEthIf = false;
   sint16 s16_Error;
   struct netif * pt_NetIf;
   const T_osy_dpd_data * const pt_dpd_data = osy_dpd_get_init_config();

   // Setup CAN init parameter
   for (u8_Channel = 0u; u8_Channel < OSY_INIT_DPD_NUMBER_OF_CAN_CHANNELS; ++u8_Channel)
   {
      const bool q_Available = can_get_availability_state(u8_Channel);

      hat_TrgInitParamsCan[u8_Channel].u16_TxBufferSize = 4u;
      hat_TrgInitParamsCan[u8_Channel].u16_RxBufferSize = 20u;

      if (!q_Available)
      {
         q_MissingCanIf = true;
      }
   }
   // Setup Ethernet init parameter
   for (u8_Channel = 0u; u8_Channel < OSY_INIT_DPD_NUMBER_OF_ETHERNET_CHANNELS; ++u8_Channel)
   {
      s16_Error = netif_x_eth_get_interface_from_channel(u8_Channel, &pt_NetIf);
      if (s16_Error == C_NO_ERR)
      {
         hat_TrgInitParamsEth[u8_Channel].pt_NetworkInterface = pt_NetIf;
      }
      else
      {
         q_MissingEthIf = true;
      }
   }

   // Initialize diagnostic protocol driver
   s16_Error = osy_dpd_init(&ht_ApplData, pt_dpd_data);
   if ((q_MissingEthIf && (s16_Error == C_RANGE)) || (q_MissingCanIf && (s16_Error == C_NOACT)))
   {
      // When the variant has no Ethernet, we get a C_RANGE
      s16_Error = C_NO_ERR;
   }

   // Set application info
   {
      T_osy_dpd_application_information t_ApplInfo;

      (void)memcpy(&t_ApplInfo.acn_Name[0], APPLICATION_NAME, sizeof(APPLICATION_NAME));
      (void)memcpy(&t_ApplInfo.acn_Version[0], APPLICATION_VERSION, sizeof(APPLICATION_VERSION));
      osy_dpd_set_application_information(&t_ApplInfo);
   }

   return s16_Error;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief openSYDE main task.                                                                                        */
//----------------------------------------------------------------------------------------------------------------------
static void m_osy_srv_task(void * const opv_TaskParameter)
{
   uint8 u8_Channel;
   T_osy_uds_ch_channel_identifier t_ChannelIdentifier;

   // Run until a reset request was issued
   while (1 /*!x_sys_rst_ongoing()*/)
   {
      // Note: the channel index always starts with zero and all channels must be handled here.
      // The bus number/interface number (CAN1 = 0, CAN2 = 1,...) is not necessarily identical to the channel index.
      // The mapping between the channel index and the bus number is done via the generated defines
      // OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_x which are used in osy_dpd_get_init_config().
      // Example:
      // When CAN1 is not used in OpenSYDE, then OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0 is "1U" instead of "0U" and so
      // channel 0 is mapped to bus number 1 (CAN2).

      // CAN
      for (u8_Channel = 0u; u8_Channel < OSY_INIT_DPD_NUMBER_OF_CAN_CHANNELS; u8_Channel++)
      {
         t_ChannelIdentifier.u8_ChannelType = OSY_UDS_CH_CHANNEL_TYPE_CAN;
         t_ChannelIdentifier.u8_ChannelIndex = u8_Channel;

         (void)osy_dpd_task(&t_ChannelIdentifier);
      }

      // Ethernet
      for (u8_Channel = 0u; u8_Channel < OSY_INIT_DPD_NUMBER_OF_ETHERNET_CHANNELS; u8_Channel++)
      {
         t_ChannelIdentifier.u8_ChannelType = OSY_UDS_CH_CHANNEL_TYPE_ETHERNET;
         t_ChannelIdentifier.u8_ChannelIndex = u8_Channel;

         (void)osy_dpd_task(&t_ChannelIdentifier);
      }

      (void)osy_app_j1939_cycle();

      // Wait for next period
      x_os_wait_period();
   }
}

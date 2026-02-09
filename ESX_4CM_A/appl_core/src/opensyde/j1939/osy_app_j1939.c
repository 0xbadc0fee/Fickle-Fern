//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief openSYDE Application Handler-Functions
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "osy_app_j1939.h"
#include "osy_com_engine.h"              //openSYDE server communication stack driver functions
#include "osy_init.h"                    //make openSYDE data pools and comm definition known
#include "osy_com_j1939_dm1.h"
#include "osy_com_j1939_dm2.h"
#include "osy_com_j1939_dm_generic.h"
#include "osy_com_j1939_driver.h"
#include "osy_com_j1939_handler.h"
#include "osy_dph_data_pool_protector.h"
#include "osy_com_trg_definitions.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define COMM_STACK_J1939_0_NUMBER_OF_RX_FILTERS (1U)
#define J1939_DM_SOURCE_ADDRESS      (5u)
#define J1939_DM_DESTINATION_ADDRESS (10u)

// RX filters for J1939 (RX range: 0x300 .. 0x3FF)
#define COMM_STACK_RX_FILTERS_J1939(CFG) \
   OSY_COM_TRG_RX_FILTER ## CFG(0U, 0x300U, 0x700U, 10U, OSY_COM_TRG_CAN_RX_MODE_APPL_CORE)
#define APP_OSY_NUM_RX_FILTERS_J1939   1U
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- (Module) Global Variables ------------------------------------------------------------------------------------- */
static T_osy_com_j1939_dtc mac_DmDtcs[445];

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
static void m_J1939HandlerOnAckReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                        const uint8 ou8_DestinationAddress, const uint8 ou8_AckType,
                                        const uint32 ou32_RequestedPgn, const uint8 ou8_GroupFunction);
static void m_J1939HandlerOnRequestReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                            const uint8 ou8_DestinationAddress, const uint32 ou32_RequestedPgn);
static sint16 m_InitJ1939Dm1Tx(void);
static sint16 m_InitJ1939Dm1Rx(void);
static sint16 m_InitJ1939Dm2Tx(void);
static sint16 m_InitJ1939Dm2Rx(void);
// DM1
static void m_J1939Dm1Enabled(const uint8 ou8_Enable);
static void m_J1939Dm1PrepareSend(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress);
static void m_J1939Dm1DtcReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                  const T_osy_com_j1939_dtc_rx * const opc_ReceivedDtc);
static void m_J1939Dm1ErrorRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType);
static void m_J1939Dm1ErrorTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType);

// DM2
static void m_J1939Dm2SendRequest();
static void m_J1939Dm2DtcReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                  const T_osy_com_j1939_dtc_rx * const opc_ReceivedDtc);
static void m_J1939Dm2ErrorRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType);
static void m_J1939Dm2ErrorTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType);
static void m_J1939Dm2AbortRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                              const uint8 ou8_DestinationAddress, const uint8 ou8_WasSent, const uint8 ou8_AbortReason);
static void m_J1939Dm2AbortTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                              const uint8 ou8_DestinationAddress, const uint8 ou8_WasSent, const uint8 ou8_AbortReason);
/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle information about incoming J1939 ACK and NACK messages

   Invoked by "osy_com_engine_handle_rx" if it detects any incoming ACK or NACK message.

   \param[in]  ou8_CanChannel          CAN channel the request was received on
   \param[in]  ou8_SourceAddress       Sender address of request PG
   \param[in]  ou8_DestinationAddress  Destination address of request PG
   \param[in]  ou8_AckType             Type of ACK (see OSY_COM_J1939_ACK_* defines)
   \param[in]  ou32_RequestedPgn       Requested PGN
   \param[in]  ou8_GroupFunction       Group function field of acknowledge
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939HandlerOnAckReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                        const uint8 ou8_DestinationAddress, const uint8 ou8_AckType,
                                        const uint32 ou32_RequestedPgn, const uint8 ou8_GroupFunction)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle information about incoming J1939 Request PG

   Invoked by "osy_com_engine_handle_rx" if it detects any incoming request PG *before* it gets
    evaluated by the COMM stack.

   \param[in]  ou8_CanChannel          CAN channel the request was received on
   \param[in]  ou8_SourceAddress       Sender address of request PG
   \param[in]  ou8_DestinationAddress  Destination address of request PG
   \param[in]  ou32_RequestedPgn       Requested PGN
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939HandlerOnRequestReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                            const uint8 ou8_DestinationAddress, const uint32 ou32_RequestedPgn)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize J1939 DM1 Tx module

   \retval C_NO_ERR
   \retval C_CONFIG
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_InitJ1939Dm1Tx(void)
{
   sint16 s16_Return;

   // TX configuration for sending as J1939 node
   static uint8 hau8_TxTpBuffer[1782];
   static T_osy_com_j1939_dm1_appl_tx_config hc_TxConfig =
   {
      {
         {},                          // Lamp status
         &mac_DmDtcs[0],              // All DTCs
         445U,                        // Maximum count DTCs
         J1939_DM_SOURCE_ADDRESS,     // Source Address
         {},                          // Thread lock
         {0U}                         // Process lock
      },
      &hau8_TxTpBuffer[0], // Tp buffer
      {},                  // Status buffer
      1782U,               // Size Tp buffer
      0U                   // disabled
   };

   static const T_osy_com_j1939_dm1_appl_tx_configs hc_TX_CCONFIGS =
   {
      &hc_TxConfig,
      1U,
   };

   // Init function for DM1 Tx
   s16_Return = osy_com_j1939_dm1_init_tx(OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0, &hc_TX_CCONFIGS);

   return s16_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize J1939 DM1 Rx module

   \retval C_NO_ERR
   \retval C_CONFIG
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_InitJ1939Dm1Rx(void)
{
   sint16 s16_Return;

   //################
   //# J1939 DM1 RX #
   //################

   // Rx configuration
   static T_osy_com_j1939_dm1_appl_rx_config hc_RxConfig =
   {
      J1939_DM_DESTINATION_ADDRESS,     // Source address of the other sender node
      {},                               // Tp buffer
      {}                                // Status buffer
   };

   static const T_osy_com_j1939_dm1_appl_rx_configs hc_RX_CONFIGS =
   {
      &hc_RxConfig,
      1U // Number configs
   };

   const T_osy_com_j1939_dm1_callbacks c_CALLBACKS =
   {
      &m_J1939Dm1DtcReceived, // Callback for getting received DTCs
      &m_J1939Dm1ErrorRx,     // Callback for detected errors in Rx process
      &m_J1939Dm1PrepareSend, // Callback before sending DM1 for preperation of application
      &m_J1939Dm1ErrorTx      // Callback for detected errors in Tx process
   };

   // Init function for DM1 Rx
   s16_Return = osy_com_j1939_dm1_init_rx(OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0, &hc_RX_CONFIGS);

   // Init DM1 callbacks
   osy_com_j1939_dm1_init_callbacks(&c_CALLBACKS);

   return s16_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize J1939 DM2 Tx module

   \retval C_NO_ERR
   \retval C_CONFIG
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_InitJ1939Dm2Tx(void)
{
   sint16 s16_Return;

   // TX configuration for sending as J1939 node
   static T_osy_com_j1939_dm_logic_node_tx_config hc_TxConfig =
   {
      {},                          // Lamp status
      &mac_DmDtcs[0],              // All DTCs
      445U,                        // Maximum count DTCs
      J1939_DM_SOURCE_ADDRESS,     // Source Address
      {},                          // Thread lock
      {0U}                         // Process lock
   };

   static uint8 hau8_TxTpBuffer[1782];

   static const T_osy_com_j1939_dm2_appl_tx_configs hc_TX_CONFIGS =
   {
      &hc_TxConfig,
      1U,                 // Number configs
      1782U,              // Size tp buffer
      &hau8_TxTpBuffer[0] // Tp buffer
   };

   // Init function for DM2 Tx
   s16_Return = osy_com_j1939_dm2_init_tx(OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0, &hc_TX_CONFIGS);

   return s16_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize J1939 DM2 Rx module

   \retval C_NO_ERR
   \retval C_CONFIG
*/
//----------------------------------------------------------------------------------------------------------------------
static sint16 m_InitJ1939Dm2Rx(void)
{
   sint16 s16_Return;

   static T_osy_com_j1939_dm2_appl_rx_configs hc_RxConfigs =
   {
      {} // Tp buffer
   };

   // DM2 Callbacks configuration
   const T_osy_com_j1939_dm2_callbacks c_CALLBACKS =
   {
      &m_J1939Dm2DtcReceived, // Callback for getting received DTCs
      &m_J1939Dm2ErrorRx,     // Callback for detected errors in Rx process
      &m_J1939Dm2AbortRx,     // Callback for J1939 abort messages in Rx process
      &m_J1939Dm2ErrorTx,     // Callback for detected errors in Tx process
      &m_J1939Dm2AbortTx,     // Callback for J1939 abort messages in Tx process
   };

   // Init function for DM2 Rx
   s16_Return = osy_com_j1939_dm2_init_rx(OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0, &hc_RxConfigs);

   // Init DM2 callbacks
   osy_com_j1939_dm2_init_callbacks(&c_CALLBACKS);

   return s16_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   DM1 enabled/disabled
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm1Enabled(const uint8 ou8_Enable)
{

   osy_com_j1939_dm1_enable_tx_cyclic_sending(OSY_INIT_DPD_BUS_NUMBER_CAN_CHANNEL_0, 0U, ou8_Enable);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called before a complete DM1 transfer begins

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of this J1939 node
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm1PrepareSend(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after each part with a complete DTC

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of received DTC
   \param[in]      opc_ReceivedDtc   currently received DTC (lamp status and DTC counts included)
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm1DtcReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                  const T_osy_com_j1939_dtc_rx * const opc_ReceivedDtc)
{
   if (opc_ReceivedDtc != NULL)
   {
      if ((opc_ReceivedDtc->u16_DtcTotalCount == 0) ||
          (opc_ReceivedDtc->u16_DtcIndex == 0))
      {
         const T_osy_com_j1939_dm_lamp_status * const pc_Lamp = &opc_ReceivedDtc->t_Lamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_DTCReceivedCnt += 1u;
         //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_AmberWarnLamp = pc_Lamp->u8_AmberWarnLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_MalfIndLamp = pc_Lamp->u8_MalfIndLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_ProtectLamp = pc_Lamp->u8_ProtectLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_RedStopLamp = pc_Lamp->u8_RedStopLamp;
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The function is called after a detected error when receiving a DM1 package

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of sender of received DTC
   \param[in]      ou8_ErrorType     detected error type
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm1ErrorRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType)
{
   //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_ErrorRx += 1u;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after a detected error when sending a DM1 package

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of this J1939 sender node
   \param[in]      ou8_ErrorType     detected error type
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm1ErrorTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType)
{
   //gt_J1939Datapool_DataPoolValues.t_Dm1Values.u8_Dm1_ErrorTx += 1u;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after each part with a complete DTC

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of received DTC
   \param[in]      opc_ReceivedDtc   currently received DTC (lamp status and DTC counts included)
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm2DtcReceived(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                                  const T_osy_com_j1939_dtc_rx * const opc_ReceivedDtc)
{
   if (opc_ReceivedDtc != NULL)
   {
      if ((opc_ReceivedDtc->u16_DtcTotalCount == 0) ||
          (opc_ReceivedDtc->u16_DtcIndex == 0))
      {
         const T_osy_com_j1939_dm_lamp_status * const pc_Lamp = &opc_ReceivedDtc->t_Lamp;

         //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_DTCReceivedCnt += 1u;
         //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_AmberWarnLamp = pc_Lamp->u8_AmberWarnLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_MalfIndLamp = pc_Lamp->u8_MalfIndLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_ProtectLamp = pc_Lamp->u8_ProtectLamp;
         //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_RedStopLamp = pc_Lamp->u8_RedStopLamp;
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The function is called after a detected error when receiving a DM2 package

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of sender of received DTC
   \param[in]      ou8_ErrorType     detected error type
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm2ErrorRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType)
{
   //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_ErrorRx += 1u;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after a detected error when sending a DM2 package

   \param[in]      ou8_CanChannel    selected CAN channel
   \param[in]      ou8_SourceAddress source address of this J1939 sender node
   \param[in]      ou8_ErrorType     detected error type
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm2ErrorTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress, const uint8 ou8_ErrorType)
{
   //gt_J1939Datapool_DataPoolValues.t_Dm2Values.u8_Dm2_ErrorTx += 1u;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after a abort message event when receiving a DM2 package

   \param[in]      ou8_CanChannel         selected CAN channel
   \param[in]      ou8_SourceAddress      source address of sender of message
   \param[in]      ou8_DestinationAddress destination address of message
   \param[in]      ou8_WasSent            Flag if abort message was sent or received by this node
   \param[in]      ou8_AbortReason        reason of abort message
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm2AbortRx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                              const uint8 ou8_DestinationAddress, const uint8 ou8_WasSent, const uint8 ou8_AbortReason)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The callback is called after a abort message event when sending a DM2 package

   \param[in]      ou8_CanChannel         selected CAN channel
   \param[in]      ou8_SourceAddress      source address of sender of message
   \param[in]      ou8_DestinationAddress destination address of message
   \param[in]      ou8_WasSent            Flag if abort message was sent or received by this node
   \param[in]      ou8_AbortReason        reason of abort message
*/
//----------------------------------------------------------------------------------------------------------------------
static void m_J1939Dm2AbortTx(const uint8 ou8_CanChannel, const uint8 ou8_SourceAddress,
                              const uint8 ou8_DestinationAddress, const uint8 ou8_WasSent, const uint8 ou8_AbortReason)
{
}
//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize J1939 communication stack

   \retval C_NO_ERR
   \retval C_CONFIG
*/
//----------------------------------------------------------------------------------------------------------------------
sint16 osy_app_j1939_init(void)
{
   sint16 s16_Return;
   T_osy_com_j1939_handler_callbacks c_Callbacks;

   static T_osy_com_rx_filter hac_RxFilterJ1939[COMM_STACK_J1939_0_NUMBER_OF_RX_FILTERS] =
   {
      //11bit, filter, mask,  buffer-size
      {1U,     0x000,  0x000, 10U}
   };
   static const T_osy_com_buffer_config hc_COMM_STACK_J1939_0_BUFFER_CONFIG =
   {
      COMM_STACK_J1939_0_NUMBER_OF_RX_FILTERS, ///< number of defined RX filters
      1U,                                      ///< TX buffer size in number of messages
      &hac_RxFilterJ1939[0]                    ///< pointer to array with u8_NumFilters filters
   };

   OSY_COM_TRG_CREATE_COMM_RX_FILTER_MODE_VARIABLE(COMM_STACK_RX_FILTERS_J1939, au8_MyCommStackFilterModeJ1939,
                                                   APP_OSY_NUM_RX_FILTERS_J1939)

   T_osy_com_trg_can_init_params ac_TrgInitParamsCan =
   {
      .pau8_ModeRx = &au8_MyCommStackFilterModeJ1939[0],
      .u16_CanClientServerStartId = 0U,
      .u16_SafetyCoreTxBufferSize = 0U
   };


   s16_Return = osy_com_engine_init(&gt_comm_j1939_can1_ProtocolConfiguration,
                                    &hc_COMM_STACK_J1939_0_BUFFER_CONFIG, &ac_TrgInitParamsCan);


   if (s16_Return == C_NO_ERR)
   {
      s16_Return |= osy_com_engine_start(&gt_comm_j1939_can1_ProtocolConfiguration);
   }


   c_Callbacks.pr_AckReceivedEvent = &m_J1939HandlerOnAckReceived;
   c_Callbacks.pr_RequestPgReceivedEvent = &m_J1939HandlerOnRequestReceived;

   // J1939 Handler generic initialization
   //make callbacks known to stack:
   if (s16_Return == C_NO_ERR)
   {
      s16_Return = osy_com_j1939_handler_init(&c_Callbacks);
   }

   // DM1 initialization
   if (s16_Return == C_NO_ERR)
   {
      s16_Return = m_InitJ1939Dm1Tx();
   }

   if (s16_Return == C_NO_ERR)
   {
      s16_Return = m_InitJ1939Dm1Rx();
   }

   //Enable DM1 Cyclic Sending
   m_J1939Dm1Enabled(TRUE);

   // DM2 initialization
   if (s16_Return == C_NO_ERR)
   {
      s16_Return = m_InitJ1939Dm2Tx();
   }

   if (s16_Return == C_NO_ERR)
   {
      s16_Return = m_InitJ1939Dm2Rx();
   }

   return s16_Return;
}

sint16 osy_app_j1939_cycle(void)
{
   sint16 s16_Return = C_NO_ERR;

   // Rx Messages----------------------------------
   (void)osy_com_engine_handle_rx(&gt_comm_j1939_can1_ProtocolConfiguration);

   // Tx Messages----------------------------------
   (void)osy_com_engine_handle_tx(&gt_comm_j1939_can1_ProtocolConfiguration);

   return s16_Return;
}

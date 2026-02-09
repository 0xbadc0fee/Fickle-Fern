//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Keeps track of the active transmission per active client connection.
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_DPD_CYCLICTRANSMISSIONHANDLERH
#define OSY_DPD_CYCLICTRANSMISSIONHANDLERH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/* -- Types --------------------------------------------------------------------------------------------------------- */
/*STW Deviation Coding Rules v4*/
/*Violated Rule: 19.2*/
/*Circumstances: object type union defined*/
/*Reason: getting the packed data of the data pool and differentiated variant records used*/
/*Potential risks: possible interoperability problem*/
/*Prevention of risks: use only compatible environment*/
/*PRQA S 0750 ++*/
///Container for "variant" DP content
typedef union
{
   uint32 u32_Value;
   uint16 au16_Value[2];
   uint8 au8_Value[4];
   sint32 s32_Value;
   sint16 as16_Value[2];
   sint8 as8_Value[4];
   float32 f32_Value;
} U_osy_dpd_ed_range_value;
/*PRQA S 0750 --*/

///struct with transmission configuration
typedef struct
{
   uint8 u8_Active;                      ///< see "DPD_CY_" defines
   uint8 u8_DataRateVariant;             ///< three different times are configurable
   uint32 u32_NextSendTime;              ///< used timestamp for compare
   U_osy_dpd_ed_range_value u_Threshold; ///< absolute value for change driven compare
   U_osy_dpd_ed_range_value u_MinValue;  ///< minimum value for compare
   U_osy_dpd_ed_range_value u_MaxValue;  ///< maximum value for compare
   U_osy_dpd_ed_range_value u_LastValue; ///< last changed value

   uint16 u16_ListIndex;    ///< list index number of data pool
   uint16 u16_ElementIndex; ///< element number of data pool list
   uint8 u8_DataPoolIndex;  ///< data pool index number
} T_osy_dpd_ed_transmission;

///struct with all transmissions
typedef struct
{
   const uint8 u8_MaxNumTransmissions;
   T_osy_dpd_ed_transmission * const pt_Transmissions;

   uint16 * const pu16_DataRates; ///< pointer to three currently configured data rates
} T_osy_dpd_ed_transmissions;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
extern sint16 osy_dpd_ed_add_transmission_cyclic(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable,
                                                 const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                                 const uint16 ou16_ElementIndex, const uint8 ou8_DataRateVariant);
extern sint16 osy_dpd_ed_add_transmission_change_driven(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable,
                                                        const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                                        const uint16 ou16_ElementIndex, const uint8 ou8_DataRateVariant,
                                                        const uint32 ou32_Threshold);
extern sint16 osy_dpd_ed_handle_transmission(T_osy_dpd_ed_transmission * const opt_Transmission,
                                             uint8 * const opu8_Data, uint16 * const opu16_Size,
                                             const uint16 opu16_DataRates[3]);
extern void osy_dpd_ed_stop_all_transmissions(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable);
extern sint16 osy_dpd_ed_stop_transmission(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable,
                                           const uint8 ou8_DataPoolIndex, const uint16 ou16_ListIndex,
                                           const uint16 ou16_ElementIndex);
extern sint16 osy_dpd_ed_set_data_rate_time(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable,
                                            const uint16 ou16_DataRateVariant, const uint16 ou16_DataRateTime);
extern sint16 osy_dpd_ed_get_data_rate_time(const T_osy_dpd_ed_transmissions * const opt_TransmissionTable,
                                            const uint16 ou16_DataRateVariant, uint16 * const opu16_DataRateTime);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif

//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Error log types

   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_ERRLOG_TYPES_H___
#define X_ERRLOG_TYPES_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_stdtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ERRLOG_MODULE_USER        (0u)

#define X_ERRLOG_FLAG_SAFETY_CRITICAL ((uint16)(1uL << 8u))  //!< Event flag: critical
#define X_ERRLOG_FLAG_SAFETY_FATAL    ((uint16)(1uL << 9u))  //!< Event flag: fatal
#define X_ERRLOG_FLAG_STARTUP         ((uint16)(1uL << 10u)) //!< Event happened during startup (not avaliable to user)

#define X_ERRLOG_USER_FLAG_MASK ((uint16)(((uint16)0x00FFu) | X_ERRLOG_FLAG_SAFETY_CRITICAL | \
                                          X_ERRLOG_FLAG_SAFETY_FATAL))

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! \brief Event user data
typedef struct
{
   uint16 u16_Flags; //!< Error flags, see X_ERRLOG_FLAG_* and user defined bits in X_ERRLOG_USER_FLAG_MASK
   uint8 u8_Domain;  /*!< Domain identifier:
                          - 0:         user domain
                          - otherwise: system domains */
   uint8 u8_ErrorId; //!< Error Id
   uint16 u16_Data0; //!< Error specific data 0
   uint32 u32_Data1; //!< Error specific data 1
   uint32 u32_Data2; //!< Error specific data 2
   uint32 u32_Data3; //!< Error specific data 3
} T_x_errlog_error;

//! \brief Event meta-data
typedef struct
{
   uint32 u32_Sequence;  //!< event sequence id, overflow counter
   uint32 u32_Timestamp; //!< system time in milliseconds
   uint16 u16_Age;       //!< age in reset cycles
   uint8 u8_Core;        /*!< origin core number
                              - [0 .. Num Cores[ */
} T_x_errlog_info;

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Error enumeration callback prototype

   \param[in]   opt_Error   Error data
   \param[in]   opt_Info    Additional error info

   \return   #TRUE    Continue enumeration
   \return   #FALSE   Terminate enumeration
*/
//----------------------------------------------------------------------------------------------------------------------
typedef uint8 (* PR_x_errlog_enumeration_callback)(const T_x_errlog_error * const opt_Error,
                                                   const T_x_errlog_info * const opt_Info, void * const opv_Argument);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_ERRLOG_TYPES_H___

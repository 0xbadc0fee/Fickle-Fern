//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief     Message Queue non-Safety-API for Inter-Core-Communication

   This module defines the application interface for the Message Queues of the Inter-Core-Communication
   for non-Safety Cores.

   \copyright   Copyright Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  X_ICC_MQ_H
#define  X_ICC_MQ_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_memtypes.h"
#include "x_icc_sig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_ICC_MQ_MEMCLASS_INT_PRIVAT_RAM       1u //!< Obsolete: Core's private DSP_RAM
#define X_ICC_MQ_MEMCLASS_INT_SHARED_RAM       2u //!< Obsolete: LMU RAM
#define X_ICC_MQ_MEMCLASS_EXT_RAM              3u //!< Obsolete: External RAM
#define X_ICC_MQ_MEMCLASS_DEFAULT              4u //!< Default memclass

#define X_ICC_MQ_MODE_RD                       1u //!< Read MQ with support of using a callback function
#define X_ICC_MQ_MODE_RD_SIG                   4u //!< Read MQ with support of using the xb_icc_mq_wait_for function
#define X_ICC_MQ_MODE_WR                       2u //!< Write into MQ
#define X_ICC_MQ_MODE_WR_SIG                   3u //!< Write into MQ with signal

#define X_ICC_MQ_OS_NOWAIT                     X_ICC_SIG_OS_NOWAIT
#define X_ICC_MQ_OS_INFINITE                   X_ICC_SIG_OS_INFINITE

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Declare memory to be used for a message queue (cf. x_icc_mq_create_static())

   Example declaration:
   static X_ICC_MQ_MEMORY(10, 20, X_MEM_SHARED_DATA) \

   \param[in]     msgCount       Number of message queue entries
   \param[in]     msgSize        Size of a message queue entry
   \param[in]     memClass       Memory class attribute, cf. x_memtypes.h (e.g. X_MEM_SHARED_DATA)
                                 A uncached memory region shall be used!
*/
//----------------------------------------------------------------------------------------------------------------------
#define X_ICC_MQ_MEMORY(msgCount, msgSize, memClass) \
   memClass struct  { \
      uint32 au32_Header[7]; \
      uint32 au32_Buffer[(((msgCount) * \
                           (((msgSize) + sizeof(uint32) + (sizeof(uint32) - 1u)) & ~(sizeof(uint32) - 1u))) / \
                          sizeof(uint32))]; \
   }

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Calculate the memory size required for a message queue

   \param[in]     msgCount       Number of message queue entries
   \param[in]     msgSize        Size of a message queue entry
*/
//----------------------------------------------------------------------------------------------------------------------
#define X_ICC_MQ_MEMORY_SIZE(msgCount, msgSize) \
   ((sizeof(uint32) * 7u) + \
    ((msgCount) * \
     (((msgSize) + sizeof(uint32) + (sizeof(uint32) - 1u)) & ~(sizeof(uint32) - 1u))))

/* -- Types --------------------------------------------------------------------------------------------------------- */

//! Shared memory handle structure (Opaque definition).
/*STW Deviation Coding Rules v4*/
/*Violated Rule: */
/*Circumstances: SW Architecture*/
/*Reason: The pointer to the structure is never dereferenced, thus implementation of the object is hidden*/
/*Potential risks: none*/
/*Prevention of risks:  opaque type, nothing to do*/
/*PRQA S 3313 1*/
typedef struct T_x_icc_mq_struct T_x_icc_mq;
typedef void (* PR_x_icc_mq_callback)(T_x_icc_mq * const opt_Handle, void * const opv_CallbackParameter);

//! Message queue configuration structure
typedef struct
{
   uint16 u16_Id;                      //!< Message queue id
   uint32 u32_Mode;                    //!< Open mode (X_ICC_MQ_MODE_..)
   uint32 u32_MsgSize;                 //!< Size of a message queue element (in bytes)
   uint32 u32_MsgCount;                //!< Size of message queue (number of elements)
   uint32 u32_MemClass;                //!< Memory class to allocate shared memory from (X_ICC_MQ_MEMCLASS_DEFAULT)
   PR_x_icc_mq_callback pr_RxCallback; //!< Rx callback function
   void * pv_CallbackParameter;        //!< Callback parameter for RX callback function
} T_x_icc_mq_config;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_icc_mq_create(const T_x_icc_mq_config * const opt_Config, T_x_icc_mq ** const oppt_Handle);

sint16 x_icc_mq_create_static(const T_x_icc_mq_config * const opt_Config, void * const opv_Mem,
                              const uint32 ou32_MemSize, T_x_icc_mq ** const oppt_Handle);

sint16 x_icc_mq_attach(const T_x_icc_mq_config * const opt_Config, T_x_icc_mq ** const oppt_Handle,
                       uint32 * const opu32_MsgSize, uint32 * const opu32_MsgCount);

sint16 x_icc_mq_write(T_x_icc_mq * const opt_Handle, const void * const opv_Data, const uint32 ou32_Size);

sint16 x_icc_mq_read(T_x_icc_mq * const opt_Handle, void * const opv_Data, const uint32 ou32_MaxSize,
                     uint32 * const opu32_Size);

sint16 x_icc_mq_peek(T_x_icc_mq * const opt_Handle, const uint32 ou32_Index, void * const opv_Data,
                     const uint32 ou32_MaxSize, uint32 * const opu32_Size);

sint16 x_icc_mq_remove(T_x_icc_mq * const opt_Handle, const uint32 ou32_MsgCount);

sint16 x_icc_mq_get_msg_count(T_x_icc_mq * const opt_Handle, uint32 * const opu32_MsgCount);

sint16 x_icc_mq_get_free_space(T_x_icc_mq * const opt_Handle, uint32 * const opu32_MsgCount);

sint16 x_icc_mq_wait_for(T_x_icc_mq * const opt_Handle, const uint32 ou32_TimeOut);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif

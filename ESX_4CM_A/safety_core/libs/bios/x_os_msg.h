//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief  RTOS flexible messaging support

   \copyright   Copyright 2022 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OS_MSG_H___
#define X_OS_MSG_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_stdtypes.h"
#include "x_os_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_OS_MSG_ALLOCATOR_TYPE_POOL 1u
#define X_OS_MSG_ALLOCATOR_TYPE_HEAP 2u

#define X_OS_MSG_HEADER_SIZE        24u

#define X_OS_MSG_ALIGNMENT           8u

#define X_OS_MSG_PAYLOAD_ALIGN(SIZE) (((SIZE) + (X_OS_MSG_ALIGNMENT - 1u)) & (X_OS_MSG_ALIGNMENT - 1u))

/*STW Deviation Coding Rules v4*/
/*Violated Rule: No MISRA C:2012 Rules applicable to message 3456*/
/*Circumstances: SW Architecture*/
/*Reason: Static assertion valuable*/
/*Potential risks: User supplies argument with side-effects on evaluation*/
/*Prevention of risks: Document that side-effect-free constant expressions are expected as argument to
                       MAX_PAYLOAD_SIZE.*/
/*PRQA S 3456 5*/
#define X_OS_MSG_POOL_BLOCK(MAX_PAYLOAD_SIZE, COUNT)                                       \
   ALIGNED(32) struct {                                                                    \
      _Static_assert((MAX_PAYLOAD_SIZE) % X_OS_MSG_ALIGNMENT == 0u, "Payload size must be multiple of 8"); \
      uint8 au8_Data[((MAX_PAYLOAD_SIZE) + (X_OS_MSG_HEADER_SIZE)) * (COUNT)];             \
   }

/* -- Types --------------------------------------------------------------------------------------------------------- */

typedef uint32 T_x_os_handle;

/*STW Deviation Coding Rules v4*/
/*Violated Rule: No MISRA C:2012 or STW Rules applicable to message 3313*/
/*Circumstances: SW Architecture */
/*Reason: Encapsulation*/
/*Potential risks: None, this prevents risks*/
/*Prevention of risks: Opaque forward declarations*/
/*PRQA S 3313 ++*/
typedef struct T_xb_icc_msg_struct T_x_os_msg;
typedef struct T_xb_icc_msg_allocator_struct T_x_os_msg_allocator;
typedef struct T_xb_os_msg_queue_handle_struct T_x_os_msg_queue;

/*PRQA S 3313 --*/

//! \brief   Pool allocator stats
typedef struct
{
   uint32 u32_Capacity; //!< Capacity in number of elements
   uint32 u32_ElemSize; //!< Element size in bytes (this will exceed the requested payload size due to the header)
   uint32 u32_CurCount; //!< Number of currently allocated messages
   uint32 u32_MaxCount; //!< Maximum number of elements that have been allocated at the same time
} T_x_os_msg_allocator_pool_stats;
typedef T_x_os_msg_allocator_pool_stats T_x_os_msg_pool_stats;

//! \brief   Heap allocator stats
typedef struct
{
   uint32 u32_Quota;    //! Maximum amount of bytes to be allocated through the heap allocator
   uint32 u32_CurCount; //! Current number of messages allocated through the heap allocator
   uint32 u32_CurSize;  //! Cumulative size of currently allocated message objects in bytes
   uint32 u32_MaxCount; //! Maximum number of allocated message objects in bytes
   uint32 u32_MaxSize;  //! Maximum allocated cumulative size of messages in bytes
} T_x_os_msg_allocator_heap_stats;
typedef T_x_os_msg_allocator_heap_stats T_x_os_msg_heap_stats;

//! \brief   Opaque endpoint handle
typedef struct
{
   T_x_os_handle u32_Handle;
} T_x_os_msg_endpoint;

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Callback endpoint function prototype

   \param[in]   opv_Argument   Argument passed during call to ::x_os_msg_make_callback_endpoint()
   \param[in]   opt_Msg        Arriving message
*/
//----------------------------------------------------------------------------------------------------------------------
typedef void (* PR_x_os_msg_callback_endpoint)(void * opv_Argument, const T_x_os_msg * const opt_Msg);

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

void x_os_msg_get_proxy_stats(T_x_os_msg_allocator_pool_stats * const opt_Status);
void x_os_msg_clear_proxy_stats(void);

// -- Task endpoint API ------------------------------------------------------------------------------------------------

sint16 x_os_msg_make_task_endpoint(const uint8 ou8_AccessLevel, T_x_os_msg_endpoint * const opt_OutResult);

// -- Generic endpoint API ---------------------------------------------------------------------------------------------

sint16 x_os_msg_publish_endpoint_named(const T_x_os_msg_endpoint ot_Endpoint, const charn * const opcn_Name);
sint16 x_os_msg_find_endpoint_named(const charn * const opcn_Name, const uint8 ou8_MinSafetyLevel,
                                    T_x_os_msg_endpoint * const opt_OutResult);
sint16 x_os_msg_wait_for_endpoint_named(const charn * const opcn_Name, const uint8 ou8_MinSafetyLevel,
                                        const uint32 ou32_Timeout, T_x_os_msg_endpoint * const opt_OutResult);

sint16 x_os_msg_publish_endpoint_numeric(const T_x_os_msg_endpoint ot_Endpoint, const uint32 ou32_Id);
sint16 x_os_msg_find_endpoint_numeric(const uint32 ou32_Id, const uint8 ou8_MinSafetyLevel,
                                      T_x_os_msg_endpoint * const opt_OutResult);
sint16 x_os_msg_wait_for_endpoint_numeric(const uint32 ou32_Id, const uint8 ou8_MinSafetyLevel,
                                          const uint32 ou32_Timeout, T_x_os_msg_endpoint * const opt_OutResult);

// -- Allocate messages ------------------------------------------------------------------------------------------------

T_x_os_msg_allocator * x_os_msg_get_default_heap(void);
sint16 x_os_msg_make_heap(const uint32 ou32_Quota, T_x_os_msg_allocator ** const oppt_OutResult);
sint16 x_os_msg_pool_from_block(const uint16 ou16_MaxPayload, void * const opv_Block, const uint32 ou32_BlockSize,
                                T_x_os_msg_allocator * const opt_Fallback,
                                T_x_os_msg_allocator ** const oppt_OutAllocator);
sint16 x_os_msg_pool_from_heap(const uint16 ou16_MaxPayload, const uint32 ou32_Capacity,
                               T_x_os_msg_allocator * const opt_Fallback, T_x_os_msg_allocator ** const opt_OutResult);
sint16 x_os_msg_allocator_get_type(const T_x_os_msg_allocator * const opt_Allocator, uint16 * const opu16_OutResult);
sint16 x_os_msg_allocator_get_pool_stats(const T_x_os_msg_allocator * const opt_Allocator,
                                         T_x_os_msg_allocator_pool_stats * const opt_OutResult);
sint16 x_os_msg_allocator_get_heap_stats(const T_x_os_msg_allocator * const opt_Allocator,
                                         T_x_os_msg_allocator_heap_stats * const opt_OutResult);
sint16 x_os_msg_allocator_clear_stats(T_x_os_msg_allocator * const opt_Allocator);

// - Work on messages --------------------------------------------------------------------------------------------------

sint16 x_os_msg_make_msg(const uint16 ou16_Size, T_x_os_msg_allocator * const opt_Allocator, const uint32 ou32_Timeout,
                         T_x_os_msg ** const oppt_OutResult);
sint16 x_os_msg_make_response(const T_x_os_msg * const opt_Request, const uint16 ou16_Size,
                              T_x_os_msg_allocator * const opt_Allocator, const uint32 ou32_Timeout,
                              T_x_os_msg ** const oppt_OutResult);
sint16 x_os_msg_make_response_inplace(const T_x_os_msg * const opt_Request, const uint16 ou16_Size,
                                      T_x_os_msg ** const oppt_OutResponse);
void * x_os_msg_modify(T_x_os_msg * const opt_Msg);
const void * x_os_msg_read(const T_x_os_msg * const opt_Msg);
uint16 x_os_msg_get_size(const T_x_os_msg * const opt_Msg);
T_x_os_msg_endpoint x_os_msg_get_origin(const T_x_os_msg * const opt_Msg);
T_x_os_msg_endpoint x_os_msg_get_destination(const T_x_os_msg * const opt_Msg);
sint16 x_os_msg_get_allocator_handle(const T_x_os_msg * const opt_Msg, T_x_os_msg_allocator ** const oppt_OutAllocator);
uint8 x_os_msg_get_safety_level(const T_x_os_msg * const opt_Msg);
uint32 x_os_msg_is_request(const T_x_os_msg * const opt_Msg);
uint32 x_os_msg_is_response(const T_x_os_msg * const opt_Msg);
const T_x_os_msg * x_os_msg_get_request(const T_x_os_msg * const opt_Response);
sint16 x_os_msg_discard(const T_x_os_msg * const opt_Msg);

// -- Send Messages ----------------------------------------------------------------------------------------------------

sint16 x_os_msg_send_msg(const T_x_os_msg_endpoint ot_Origin, const T_x_os_msg_endpoint ot_Destination,
                         T_x_os_msg * const opt_Msg);

// -- Request/Response -------------------------------------------------------------------------------------------------

sint16 x_os_msg_send_request_parallel(const T_x_os_msg_endpoint ot_Origin, const T_x_os_msg_endpoint ot_Destination,
                                      T_x_os_msg * const opt_Request);
sint16 x_os_msg_send_request_synch(const T_x_os_msg_endpoint ot_Destination, T_x_os_msg * const opt_Request,
                                   const uint32 ou32_Timeout, const T_x_os_msg ** const oppt_OutResponse);
sint16 x_os_msg_send_response(T_x_os_msg * const opt_Response);

// -- Msg queue API ----------------------------------------------------------------------------------------------------

sint16 x_os_msg_make_queue(const uint8 ou8_AccessLevel, T_x_os_msg_queue ** const oppt_OutQueue);
sint16 x_os_msg_queue_get_endpoint(const T_x_os_msg_queue * const opt_Queue,
                                   T_x_os_msg_endpoint * const opt_OutEndpoint);
sint16 x_os_msg_queue_get_message(T_x_os_msg_queue * const opt_Queue, const uint32 ou32_Timeout,
                                  const T_x_os_msg ** const oppt_OutResult);

// -- Callback endpoint API --------------------------------------------------------------------------------------------

sint16 x_os_msg_make_callback_endpoint(T_x_os_msg_endpoint * const opt_Handle, const uint8 ou8_AccessLevel,
                                       const PR_x_os_msg_callback_endpoint opr_Callback, void * const opv_Argument);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check if endpoint handle is considered 'good'

   \param[in]   ot_Endpoint   Endpoint handle

   \retval   true    Endpoint handle is good
   \retval   false   otherwise
*/
//----------------------------------------------------------------------------------------------------------------------
static inline _Bool x_os_msg_endpoint_good(const T_x_os_msg_endpoint ot_Endpoint)
{
   return ot_Endpoint.u32_Handle != 0u;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check if two endpoint handles reference the same endpoint

   \param[in]   ot_Endpoint1   Endpoint handle to first message
   \param[in]   ot_Endpoint2   Endpoint handle to second message

   \retval   true    Endpoint handles reference the same endpoint
   \retval   false   otherwise
*/
//----------------------------------------------------------------------------------------------------------------------
static inline _Bool x_os_msg_endpoint_equal(const T_x_os_msg_endpoint ot_Endpoint1,
                                            const T_x_os_msg_endpoint ot_Endpoint2)
{
   return ot_Endpoint1.u32_Handle == ot_Endpoint2.u32_Handle;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Create 'empty' endpoint

   To be used for anonymous messages as blank origin endpoint handle

   \return   Returns generic 'nil' endpoint handle
*/
//----------------------------------------------------------------------------------------------------------------------
static inline T_x_os_msg_endpoint x_os_msg_endpoint_nil(void)
{
   return (T_x_os_msg_endpoint){
             0u
   };
}

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif // X_OS_MSG_H___

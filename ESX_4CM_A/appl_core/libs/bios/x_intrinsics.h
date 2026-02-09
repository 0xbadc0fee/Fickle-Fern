//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      C interface for TriCore special machine instructions
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_INTRINSICS_H
#define X_INTRINSICS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "gcc_attributes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define CACHE_LINE_SIZE          (32u)         //!< size of a TriCore V1.6 data cache line
#define UNCACHED_SEGMENT_ADDR    (0x20000000u) //!< define to create a non-cached address (logic OR)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a compiler fence (does not introduce any code)

             The compiler optimizer is not allowed to reorder code or load/store memory operations
             before/after this statement
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void compiler_fence(void)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("" : : : "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a nop command (no operation)
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void nop(void)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("nop" : : : "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    debug command: setup a break event, if debug engine is enabled
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void debug(void)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("debug" : : : "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a instruction sync command
             1. Wait until all instructions semantically prior to the ISYNC have completed.
             2. Flush the CPU pipeline and cancel all instructions semantically after the ISYNC.
             3. Invalidate all cached state in the pipeline.
             4. Re-Fetch the next instruction after the ISYNC.
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void isync(void)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("isync" : : : "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a data sync command
             The DSYNC instruction guarantees that all data accesses associated
             with instructions semantically prior to the DSYNC instruction are completed before any
             data memory accesses associated with an instruction semantically after DSYNC are
             initiated. This includes all accesses to the system bus and local data memory.
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void dsync(void)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("dsync" : : : "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    read a single bit from word u32_DataReg at u32_Pos

   \param[in]    ou32_DataReg    32bit memory/register that should be read
   \param[in]    ou32_Pos        bit position  [0..31]

   \return   u32_Bit     bit data      [0..1]
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 get_bit(const uint32 ou32_DataReg, const uint32 ou32_Pos)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Bit = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("extr.u %0, %1, %2, 1" : "=d" (u32_Bit) : "d" (ou32_DataReg), "d" (ou32_Pos));

   return u32_Bit;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    read a bit field from word u32_DataReg at u32_Pos

   \param[in]    ou32_DataReg    32bit memory/register that should be read
   \param[in]    ou32_Pos        bit position    [0..31]
   \param[in]    ou8_Width       bit field width [1..31]

   \return   u32_Bitfield bitfield data
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 get_bitfield(const uint32 ou32_DataReg, const uint32 ou32_Pos, const uint8 ou8_Width)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Bitfield = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("extr.u %0, %1, %2, %3"
                 : "=d" (u32_Bitfield) : "d" (ou32_DataReg), "d" (ou32_Pos), "i" (ou8_Width));

   return u32_Bitfield;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   read a bit field (width = ou8_Width) from ou32_Value and treat it
            like a signed value (extend sign bit to MSB of u32_Bitfield)

   \param[in]    ou32_DataReg    32bit memory/register that should be read
   \param[in]    ou32_Pos        bit position    [0..31]
   \param[in]    ou8_Width       bit field width [1..31]

   \return  u32_Bitfield   bitfield data
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 get_bitfield_signed(const uint32 ou32_DataReg, const uint32 ou32_Pos, const uint8 ou8_Width)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Bitfield = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("extr %0, %1, %2, %3"
                 : "=d" (u32_Bitfield) : "d" (ou32_DataReg), "d" (ou32_Pos), "i" (ou8_Width));

   return u32_Bitfield;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a single bit into u32_DataReg at u32_Pos

   \param[in]    ou32_DataReg    32bit data register that should be written
   \param[in]    ou32_Pos        bit position  [0..31]
   \param[in]    ou32_Bit        bit data      [0..1]

   \return   modified data register
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 insert_bit(const uint32 ou32_DataReg, const uint32 ou32_Pos, const uint32 ou32_Bit)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Data = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("insert %0,%1,%2,%3,1"
                 : "=d" (u32_Data) : "d" (ou32_DataReg), "d" (ou32_Bit), "d" (ou32_Pos));

   return u32_Data;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    insert a bit field into u32_DataReg at u32_Pos

   \param[in]    ou32_DataReg    32bit data register that should be written
   \param[in]    ou32_Pos        bit position    [0..31]
   \param[in]    ou8_Width       bit field width [0..31]
   \param[in]    ou32_Bitfield   bitfield data

   \return   modified data register
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  2*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 insert_bitfield(const uint32 ou32_DataReg, const uint32 ou32_Pos, const uint8 ou8_Width,
                              const uint32 ou32_Bitfield)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Data = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("insert %0,%1,%2,%3,%4"
                 : "=d" (u32_Data) : "d" (ou32_DataReg), "d" (ou32_Bitfield), "d" (ou32_Pos), "i" (ou8_Width));

   return u32_Data;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    modify (atomic) a memory location addressed by pu32_RegPtr
             *pu32_RegPtr = (*pu32_RegPtr & ~u32_Mask) | (u32_Data & u32_Mask)

   \note Pay attention when using this instruction on SRI mapped Peripheral Registers (range 0xF800 0000-0xFFFF FFFF).
         Becaue the bit-manipulation functionality is supported only on registers accessed via the SPB bus, and is
         not supported on the SRI mapped peripheral range (i.e. address range 0xF800 0000 to 0xFFFF FFFF,
         including (if available) EBU, PMU0, SRI Crossbar, LMU, DAM, FFT, CPUx SFRs and CSFRs, MCDS, miniMCDS);
         see table 'On Chip Bus Address Map of Segment 15' in chapter 'Memory Map' of user manual for details).
         On the SRI mapped peripherals, usage of these instructions always results in all the bits of a register
         being written, and not just specific individual bits. (See errata SRI_TC.H001)

   \param[in,out]    opu32_RegPtr   pointer to 32bit memory/register that should be modified
   \param[in]        ou32_Mask      bit mask: only set bits are modified
   \param[in]        ou32_Data      data that should be copied to *pRegPtr
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void modify_mask(volatile uint32 * opu32_RegPtr, const uint32 ou32_Mask, const uint32 ou32_Data)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("   mov   %%d12, %2         # load data into D12                            \n\t" \
                 "   mov   %%d13, %1         # load mask into D13                            \n\t" \
                 "   ldmst [%0]0, %%e12      # modify memory location addressed by pRegPtr   \n\t" \
                 : : "a" (opu32_RegPtr), "d" (ou32_Mask), "d" (ou32_Data) : "d12", "d13", "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    modify (atomic) a single bit addressed by pu32_RegPtr at u32_Pos

   \note Pay attention when using this instruction on SRI mapped Peripheral Registers (range 0xF800 0000-0xFFFF FFFF).
         Becaue the bit-manipulation functionality is supported only on registers accessed via the SPB bus, and is
         not supported on the SRI mapped peripheral range (i.e. address range 0xF800 0000 to 0xFFFF FFFF,
         including (if available) EBU, PMU0, SRI Crossbar, LMU, DAM, FFT, CPUx SFRs and CSFRs, MCDS, miniMCDS);
         see table 'On Chip Bus Address Map of Segment 15' in chapter 'Memory Map' of user manual for details).
         On the SRI mapped peripherals, usage of these instructions always results in all the bits of a register
         being written, and not just specific individual bits. (See errata SRI_TC.H001)

   \param[in,out]    opu32_RegPtr  pointer to 32bit memory/register that should be modified
   \param[in]        ou32_Pos      bit position  [0..31]
   \param[in]        ou32_Bit      bit data      [0..1]
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void modify_bit(volatile uint32 * opu32_RegPtr, const uint32 ou32_Pos, const uint32 ou32_Bit)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("   imask %%e12, %2, %1, 1  # load bit mask and data into E12               \n\t" \
                 "   ldmst [%0]0, %%e12      # modify memory location addressed by pRegPtr   \n\t" \
                 : : "a" (opu32_RegPtr), "d" (ou32_Pos), "d" (ou32_Bit) : "d12", "d13", "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    modify (atomic) a bit field addressed by pu32_RegPtr at u32_Pos

   \note Pay attention when using this instruction on SRI mapped Peripheral Registers (range 0xF800 0000-0xFFFF FFFF).
         Becaue the bit-manipulation functionality is supported only on registers accessed via the SPB bus, and is
         not supported on the SRI mapped peripheral range (i.e. address range 0xF800 0000 to 0xFFFF FFFF,
         including (if available) EBU, PMU0, SRI Crossbar, LMU, DAM, FFT, CPUx SFRs and CSFRs, MCDS, miniMCDS);
         see table 'On Chip Bus Address Map of Segment 15' in chapter 'Memory Map' of user manual for details).
         On the SRI mapped peripherals, usage of these instructions always results in all the bits of a register
         being written, and not just specific individual bits. (See errata SRI_TC.H001)

   \param[in,out]    opu32_RegPtr  pointer to 32bit memory/register that should be modified
   \param[in]        ou32_Pos      bit position    [0..31]
   \param[in]        ou8_Width     bit field width [0..31]
   \param[in]        ou32_Data     bit field data
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  2*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void modify_bitfield(volatile uint32 * opu32_RegPtr, const uint32 ou32_Pos, const uint8 ou8_Width,
                            const uint32 ou32_Data)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("   imask %%e12, %2, %1, %3 # load bit mask and data into E12               \n\t" \
                 "   ldmst [%0]0, %%e12      # modify memory location addressed by pRegPtr   \n\t" \
                 : : "a" (opu32_RegPtr), "d" (ou32_Pos), "d" (ou32_Data), "i" (ou8_Width) : "d12", "d13", "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    swap (atomic) contents of memory location addressed by opu32_RegPtr
             with ou32_Data, return previous memory content

   \param[in,out]    opu32_RegPtr   pointer to 32bit memory/register
   \param[in]        ou32_Data      data that should be swapped

   \return   previous content of memory location addressed by opu32_RegPtr
*/
//----------------------------------------------------------------------------------------------------------------------
/*STW Deviation Coding Rules v4*/
/*Violated Rule: No MISRA C:2012 or STW Rules applicable to message 3227*/
/*Circumstances: Access to hardware*/
/*Reason: QA-C thinks that 'ou32_Data' won't get changed but it is inside the assembler function*/
/*Potential risks: none*/
/*Prevention of risks: nothing to do*/
/*PRQA S 3227 2*/
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 swap_uint32(volatile uint32 * opu32_RegPtr, uint32 ou32_Data)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("swap.w [%1]0,%0" : "=d" (ou32_Data) : "a" (opu32_RegPtr), "0" (ou32_Data) : "memory");

   return ou32_Data;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    swap (atomic) contents of memory location addressed by opu32_RegPtr
             with ou32_Data, under the given bitmask ou32_Mask.

   \note Pay attention when using this instruction on SRI mapped Peripheral Registers (range 0xF800 0000-0xFFFF FFFF).
         Becaue the bit-manipulation functionality is supported only on registers accessed via the SPB bus, and is
         not supported on the SRI mapped peripheral range (i.e. address range 0xF800 0000 to 0xFFFF FFFF,
         including (if available) EBU, PMU0, SRI Crossbar, LMU, DAM, FFT, CPUx SFRs and CSFRs, MCDS, miniMCDS);
         see table 'On Chip Bus Address Map of Segment 15' in chapter 'Memory Map' of user manual for details).
         On the SRI mapped peripherals, usage of these instructions always results in all the bits of a register
         being written, and not just specific individual bits. (See errata SRI_TC.H001)

   \param[in,out]    opu32_RegPtr   pointer to 32bit memory/register
   \param[in]        ou32_Data      data bits that should be swapped
   \param[in]        ou32_Mask      mask of the involved bits

   \return   previous content of memory location addressed by opu32_RegPtr
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 swapmsk_uint32(volatile uint32 * opu32_RegPtr, uint32 const ou32_Data, uint32 const ou32_Mask)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint64 u64_Result = ((uint64)ou32_Mask << 32u) | ou32_Data;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("swapmsk.w [%1]0,%A0" : "=d" (u64_Result) : "a" (opu32_RegPtr), "0" (u64_Result) : "memory");

   return (uint32)u64_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    Conditionally swap (atomic) of memory location addressed by opu32_RegPtr
             with ou32_Data, when memory content is equal to compare value ou32_Compare.

   \param[in,out]    opu32_RegPtr  pointer to 32bit memory/register
   \param[in]        ou32_Data     data that should be swapped
   \param[in]        ou32_Compare  compare value

   \return   (previous) content of memory location addressed by opu32_RegPtr
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 cmpswap_uint32(volatile uint32 * opu32_RegPtr, uint32 const ou32_Data, uint32 const ou32_Compare)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint64 u64_Result = ((uint64)ou32_Compare << 32u) | ou32_Data;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("cmpswap.w [%1]0,%A0" : "=d" (u64_Result) : "a" (opu32_RegPtr), "0" (u64_Result) : "memory");
   return (uint32)u64_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    calculate position of most significant bit = 1

   \note     if no bit is set in u32_Value the function returns 0xFFFFFFFF

   \param[in]    ou32_Value    32bit value

   \return     [0...31] for position of most significant bit which is set to 1, 0xFFFFFFFF if no bit is set
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 msb_pos(const uint32 ou32_Value)
{
   uint32 u32_BitPosMSB = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("clz %0,%1" : "=d" (u32_BitPosMSB) : "d" (ou32_Value));

   u32_BitPosMSB = (31u - u32_BitPosMSB);

   return u32_BitPosMSB;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    get minimum value from u32_Data1 and u32_Data2

   \param[in]    ou32_Data1    unsigned 32bit data value 1
   \param[in]    ou32_Data2    unsigned 32bit data value 2

   \return   u32_Min       min. data value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 min_u32(const uint32 ou32_Data1, const uint32 ou32_Data2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Min = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("min.u %0, %1, %2" : "=d" (u32_Min) : "d" (ou32_Data1), "d" (ou32_Data2));

   return u32_Min;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    get minimum value from s32_Data1 and s32_Data2

   \param[in]    os32_Data1    signed 32bit data value 1
   \param[in]    os32_Data2    signed 32bit data value 2

   \return   s32_Min       min. data value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE sint32 min_s32(const sint32 os32_Data1, const sint32 os32_Data2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   sint32 s32_Min = 0;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("min %0, %1, %2" : "=d" (s32_Min) : "d" (os32_Data1), "d" (os32_Data2));

   return s32_Min;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    get maximum value from u32_Data1 and u32_Data2

   \param[in]    ou32_Data1    unsigned 32bit data value 1
   \param[in]    ou32_Data2    unsigned 32bit data value 2

   \return   u32_Max       max. data value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 max_u32(const uint32 ou32_Data1, const uint32 ou32_Data2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Max = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("max.u %0, %1, %2" : "=d" (u32_Max) : "d" (ou32_Data1), "d" (ou32_Data2));

   return u32_Max;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    get maximum value from s32_Data1 and s32_Data2

   \param[in]    os32_Data1    signed 32bit data value 1
   \param[in]    os32_Data2    signed 32bit data value 2

   \return   s32_Max       max. data value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE sint32 max_s32(const sint32 os32_Data1, const sint32 os32_Data2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   sint32 s32_Max = 0;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("max %0, %1, %2" : "=d" (s32_Max) : "d" (os32_Data1), "d" (os32_Data2));

   return s32_Max;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    round the input value of32_Data to an unsigned integer
             using the current FPU rounding mode set in PSW.RM
             default rounding mode is "round to nearest"

   \param[in]    of32_Data     float32 input value

   \return   u32_Result    of32_Data rounded to unsigned integer
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 float_to_uint32(const float32 of32_Data)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Result = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("ftou %0,%1" : "=d" (u32_Result) : "d" (of32_Data));

   return u32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    round the input value of32_Data to a signed integer
             using the current FPU rounding mode set in PSW.RM
             default rounding mode is "round to nearest"

   \param[in]    of32_Data     float32 input value

   \return   s32_Result    of32_Data rounded to signed integer
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE sint32 float_to_sint32(const float32 of32_Data)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   sint32 s32_Result = 0;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("ftoi %0,%1" : "=d" (s32_Result) : "d" (of32_Data));

   return s32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    multiply float32 value of32_Mult1 by of32_Mult2 and add of32_Add

   \param[in]    of32_Mult1  multiplier
   \param[in]    of32_Mult2  multiplier
   \param[in]    of32_Add    offset

   \return   f32_Result = (of32_Mult1 * of32_Mult2) + of32_Add
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE float32 madd_f32(float32 of32_Mult1, float32 of32_Mult2, float32 of32_Add)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   float32 f32_Result = 0.0f;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("madd.f %0, %3, %1, %2" : "=d" (f32_Result)
                 : "d" (of32_Mult1), "d" (of32_Mult2), "d" (of32_Add));
   return f32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    multiply float32 value of32_Mult1 by of32_Mult2 and
             subtract product from of32_Minuend

   \param[in]    of32_Mult1     multiplier
   \param[in]    of32_Mult2     multiplier
   \param[in]    of32_Minuend   minuend

   \return   f32_Result = of32_Minuend - of32_Mult1 * of32_Mult2
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE float32 msub_f32(float32 of32_Mult1, float32 of32_Mult2, float32 of32_Minuend)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   float32 f32_Result = 0.0f;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("msub.f %0, %3, %1, %2" : "=d" (f32_Result)
                 : "d" (of32_Mult1), "d" (of32_Mult2), "d" (of32_Minuend));
   return f32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   unsigned subtraction with saturation:
            if ((ou32_Minuend - ou32_Subtrahend) < 0)
               u32_Result = 0;
            else
               u32_Result = ou32_Minuend - ou32_Subtrahend;

   \param[in]    ou32_Minuend     minuend
   \param[in]    ou32_Subtrahend  subtrahend

   \return   u32_Result       see description above
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 subs_u32(const uint32 ou32_Minuend, const uint32 ou32_Subtrahend)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Result = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("subs.u %0, %1, %2" : "=d" (u32_Result) : "d" (ou32_Minuend), "d" (ou32_Subtrahend));

   return u32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   signed subtraction with saturation:
            if ((os32_Minuend - os32_Subtrahend) < -2^31)
               s32_Result = -2^31;
            else if ((os32_Minuend - os32_Subtrahend) > 2^31-1)
               s32_Result = 2^31-1
            else
               s32_Result = os32_Minuend - os32_Subtrahend;

   \param[in]    os32_Minuend     minuend
   \param[in]    os32_Subtrahend  subtrahend

   \return   s32_Result       see description above
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE sint32 subs_s32(const sint32 os32_Minuend, const sint32 os32_Subtrahend)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   sint32 s32_Result = 0;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("subs %0, %1, %2" : "=d" (s32_Result) : "d" (os32_Minuend), "d" (os32_Subtrahend));

   return s32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   unsigned addition with saturation:
            if ((ou32_Summand1 + ou32_Summand2) > 0xFFFFFFFF)
               u32_Result = 0xFFFFFFFF;
            else
               u32_Result = ou32_Summand1 + ou32_Summand2

   \param[in]    ou32_Summand1    summand
   \param[in]    ou32_Summand2    summand

   \return   u32_Result       see description above
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 adds_u32(const uint32 ou32_Summand1, const uint32 ou32_Summand2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Result = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("adds.u %0, %1, %2" : "=d" (u32_Result) : "d" (ou32_Summand1), "d" (ou32_Summand2));

   return u32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   signed addition with saturation:
            if ((os32_Summand1 + os32_Summand2) < -2^31)
               s32_Result = -2^31;
            else if ((os32_Summand1 + os32_Summand2) > 2^31-1)
               s32_Result = 2^31-1
            else
               s32_Result = os32_Summand1 + os32_Summand2;

   \param[in]    os32_Summand1    summand
   \param[in]    os32_Summand2    summand

   \return   s32_Result       see description above
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE sint32 adds_s32(const sint32 os32_Summand1, const sint32 os32_Summand2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   sint32 s32_Result = 0;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("adds %0, %1, %2" : "=d" (s32_Result) : "d" (os32_Summand1), "d" (os32_Summand2));

   return s32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    convert a uint32 value into a void* address pointer

   \param[in]    ou32_UInt32   uint32 value

   \return   void* address pointer
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void * uint32_to_ptr(const uint32 ou32_UInt32)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   void * pvVoid = (void *)0u;

   /* move u32_UInt32 (data register value) into pvVoid (address register value) */
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("mov.a %0, %1" : "=a" (pvVoid) : "d" (ou32_UInt32));

   return pvVoid;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    convert a void* pointer into a uint32 value

   \param[in]    opv_Void      address pointer

   \return   u32_UInt32    uint32 value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 ptr_to_uint32(const volatile void * opv_Void)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_UInt32 = 0u;

   /* move pvVoid (address register value) into u32_UInt32 (data register value) */
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("mov.d %0, %1" : "=d" (u32_UInt32) : "a" (opv_Void));

   return u32_UInt32;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    calculate the (byte) difference between two void pointers

   \param[in]    opv_Void1     address pointer1
   \param[in]    opv_Void2     address pointer2

   \return   u32_Diff      u32_Diff = abs(opv_Void1 - opv_Void2)
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 ptr_addr_diff(const volatile void * opv_Void1, const volatile void * opv_Void2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Diff = 0u; // preset variable to prevent warning

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("sub.a %%a15, %1, %2 # %1 = (%1 - %2)      \n\t" \
                 "mov.d %0, %%a15     # %0 = (%1 - %2)      \n\t" \
                 "abs   %0, %0        # %0 = ABS(%1 -%2)    \n\t" \
                 : "=d" (u32_Diff) : "a" (opv_Void1), "a" (opv_Void2) : "a15");

   return u32_Diff;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    add signed (byte) offset to void pointer


   \param[in]    opv_Void      address pointer
   \param[in]    os32_Offset   signed address byte offset

   \return   pvVoidOffset  pvVoidOffset = pvVoid + iOffset;

*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void * ptr_addr_offset(const volatile void * opv_Void, sint32 os32_Offset)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   void * pvVoidOffset = (void *)0u; // preset variable to prevent compiler warning

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("addsc.a %0, %1, %2, 0      # %0 = (%1 + %2)      \n\t" \
                 : "=a" (pvVoidOffset) : "a" (opv_Void), "d" (os32_Offset));

   return pvVoidOffset;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    calculate the absolute difference between two uint32 data words

   \param[in]    ou32_Data1    data word 1
   \param[in]    ou32_Data2    data word 2

   \return   u32_Diff      u32_Diff = abs(ou32_Data1 - ou32_Data2)

*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 data_diff_u32(const uint32 ou32_Data1, const uint32 ou32_Data2)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Diff = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("sub   %0, %1, %2 # %1 = (%1 - %2)      \n\t" \
                 "abs   %0, %0     # %0 = ABS(%1 -%2)    \n\t" \
                 : "=d" (u32_Diff) : "d" (ou32_Data1), "d" (ou32_Data2));

   return u32_Diff;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Check, if address opu8_Data points to a cached address space

            This function assumes that the physical memory attributes (PMA) settings are
            left unmodified from their reset values.

            Return TRUE when opu8_Data is a cached address otherwise return FALSE

   \param[in]   opv_Data      pointer to address for checking

   \return  0 if address is not cached, 1 if address is cahced
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE uint32 address_is_cached(const volatile void * const opv_Data)
{
   const uint32 u32_Addr = ptr_to_uint32(opv_Data);

   return ((u32_Addr & 0xE0000000uL) == 0x80000000uL) ? 1uL : 0uL;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   translate the pointer address into an uncached address

            This function assumes that the physical memory attributes (PMA) settings are
            left unmodified from their reset values.

   \param[in]   opv_Data      pointer to address for translation

   \return  pointer to uncached address
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void * address_make_uncached(const volatile void * const opv_Data)
{
   const uint32 u32_Addr = ptr_to_uint32(opv_Data);

   return uint32_to_ptr(u32_Addr | UNCACHED_SEGMENT_ADDR);
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Write back and invalidate cache lines associated with address range

      This function can be used to force a cache sync with the associated memory.
      "Dirty" data in the cache line(s) will be written back to physical memory
      and marked as "invalid".

      Cached data must be synced before another bus master (eg. DMA, CPU2, etc.)
      accesses this memory area. Otherwise, inconsistency of data may occur.
      Program cache does not need to be synced as code will not be changed during
      runtime.

      This function returns a pointer to the opu8_Data memory start address aligned to
      the next cache line start address.

   \note    TriCore DCACHE and PCACHE are implemented as a "write-back" cache type.
            Cache is organized in 512 lines with each of 32 byte size.
            Write back and invalidation is always done for a complete line.

   \param[in]   opu8_Data      pointer to base address
   \param[in]  ou32_Count     size of memory area (in bytes)

   \return  pointer to the opu8_Data memory start address aligned to the next cache line

*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void * cache_writeback_invalidate(const volatile void * const opu8_Data, const uint32 ou32_Count)
{
   const uint32 u32_Addr = ptr_to_uint32(opu8_Data);
   const uint32 u32_AddrCacheAligned = u32_Addr & 0xFFFFFFE0uL;

   // check, if source address is a cached address and ou32_Count > 0
   if (((u32_Addr & 0xE0000000uL) == 0x80000000uL) && (ou32_Count != 0u))
   {
      // variable u32_CacheLineCount is used by the inline assembly code
      /*PRQA S 3205  1*/ /*STW_MD4_M_Rule-1.2_1006*/
      const uint32 u32_CacheLineCount = ((u32_Addr & 0x0000001FuL) + (ou32_Count - 1u)) >> 5u;

      // synchronize CPU data cache and memory
      /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
      asm volatile ("   mov.a     %%a13, %0           # load destination address                         \n\t"
                    "   mov.a     %%a15, %1           # load loop counter                                \n\t"
                    " 1:                                                                                 \n\t"
                    "   cachea.wi [%%a13+]32          # invalidate cache line associated with address    \n\t"
                    "   loop      %%a15, 1b           # iterate till memory has been invalidated + saved \n\t"
                    "   dsync                         # finish all memory write transactions             \n\t"
                    :  : "d" (u32_AddrCacheAligned), "d" (u32_CacheLineCount) : "a13", "a15", "memory");
   }

   return uint32_to_ptr(u32_AddrCacheAligned);
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief    calculate the absolute value (unsigned) of a given sint32 value

   \param[in]    os32_Data     signed data

   \return   u32_Result    abs(s32_Data)
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 abs_s32(const sint32 os32_Data)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Result = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("abs %0, %1" : "=d" (u32_Result) : "d" (os32_Data) : "memory");

   return u32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Lock a spinlock variable. Block until it could be locked.

   \note    If the memory addressed by opu32_Spinlock contains a zero, the Spinlock is
            available. For all other values, the spinlock is considered as "in use".

   \note    If data is written, then the writes are performed automatically via core-
            attached store buffers. Atomic cmpswap operation, however, do not use
            the store buffer.
            So we need a dsync to make sure that all store-data is written before the
            spinlock is obtained.

   \param[in,out] opu32_Spinlock    Spinlock variable
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void spin_lock(volatile uint32 * const opu32_Spinlock)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile (" mov     %%d12, 1          # try to obtain spinlock                                          \n\t"
                 " mov     %%d13, 0          # spinlock must be unused before                                  \n\t"
                 " dsync                     # flush store buffers                                             \n\t"
                 " 1:                        # try again                                                       \n\t"
                 " cmpswap.w  [%0]0,%%e12    # *pu32_Spinlock = (*pu32_Spinlock == D13) ? D12 : *pu32_Spinlock \n\t"
                 " jnz     %%d12,1b          # D12 = *pu32_Spinlock (zero on success)                          \n\t"
                 : : "a" (opu32_Spinlock) : "d12", "d13", "memory");
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Try to lock spinlock variable. Only try once to lock it, the return.

   Details see spin_lock().

   \param[in,out] opu32_Spinlock    Spinlock variable

   \retval  0      Spinlock has been lock
   \retval  1      Spinlock already locked by other core
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 spin_trylock(volatile uint32 * const opu32_Spinlock)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Result = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile (
      " mov     %%d12, 1          # try to obtain spinlock                                          \n\t"
      " mov     %%d13, 0          # spinlock must be unused before                                  \n\t"
      " dsync                     # flush store buffers                                             \n\t"
      " cmpswap.w  [%1]0,%%e12    # *pu32_Spinlock = (*pu32_Spinlock == D13) ? D12 : *pu32_Spinlock \n\t"
      " mov     %0, %%d12         # return previous sate of  *pu32_Spinlock (zero on success)       \n\t"
      : "=d" (u32_Result) : "a" (opu32_Spinlock) : "d12", "d13", "memory");

   return u32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Unlock a spinlock

   \param[in,out] opu32_Spinlock    Spinlock variable
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE void spin_unlock(volatile uint32 * const opu32_Spinlock)
{
   *opu32_Spinlock = 0u;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Obtain core number of the executing core (0...x)

   \return  core id
*/
//----------------------------------------------------------------------------------------------------------------------
INLINE uint32 get_core_id(void)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_CoreRegVal = 0u;

   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("mfcr %0, 0xFE1C" : "=d" (u32_CoreRegVal) : : "memory"); // return content of CORE_ID register

   return u32_CoreRegVal;
}

//----------------------------------------------------------------------------------------------------------------------
/*!
   \brief   Calculate 32-Bit CRC

      The CRC polynomial used is the CRC-32 polynomial as defined in the
      IEEE 802.3 standard. The initial value should be zero.

   \param[in]  ou32_CrcVal    start value for CRC calculation (recent CRC)
   \param[in]  ou32_CrcData   new data for CRC calculation

   \return calculated new CRC value
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE uint32 crc32(const uint32 ou32_CrcVal, const uint32 ou32_CrcData)
{
   /*PRQA S 3204  1*/ /*STW_MD4_M_Rule-1.2_1006*/
   uint32 u32_Crc = 0u;

#if ((__GNUC__ > 4) || \
   ((__GNUC__ == 4) && (__GNUC_MINOR__ > 6)) || \
   ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6) && (__GNUC_PATCHLEVEL__ > 3)))
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("crc32 %0, %1, %2" : "=d" (u32_Crc) : "d" (ou32_CrcVal), "d" (ou32_CrcData) : "memory");
#else
   // Tricore GCC Implementations < 4.6.4.0 implement deprecated oder of crc parameters
   asm volatile ("crc32 %0, %2, %1" : "=d" (u32_Crc) : "d" (ou32_CrcVal), "d" (ou32_CrcData) : "memory");
#endif

   return u32_Crc;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Fast 64bit memory copy

   The caller must ensure the source and destination address are 64bit aligned.
   The caller must ensure ou32_Count is the expected number of 64bit words to be copied minus 1.

   \param[in]     opu8_Source    source address pointer
   \param[in]     opu8_Dest      destination address pointer
   \param[in]     ou32_Count     number of 64bit words plus 1 to be copied (must be reduced by 1 to have the real
                                                                            count)
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 3206  1*/ /*STW_MD4_M_Rule-1.2_1006*/
INLINE void mem_copy64(const uint8 * opu8_Source, uint8 * opu8_Dest, const uint32 ou32_Count)
{
   /*PRQA S 1006 1*//*STW_MD4_M_Rule-1.2_1006*/
   asm volatile ("1:                                       # loop label         \n\t"
                 "ld.d    %%e14,[%0+]8                     # load 64 bit value  \n\t"
                 "st.d    [%1+]8,%%e14                     # store 64 bit value \n\t"
                 "loop    %2,1b                            # next 64 bit value  \n\t"
                 : : "a" (opu8_Source), "a" (opu8_Dest), "a" (ou32_Count) : "d14", "d15", "memory");
}

#endif /* X_INTRINSICS_H */

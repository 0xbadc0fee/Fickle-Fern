//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       System Type Definition
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SYS_TYPES_H
#define X_SYS_TYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include <stdbool.h>

/* -- Defines ------------------------------------------------------------------------------------------------------- */

#define X_SYS_TRAPLOG_STACKSIZE 16u

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief  Error Informations

   This Structure contains Informations about an Initialization Error, like what and where the error happened.
*/
typedef struct
{
   sint16 s16_Error; //!< error that the instruction returned
   uint16 u16_Line;  //!< instruction line, where the error happened
} T_x_sys_init_err;

/*! \brief  Wake-up Status Informations

   This Structure contains Wake-up Information of the System.
*/
typedef struct
{
   uint32 u32_WakeupEvent;    //!< wake-up event that activated the ECU
   uint32 u32_WakeupFeedback; //!< feedback of wake-up signals which are currently active
   uint32 u32_WakeupEnable;   //!< enabled wake-up sources
} T_x_sys_wakeup_status;

/*! \brief  Application Information

   This structure contains the Application Informations and will get filled by OpenSYDE.
*/
typedef struct
{
   charn acn_Magic[8];               //!< e.g. X_SYS_INFO_BIOS_MAGIC
   uint8 u8_StructVersion;           //!< = 3 (0..2 are reserved)
   charn acn_Devicename[29];         /*!< name of device the application is intended for;
                                     e.g. "Y_ESX2" (28 characters and zero termination)*/
   charn acn_Date[12];               /*!< date of compilation (format: __DATE__ ANSI C macro) (11
                                     characters and zero termination)*/
   charn acn_Time[9];                /*!< time of compilation (format: __TIME__ ANSI C macro) (8
                                     characters and zero termination)*/
   charn acn_ApplicationName[33];    /*!< name of application or data block (32 characters and zero
                                     termination)*/
   charn acn_ApplicationVersion[17]; /*!< version number of application or data block; (16characters
                                     and zero termination)*/
   uint8 u8_LenAdditionalInfo;       /*!< Size in bytes of additional info array following after this
                                     object; value is OSY_FL_LEN_ADDITIONAL_INFO*/
   /*PRQA S 0642 1*/ /*STW_MD4_M_Rule-1.1_0642*/
   charn acn_AdditionalInfo[]; //!< Placeholder for additional data behind this object
} T_x_sys_application_information;

/*! \brief  Constant System Voltages

   This structure contains the Informations about the constant System Voltages.
*/
typedef struct
{
   sint32 s32_TypicalValue; //!< Typical voltage value
   sint32 s32_LowerLimit;   //!< Lower limit which is beeing checked by ADC monitor
   sint32 s32_UpperLimit;   //!< Upper limit which is beeing checked by ADC monitor
   bool q_SafetyRelevant;   //!< This Voltage is safety relevant if != 0
} T_x_sys_monitored_voltage_constants;

/*! \brief  Variable System Voltages

   This structure contains the Informations about the measured System Voltages.
*/
typedef struct
{
   sint32 s32_RawValue;      //!< Measured raw voltage value
   sint32 s32_FilteredValue; //!< Measured filtered voltage value
   sint32 s32_MinValue;      //!< Minimum measured voltage
   sint32 s32_MaxValue;      //!< Maximum measured voltage
} T_x_sys_monitored_voltage_measurements;

/*! \brief  General variant specific HW informations

   This Structure lists HW informations and versioning of variant structure
*/
typedef struct
{
   uint16 u16_StructVersionMajor; //!< Struct version major
   uint16 u16_StructVersionMinor; //!< Struct version minor
   uint16 u16_HardwareRevision;   //!< Hardware revision information, see defines X_SYS_HARDWARE_INFO_REVISION_..
   uint16 u16_HardwareType;       //!< Hardware type information, see defines X_SYS_HARDWARE_INFO_TYPE_..
} T_x_sys_hw_information;

/*! \brief  Trap information */
typedef struct
{
   uint8 u8_Class;                //!< Trap class [0 .. 7]
   uint8 u8_TIN;                  //!< TIN, trap class specific range, refer to the architecture manual
   uint8 u8_Synchronous;          //!< TRUE: Synchronous trap, FALSE: Asynchronous trap
   uint8 u8_StackEntries;         //!< Number of valid entries in au32_PCStack
   uint32 u32_DataAddr;           //!< Data access address, not valid for all traps
   uint32 u32_TrapStatus;         //!< Trap status flags, source register depends on u8_Synchronous
   const charn * pcn_Description; //!< Null-terminated string description of the trap
   uint32 u32_TimestampMs;        //!< Milliseconds since system startup
   uint16 u16_TaskPriority;       //!< Priority of the currently running task

   uint16 u16_Reserved;

   //! Program counter addresses (u8_Stackdepth contains number of leading valid entries, first element top of stack)
   uint32 au32_PCStack[X_SYS_TRAPLOG_STACKSIZE];
} T_x_sys_local_trap_info;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif

//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      Target specific System definitions
   \copyright  Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_SYS_DEFS_H
#define X_SYS_DEFS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_sys_types.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! Available Cores
#define  X_SYS_ID_CORE0                (0u)           //!< ID for Core0
#define  X_SYS_ID_CORE1                (1u)           //!< ID for Core1
#define  X_SYS_ID_CORE2                (2u)           //!< ID for Core2
#define  X_SYS_NUMBER_OF_CORES         (3u) //!< Number of available Cores

#define  X_SYS_ID_CORE_CAN_TRANS       (X_SYS_ID_CORE2) //!< ID of the Core which is responsible for CAN
#define  X_SYS_ID_CORE_RST_SERVER      (X_SYS_ID_CORE1) //!< Core ID of the Reset Server
#define  X_SYS_ID_CORE_WAKEUP          (X_SYS_ID_CORE2) //!< ID of the Core which is responsible for wakeup functions
#define  X_SYS_ID_CORE_ETH             (X_SYS_ID_CORE2) //!< ID of the Core which is responsible for ETH
#define  X_SYS_ID_CORE_SYS_VOLTAGE     (X_SYS_ID_CORE1) //!< ID of the Core which reads out the system voltages

#define  X_SYS_ICC_TIMEOUT             (100u) //!< ICC Timeout to wait for signal in OS timer ticks

#define  X_SYS_TEMP1_BOARD              (0u) //!< board temperature circuit board top
#define  X_SYS_TEMP2_BOARD              (1u) //!< board temperature circuit board bottom
#define  X_SYS_TEMP_CPU                 (2u) //!< CPU temperature
#define  X_SYS_TEMP_CH_COUNT            (3u) //!< max. number of temperature channels

//! IDs for System Voltages
#define  X_SYS_22V_VFB                  (1uL << 0u) //!< ID of power supply 22V voltage feedback
#define  X_SYS_13V5_VFB                 (1uL << 1u) //!< ID of power supply 13.5V voltage feedback
#define  X_SYS_8V5_VFB                  (1uL << 2u) //!< ID of power supply 8.5V voltage feedback
#define  X_SYS_6V_VFB                   (1uL << 3u) //!< ID of power supply 6V voltage feedback
#define  X_SYS_5VANA_VFB                (1uL << 4u) //!< ID of power supply 5Vana voltage feedback
#define  X_SYS_IOFFS_VFB                (1uL << 5u) //!< ID of current offset voltage feedback
#define  X_SYS_3V3_VFB                  (1uL << 6u) //!< ID of power supply 3.3V voltage feedback
#define  X_SYS_LIMIT_HS_VFB             (1uL << 7u) //!< ID of highside limit voltage feedback
#define  X_SYS_OC_LIMIT_LS              (1uL << 8u) //!< ID of lowside overcurrent limit voltage feedback
#define  X_SYS_LIMIT_LS_VFB             (1uL << 9u) //!< ID of lowside limit voltage feedback
#define  X_SYS_5V_VFB                  (1uL << 10u) //!< ID of power supply 5V voltage feedback
#define  X_SYS_5V3_VFB                 (1uL << 11u) //!< ID of power supply 5.3V voltage feedback
#define  X_SYS_5V_WD_VFB               (1uL << 12u) //!< ID of 5V supply of watchdog voltage feedback
#define  X_SYS_NEG_2V_VFB              (1uL << 13u) //!< ID of power supply -2V voltage feedback
#define  X_SYS_3V5_VFB                 (1uL << 14u) //!< ID of power supply 3.5V voltage feedback
#define  X_SYS_1V2_VFB                 (1uL << 15u) //!< ID of power supply 1.2V voltage feedback
#define  X_SYS_OC_LIMIT_HS             (1uL << 16u) //!< ID of highside overcurrent limit voltage feedback
#define  X_SYS_UF_VFB                  (1uL << 17u) //!< ID of power supply +UF voltage feedback
#define  X_SYS_VOLTAGE_COUNT           (18u)        //!< max. number of system voltages on this core

//! IDs for System Information
#define  X_SYS_INFO_ID_MAINBOARD          (0u)  //!< System Information ID Mainboard
#define  X_SYS_INFO_ID_CPU                (1u)  //!< System Information ID CPU
#define  X_SYS_INFO_ID_FPGA               (2u)  //!< System Information ID FPGA
#define  X_SYS_INFO_ID_RAM_SIZE_CORE0     (3u)  //!< System Information ID RAM size Core0
#define  X_SYS_INFO_ID_RAM_SIZE_CORE1     (4u)  //!< System Information ID RAM size Core1
#define  X_SYS_INFO_ID_RAM_SIZE_CORE2     (5u)  //!< System Information ID RAM size Core2
#define  X_SYS_INFO_ID_ROM_SIZE_CORE0     (6u)  //!< System Information ID ROM size Core0
#define  X_SYS_INFO_ID_ROM_SIZE_CORE1     (7u)  //!< System Information ID ROM size Core1
#define  X_SYS_INFO_ID_ROM_SIZE_CORE2     (8u)  //!< System Information ID ROM size Core2
#define  X_SYS_INFO_ID_CLOCK_CORE0        (9u)  //!< System Information ID Clock Core0
#define  X_SYS_INFO_ID_CLOCK_CORE1        (10u) //!< System Information ID Clock Core1
#define  X_SYS_INFO_ID_CLOCK_CORE2        (11u) //!< System Information ID Clock Core2
#define  X_SYS_INFO_ID_HEAP_DEFAULT_SIZE  (12u) //!< System Information ID HEAP size of Default EMEM SRAM
#define  X_SYS_INFO_ID_HEAP_DEFAULT_USED  (13u) //!< System Information ID HEAP used of Default EMEM SRAM
#define  X_SYS_INFO_ID_HEAP_DEFAULT_FREE  (14u) //!< System Information ID HEAP free of Default EMEM SRAM
#define  X_SYS_INFO_ID_HEAP_DSPR_SIZE     (15u) //!< System Information ID HEAP size of DSPR
#define  X_SYS_INFO_ID_HEAP_DSPR_USED     (16u) //!< System Information ID HEAP used of DSPR
#define  X_SYS_INFO_ID_HEAP_DSPR_FREE     (17u) //!< System Information ID HEAP free of DSPR
#define  X_SYS_INFO_ID_HEAP_EMEM_SIZE     (18u) //!< System Information ID HEAP size of EMEM
#define  X_SYS_INFO_ID_HEAP_EMEM_USED     (19u) //!< System Information ID HEAP used of EMEM
#define  X_SYS_INFO_ID_HEAP_EMEM_FREE     (20u) //!< System Information ID HEAP free of EMEM
#define  X_SYS_INFO_ID_COUNT              (21u) //!< Maximum System Information ID

#define  X_SYS_WAKEUP_KL15             (0x00000001u) //!< ECU wake-up flag for wakeup via KL15
#define  X_SYS_WAKEUP_CAN1             (0x00000002u) //!< ECU wake-up flag for wakeup via CAN1
#define  X_SYS_WAKEUP_BRR0             (0x00000004u) //!< ECU wake-up flag for wakeup via BRR0
#define  X_SYS_WAKEUP_BRR1             (0x00000008u) //!< ECU wake-up flag for wakeup via BRR1
#define  X_SYS_WAKEUP_CH_COUNT         (4u)          //!< Maximum wake-up Channels

//! Application Informations
#define  OSY_FL_LEN_ADDITIONAL_INFO    (0xFFu)     //!< Additional Info Max Length
#define  X_SYS_STRUCT_VERSION          (0x03U)     //!< tag to identify structure version
#define  X_SYS_DEVICE_NAME             "ESX-4CM-A" //!< tag to identify device name

//! magic codes to distinguish the type of information stored in T_x_sys_application_information:
//! tag to identify STW BIOS libraries, shall not be used by application
#define  X_SYS_INFO_BIOS_MAGIC         "Lx_?zg1"
//! tag to identify driver and application libraries (e.g. CAN-Protocols)
#define  X_SYS_INFO_DRIVER_MAGIC       "Lx_?zg2"
//! tag to identify application information
#define  X_SYS_INFO_APPL_MAGIC         "Lx_?zg."

//! Reset Reasons
#define  X_SYS_RST_REASON_ESR0                 (0x00000001u) //!< ESR0 is responsible for the last reset
#define  X_SYS_RST_REASON_ESR1                 (0x00000002u) //!< ESR1 is responsible for the last reset
#define  X_SYS_RST_REASON_SMU                  (0x00000008u) //!< SMU is responsible for the last reset
#define  X_SYS_RST_REASON_SW                   (0x00000010u) //!< SW is responsible for the last reset
#define  X_SYS_RST_REASON_PORST                (0x00010000u) //!< PORST is responsible for the last reset
#define  X_SYS_RST_REASON_EVR13                (0x00800000u) //!< EVR13 is responsible for the last reset
#define  X_SYS_RST_REASON_EVR33                (0x01000000u) //!< EVR33 is responsible for the last reset
#define  X_SYS_RST_REASON_SWD                  (0x02000000u) //!< SWD is responsible for the last reset
#define  X_SYS_RST_REASON_STBYR                (0x10000000u) //!< STBYR is responsible for the last reset

//! User Software Reset Reasons
#define  X_SYS_RST_SW_REASON_APPL0             (0x00000001u) /*!< The Application on Boot Core triggered the last SW
                                                                  reset*/
#define  X_SYS_RST_SW_REASON_APPL1             (0x00000002u) /*!< The Application on Safety Core triggered the last SW
                                                                  reset*/
#define  X_SYS_RST_SW_REASON_APPL2             (0x00000004u) /*!< The Application on Application Core triggered the last
                                                                  SW reset*/
//! Place new User SW reset reasons here
#define  X_SYS_RST_SW_REASON_COUNT             (0x00010000u) //!< Maximum number of user SW reset reasons

//! System Software Reset Reasons
#define  X_SYS_RST_SW_REASON_OSY               (0x04000000u) //!< The SW reset was triggered by openSYDE
#define  X_SYS_RST_SW_REASON_WD_SAFETY         (0x08000000u) //!< The SW reset was triggered by the safety Watchdog
#define  X_SYS_RST_SW_REASON_WD_CPU0           (0x10000000u) //!< The SW reset was triggered by the CPU0 Watchdog
#define  X_SYS_RST_SW_REASON_WD_CPU1           (0x20000000u) //!< The SW reset was triggered by the CPU1 Watchdog
#define  X_SYS_RST_SW_REASON_WD_CPU2           (0x40000000u) //!< The SW reset was triggered by the CPU2 Watchdog
#define  X_SYS_RST_SW_REASON_DIAG              (0x80000000u) //!< The SW reset was triggered by the diagnosis

//! Define the starting bit positions of the ADC faults within the system faults
#define X_SYS_ADC_FAULT_POS 30u

//! System & ADC faults
#define  X_SYS_FAULT_UPTIME_WARNING          (1uL << 0uL)  //!< system is going to exceed the uptime limit
#define  X_SYS_FAULT_UPTIME_OVERFLOW         (1uL << 1uL)  //!< system uptime overflow
#define  X_SYS_FAULT_UF_OUT_OF_RANGE         (1uL << 2uL) //!< UF was below it's limit for to long
#define  X_SYS_FAULT_TEMP_CPU_UPPER_LIMIT    (1uL << 3uL)  //!< system cpu temperature exceeded upper limit
#define  X_SYS_FAULT_TEMP_CPU_LOWER_LIMIT    (1uL << 4uL)  //!< system cpu temperature fall below lower limit
#define  X_SYS_FAULT_TEMP1_UPPER_LIMIT       (1uL << 5uL)  //!< system top board temperature exceeded upper limit
#define  X_SYS_FAULT_TEMP1_LOWER_LIMIT       (1uL << 6uL)  //!< system top board temperature fall below lower limit
#define  X_SYS_FAULT_TEMP2_UPPER_LIMIT       (1uL << 7uL)  //!< system bottom board temperature exceeded upper limit
#define  X_SYS_FAULT_TEMP2_LOWER_LIMIT       (1uL << 8uL)  //!< system bottom board temperature fall below lower limit


#define X_SYS_FAULT_ADC_ALIVE                (1uL << X_SYS_ADC_FAULT_POS)       /*!< Fault ADC did not start a conversion
                                                                                    within expected time */
#define  X_SYS_FAULT_ADC_DIVIDER             (1uL << 31uL) /*!< VADC clock divider changed (DIVA) while system is
                                                                running */
#define  X_SYS_FAULT_ALL                     (0xFFFFFFFFu) //!< all fault flags set

// Available 4CM-A hardware variants
// SE2 - Hardware
#define  X_SYS_TRG_SE2_HW_VARIANT_1  (0x21u) //!< LIN,   2x 100Base Eth, 4x CAN, FRAM
#define  X_SYS_TRG_SE2_HW_VARIANT_2  (0x26u) //!< LIN,   2x 100Base Eth, 4x CAN, DFLASH
#define  X_SYS_TRG_SE2_HW_VARIANT_3  (0x27u) //!< LIN,                   4x CAN, DFLASH
#define  X_SYS_TRG_SE2_HW_VARIANT_4  (0x28u) //!< RS232, 2x 100Base Eth, 4x CAN, FRAM
#define  X_SYS_TRG_SE2_HW_VARIANT_5  (0x23u) //!<                        1x CAN,         KL15&CAN1 Wakeup (STIO)
#define  X_SYS_TRG_SE2_HW_VARIANT_6  (0x24u) //!< LIN,                   4x CAN,         KL15&CAN1 Wakeup (STCU)
#define  X_SYS_TRG_SE2_HW_VARIANT_7  (0x25u) //!< LIN,   2x 100Base Eth, 4x CAN, DFLASH                   (BSH)
#define  X_SYS_TRG_SE2_HW_VARIANT_8  (0x22u) //!<                        1x CAN,                          (Y_4IOM-A V1)
#define  X_SYS_TRG_SE2_HW_VARIANT_9  (0x29u) //!< LIN,   2x 100Base Eth, 1x CAN,                          (Y_4IOM-A V2)
#define  X_SYS_TRG_SE2_HW_VARIANT_10 (0x2Bu) //!< LIN,   2x 100Base Eth, 4x CAN, FRAM    KL15&CAN1 Wakeup

#define X_SYS_HARDWARE_INFO_TYPE_INVALID     (0u)  //!< 0 = Detected hardware type is not valid
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT1    (1u)  //!< variant 1 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT2    (2u)  //!< variant 2 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT3    (3u)  //!< variant 3 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT4    (4u)  //!< variant 4 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT5    (5u)  //!< variant 5 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT6    (6u)  //!< variant 6 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT7    (7u)  //!< variant 7 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT8    (8u)  //!< variant 8 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT9    (9u)  //!< variant 9 - see variant description of each se version below
#define X_SYS_HARDWARE_INFO_TYPE_VARIANT10   (10u) //!< variant 10- see variant description of each se version below

// defines for system hardware config struct
#define X_VAR_IN_COUNT       (34u)
#define X_VAR_MSW_COUNT      (3u)
#define X_VAR_OUT_COUNT      (29u)
#define X_VAR_UEXT_COUNT     (4u)
#define X_VAR_ETH_COUNT      (1u)
#define X_VAR_ETH_PORT_COUNT (4u)
#define X_VAR_CAN_COUNT      (4u)
#define X_VAR_SER_COUNT      (1u)
#define X_VAR_LIN_COUNT      (1u)

#define X_VAR_INPUT_EQUIPPED   (1u)
#define X_VAR_MSW_EQUIPPED     (1u)
#define X_VAR_OUTPUT_EQUIPPED  (1u)
#define X_VAR_UEXT_EQUIPPED    (1u)
#define X_VAR_SER_EQUIPPED     (1u)
#define X_VAR_LIN_EQUIPPED     (1u)
#define X_VAR_IMX6_EQUIPPED    (1u)

// Ethernet defines
#define X_VAR_ETH_CHANNEL_EQUIPPED   (1u)
#define X_VAR_ETH_PORT_EQUIPPED      (1u)

// CAN defines
#define X_VAR_CAN_NODE_EQUIPPED      (1u)
#define X_VAR_CAN_NODE_WAKEUP        (2u)

/* -- Types --------------------------------------------------------------------------------------------------------- */

typedef struct
{
  T_x_sys_hw_information t_HwInfo;                //!< HW informations and versioning of variant structure
   uint32 u32_HwVariant;                           //!< Hardware variant or Partnumber
   uint8 au8_Inputs[X_VAR_IN_COUNT];               //!< placed inputs - check with X_VAR_INPUT_EQUIPPED
   uint8 au8_Msws[X_VAR_MSW_COUNT];                //!< placed msws - check with X_VAR_MSW_EQUIPPED
   uint8 au8_Outputs[X_VAR_OUT_COUNT];             //!< placed outputs - check with X_VAR_OUTPUT_EQUIPPED
   uint8 au8_Uexts[X_VAR_UEXT_COUNT];              //!< placed sensor supply voltages - check with X_VAR_UEXT_EQUIPPED
   uint8 au8_EthChannels[X_VAR_ETH_COUNT];         //!< placed ethernet channels - check with X_VAR_ETH_CHANNEL_EQUIPPED
   uint32 au32_EthPorts[X_VAR_ETH_PORT_COUNT];     //!< placed ethernet ports - check with X_VAR_ETH_PORT_EQUIPPED
   uint8 au8_Cans[X_VAR_CAN_COUNT];                //!< placed can nodes - check with X_VAR_CAN_NODE_EQUIPPED
   uint8 au8_Rs232s[X_VAR_SER_COUNT];              //!< placed serial interfaces - check with X_VAR_SER_EQUIPPED
   uint8 au8_Lins[X_VAR_LIN_COUNT];                //!< placed lin interfaces - check with X_VAR_LIN_EQUIPPED
   uint8 u8_FramCount;                             //!< number of placed fram devices
   uint8 u8_DflashCount;                           //!< number of placed dflash devices
   uint8 u8_EepromCount;                           //!< number of placed eeprom devices
   uint8 u8_Imx6;                                  //!< 1u: imx6 placed
   uint32 u32_UserMemoryPartizionSize;             //!< number of bytes in user nvm partition
} T_x_sys_hw_cfg;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif // X_SYS_DEFS_H

//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Aurix platform input api definition

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_IN_H___
#define X_IN_H___

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"
#include "x_stdtypes.h"
#include "x_in_defs.h"

#include "x_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/*! \brief Circuit flags */
#define X_IN_CIRCUIT_PULL_DOWN  (1uL <<  0u) //!< Digital inputs: Activate pull down resistor
#define X_IN_CIRCUIT_PULL_UP    (1uL <<  1u) //!< Digital inputs: Activate pull up resistor
#define X_IN_CIRCUIT_NAMUR      (1uL <<  2u) //!< Digital inputs: Select Namur mode (and activate & lock pull-up)

#define X_IN_VOLTAGE_RANGE_5V   (1uL << 16u) //!< Select 5V voltage range (not supported on all channels!)
#define X_IN_VOLTAGE_RANGE_12V  (2uL << 16u) //!< Select 12V voltage range (not supported on all channels!)
#define X_IN_VOLTAGE_RANGE_35V  (3uL << 16u) //!< Select 35V voltage range (not supported on all channels!)

#define X_IN_CIRCUIT_IDACV_5V   (X_IN_VOLTAGE_RANGE_5V)  //!< Digital inputs: IDACV  5V range (deprecated)
#define X_IN_CIRCUIT_IDACV_12V  (X_IN_VOLTAGE_RANGE_12V) //!< Digital inputs: IDACV 12V range (deprecated)
#define X_IN_CIRCUIT_IDACV_35V  (X_IN_VOLTAGE_RANGE_35V) //!< Digital inputs: IDACV 35V range (deprecated)

/*! \brief Digital logic selectors */
#define X_IN_LOGIC_POSITIVE 0u //!< Digital inputs: Use positive logic, High level <-> True
#define X_IN_LOGIC_NEGATIVE 1u //!< Digital inputs: Use negative logic, Low Level  <-> True

/*! \brief Diagnostic switch input type selectors */
#define X_IN_DIAGSWITCH_LOWSIDE 0u  //!< Diagnostic switch: Placed between input and ground
#define X_IN_DIAGSWITCH_HIGHSIDE 1u //!< Diagnostic switch: Places between input and UB

/*! \brief Frequency + Switch input type selectors */
#define X_IN_FREQSWITCH_LOWSIDE   0u //!< Frequency switch: low side frequency
#define X_IN_FREQSWITCH_HIGHSIDE  1u //!< Frequency switch: high side frequency

/*! \brief Incremental input direction selectors */
#define X_IN_INCREMENTAL_MODE_X4_POSITIVE 0u //!< Incremental direction: Increment on leading A channel edges
#define X_IN_INCREMENTAL_MODE_X4_NEGATIVE 1u //!< Incremental direction: Decrement on leading A channel edges

#define X_IN_INCREMENTAL_DIR_POSITIVE 0u //!< Incremental direction: Measured positive rotation wrt. initialization
#define X_IN_INCREMENTAL_DIR_NEGATIVE 1u //!< Incremental direction: Measured negative rotation wrt. initialization

/*! \brief Event input mode selectors */
#define X_IN_EVENT_MODE_RISING_EDGE   1u //!< Trigger event on rising edges
#define X_IN_EVENT_MODE_FALLING_EDGE  2u //!< Trigger event on falling edges
#define X_IN_EVENT_MODE_BOTH_EDGES    3u //!< Trigger event on both edges

/*! \brief Diagnostic test case fault flags */
#define X_IN_FAULT_SHORT_TO_GND           0x0001uL //!< Short to ground fault
#define X_IN_FAULT_COMMON_FAULT           0x0002uL //!< Electrical fault that cannot be further specified
#define X_IN_FAULT_OPEN_LOAD              0x0004uL //!< Open load fault
#define X_IN_FAULT_SHORT_TO_UB            0x0008uL /*!< Short To UB fault (Might include open load faults for some
                                                        roles */
#define X_IN_FAULT_VOLTAGE_REDUNDANCY     0x0010uL //!< Voltage redundancy tolerance fault
#define X_IN_FAULT_CURRENT_REDUNDANCY     0x0020uL //!< Current redundancy tolerance fault
#define X_IN_FAULT_DIGITAL_REDUNDANCY     0x0040uL //!< Digital value redundancy tolerance fault
#define X_IN_FAULT_DIGITAL_PLAUSIBILITY   0x0080uL //!< Voltage feedback and digital feedback not consistent
#define X_IN_FAULT_FREQUENCY_LOWER_LIMIT  0x0100uL //!< Frequency lower limit violation fault
#define X_IN_FAULT_FREQUENCY_UPPER_LIMIT  0x0200uL //!< Frequency upper limit violation fault
#define X_IN_FAULT_FREQUENCY_REDUNDANCY   0x0400uL //!< Frequency redundancy tolerance fault
#define X_IN_FAULT_DUTY_CYCLE_LOWER_LIMIT 0x0800uL //!< Duty cycle lower limit violation fault
#define X_IN_FAULT_DUTY_CYCLE_UPPER_LIMIT 0x1000uL //!< Duty cycle upper limit violation fault
#define X_IN_FAULT_DUTY_CYCLE_REDUNDANCY  0x2000uL //!< Duty cycle redundancy tolerance fault
#define X_IN_FAULT_INCREMENTAL_REDUNDANCY 0x4000uL //!< Incremental counter redundancy tolerance fault
#define X_IN_FAULT_CCF                    0x8000uL //!< CCF fault

#define X_IN_FAULT_ALL 0xFFFFFFFFuL

#define X_IN_TYPE_NONE                   0u
#define X_IN_TYPE_VOLTAGE                1u
#define X_IN_TYPE_VOLTAGE_REDUNDANT      2u
#define X_IN_TYPE_CURRENT                3u
#define X_IN_TYPE_CURRENT_REDUNDANT      4u
#define X_IN_TYPE_DIGITAL                5u
#define X_IN_TYPE_DIGITAL_REDUNDANT      6u
#define X_IN_TYPE_FREQUENCY              7u
#define X_IN_TYPE_FREQUENCY_REDUNDANT    8u
#define X_IN_TYPE_INCREMENTAL            9u
#define X_IN_TYPE_INCREMENTAL_REDUNDANT 10u
#define X_IN_TYPE_DIAGSWITCH            11u
#define X_IN_TYPE_NAMUR                 12u
#define X_IN_TYPE_FREQSWITCH            13u
#define X_IN_TYPE_EVENT                 14u
#define X_IN_TYPE_ONESHOT               15u
#define X_IN_TYPE_UNAVAILABLE           16u

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_in_get_type(const uint16 ou16_Channel, uint16 * const opu16_Type);

sint16 x_in_voltage_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter);
sint16 x_in_voltage_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_Min,
                         const sint32 os32_Max);

sint16 x_in_voltage_redundant_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter);
sint16 x_in_voltage_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_Tolerance);

sint16 x_in_current_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter);
sint16 x_in_current_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_Min,
                         const sint32 os32_Max);

sint16 x_in_current_redundant_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter);
sint16 x_in_current_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_Tolerance);

sint16 x_in_current_redundant_f02_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_Tolerance);

sint16 x_in_digital_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint8 ou8_Logic,
                         const uint16 ou16_DebouncePeriod);

sint16 x_in_digital_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_VoltageMin,
                         const sint32 os32_VoltageMax);

sint16 x_in_digital_redundant_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlagsA, const uint8 ou8_LogicA,
                                   const uint32 ou32_CircuitFlagsB, const uint8 ou8_LogicB,
                                   const uint16 ou16_DebouncePeriod);
sint16 x_in_digital_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_in_namur_init(const uint16 ou16_Channel, const uint8 ou8_Logic, const uint16 ou16_DebouncePeriod);
sint16 x_in_namur_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_in_diagswitch_init(const uint16 ou16_Channel, const uint8 ou8_SwitchType, const uint8 ou8_Logic,
                            const uint16 ou16_DebouncePeriod);
sint16 x_in_diagswitch_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_in_freqswitch_init(const uint16 ou16_Channel, const uint8 ou8_SwitchType, const uint8 ou8_Logic,
                            const uint16 ou16_DebouncePeriod, const uint32 ou32_FrequencyMin,
                            const uint32 ou32_FrequencyMax);
sint16 x_in_freqswitch_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_in_frequency_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint8 ou8_Logic,
                           const uint16 ou16_DebouncePeriod);
sint16 x_in_frequency_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_FrequencyMin,
                           const uint32 ou32_FrequencyMax, const uint32 ou32_DutyCycleMin,
                           const uint32 ou32_DutyCycleMax);

sint16 x_in_frequency_redundant_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlagsA, const uint8 ou8_LogicA,
                                     const uint32 ou32_CircuitFlagsB, const uint8 ou8_LogicB,
                                     const uint16 ou16_DebouncePeriod);
sint16 x_in_frequency_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_FrequencyMin,
                                     const uint32 ou32_FrequencyTolRel, const uint32 ou32_FrequencyTolAbs,
                                     const uint32 ou32_DutyCycleTolAbs);

sint16 x_in_incremental_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint8 ou8_Direction,
                             const uint16 ou16_DebouncePeriod);
sint16 x_in_incremental_redundant_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags,
                                       const uint8 ou8_Direction, const uint16 ou16_DebouncePeriod);
sint16 x_in_incremental_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_Tolerance);

sint16 x_in_task_event_init(const uint16 ou16_Channel, const uint8 ou8_Mode, const uint32 ou32_CircuitFlags,
                            const uint16 ou16_DebounceRising, const uint16 ou16_DebounceFalling,
                            T_x_os_task * const opt_Task, const uint32 ou32_TaskEvents);

sint16 x_in_get_pending_events(const uint16 ou16_Word, const uint32 ou32_ClearMask, uint32 * const opu32_Result);
sint16 x_in_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_in_get_sticky_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_in_clear_sticky_faults(const uint16 ou16_Channel, const uint32 ou32_FaultMask);
sint16 x_in_get_fault_health(const uint16 ou16_Channel, const uint32 ou32_Fault, uint32 * const opu32_Health);

sint16 x_in_set_scaling(const uint16 ou16_Channel, const sint32 os32_SourceStart, const sint32 os32_SourceEnd,
                        const sint32 os32_DestStart,   const sint32 os32_DestEnd);

sint16 x_in_set_circuit(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags);
sint16 x_in_get_circuit(const uint16 ou16_Channel, uint32 * const opu32_Circuit);

sint16 x_in_get_digital_raw(const uint16 ou16_Channel, uint8 * const opu8_DigitalState);
sint16 x_in_get_digital_debounced(const uint16 ou16_Channel, uint8 * const opu8_DigitalState);
sint16 x_in_get_digital_counter(const uint16 ou16_Channel, uint32 * const opu32_DigitalCounter);
sint16 x_in_set_digital_counter(const uint16 ou16_Channel, const uint32 ou32_Counter);

sint16 x_in_get_voltage_raw(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_in_get_voltage_filtered(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_in_get_voltage_scaled(const uint16 ou16_Channel, sint32 * const ops32_Scaled);

sint16 x_in_get_current_raw(const uint16 ou16_Channel, sint32 * const ops32_Current);
sint16 x_in_get_current_filtered(const uint16 ou16_Channel, sint32 * const ops32_Current);
sint16 x_in_get_current_scaled(const uint16 ou16_Channel, sint32 * const ops32_Scaled);

sint16 x_in_get_frequency(const uint16 ou16_Channel, uint32 * const opu32_Frequency, uint32 * const opu32_DutyCycle);

sint16 x_in_get_incremental(const uint16 ou16_Channel, sint32 * const ops32_IncrementalCounter,
                            uint8 * const opu8_Direction);
sint16 x_in_get_incremental_redundant(const uint16 ou16_Channel, sint32 * const ops32_CounterMission,
                                      sint32 * const ops32_CounterMonitor, uint8 * const opu8_DirectionMission,
                                      uint8 * const opu8_DirectionMonitor);
sint16 x_in_set_incremental(const uint16 ou16_Channel, const sint32 os32_IncrementalCounter);

sint16 x_in_oneshot_init(const uint16 ou16_Channel, const uint8 ou8_Logic, const uint32 ou32_CircuitFlags,
                         const uint16 ou16_DebounceRising, const uint16 ou16_DebounceFalling,
                         T_x_os_task * const opt_Task, const uint32 ou32_TaskEvents);
sint16 x_in_oneshot_arm(const uint16 ou16_Channel);
sint16 x_in_oneshot_get(const uint16 ou16_Channel, uint32 * const opu32_Duty, uint32 * const opu32_Period);

#ifdef __cplusplus
}
#endif

#endif // X_IN_H___

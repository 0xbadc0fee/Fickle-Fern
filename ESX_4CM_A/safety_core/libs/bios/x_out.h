//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Output API functions
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_OUT_H
#define X_OUT_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "x_stdtypes.h"
#include "x_out_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//! \brief defines for output types
#define X_OUT_TYPE_NONE                   (0x0000u) //!< output has not been initialized yet
#define X_OUT_TYPE_DIGITAL                (0x0001u) //!< output is digital type
#define X_OUT_TYPE_PWM                    (0x0002u) //!< output is PWM type
#define X_OUT_TYPE_CC                     (0x0003u) //!< output is type current controlled
#define X_OUT_TYPE_CC_REDUNDANT           (0x0004u) //!< master channel of current controlled output-pair
#define X_OUT_TYPE_HB_DIGITAL_PP          (0x0005u) //!< output is digital type half-bridge in push-pull mode
#define X_OUT_TYPE_HB_PWM_PP              (0x0006u) //!< output is PWM type half-bridge in push-pull mode
#define X_OUT_TYPE_HB_CC_PP               (0x0007u) //!< output is current controlled type half-bridge in push-pull mode
#define X_OUT_TYPE_FB_DIGITAL_PP          (0x0008u) //!< output is digital type full-bridge in push-pull mode
#define X_OUT_TYPE_FB_PWM_PP              (0x0009u) //!< output is PWM type full-bridge in push-pull mode
#define X_OUT_TYPE_FB_CC_PP               (0x000Au) //!< output is current controlled type full-bridge in push-pull mode
#define X_OUT_TYPE_UNAVAILABLE            (0x000Bu) //!< output is not available for the product (variant)

//! \brief output circuit configuration
#define X_OUT_CIRCUIT_PULL_UP             (0x0001u) //!< use pull up circuit

#define X_OUT_CC_MAX_DITHER_AMPLITUDE       (5000u) //!<maximum dither amplitude for current-controlled output in 0.01%

//! \brief output bridge mode for initialization
#define X_OUT_BRIDGE_MODE_PP              0u //!< bridge mode push-pull

//! \brief special half-/full-bridge state
#define X_OUT_VALUE_PUSH_PULL_TRISTATE    (0x7FFFFFFE) //!< disable push-pull half- or full-bridge output

//! \brief output faults
#define X_OUT_FAULT_SHORT_GND              (1uL << 0uL)             //!< fault output has short to GND
#define X_OUT_FAULT_SHORT_UB_OL            (1uL << 1uL)             //!< fault out has short to UB or open-load
#define X_OUT_FAULT_OVERCURRENT            (1uL << 2uL)             //!< fault output working with over-current
#define X_OUT_FAULT_DUTY_CYCLE_OOOB        (1uL << 3uL)             //!< fault operation out of bounds
#define X_OUT_FAULT_DUTY_CYCLE_TOO_LOW     (1uL << 4uL)             //!< fault output duty-cycle too low
#define X_OUT_FAULT_DUTY_CYCLE_TOO_HIGH    (1uL << 5uL)             //!< fault output duty-cycle too high
#define X_OUT_FAULT_CURRENT_TOO_LOW        (1uL << 6uL)             //!< fault output current too low
#define X_OUT_FAULT_CURRENT_TOO_HIGH       (1uL << 7uL)             //!< fault output current too high
#define X_OUT_FAULT_CURRENT_DEVIATION      (1uL << 8uL)             /*!< fault deviation of the current of two redundant
                                                                         current-controlled outputs to high*/
#define X_OUT_FAULT_CURRENT_DIAG_FUNC      (1uL << 9uL)             //!< fault output current exceeds limit set by
                                                                    // application
#define X_OUT_FAULT_GND_OFFSET             (1uL << 10uL)            //!< fault output GND offset exceeding
#define X_OUT_FAULT_LSGND_OFFS_TOO_HIGH    (X_OUT_FAULT_GND_OFFSET) //!< DEPRECATED: use X_OUT_FAULT_GND_OFFS instead
#define X_OUT_FAULT_LS5V_TOO_HIGH_OR_LOW   (1uL << 11uL)            //!< fault low-side output supply voltage too low or
                                                                    // too high
#define X_OUT_FAULT_SHORT_UB               (1uL << 12uL)            //!< fault output has short to UB
#define X_OUT_FAULT_OPEN_LOAD              (1uL << 13uL)            //!< fault output has open-load
#define X_OUT_FAULT_SHORT_GND_OL           (1uL << 14uL)            //!< fault out has short to GND or open-load
#define X_OUT_FAULT_ALL                     0xFFFFFFFFu             //!< all fault flags set

//! \brief safe state for outputs
#define X_OUT_SAFE_STATE_DISABLED          (0u) //!<normal state: output active
#define X_OUT_SAFE_STATE_ENABLED           (1u) /*!<safe state: fatal error detected or set by application
                                                     -> output inactive*/
/* -- Types --------------------------------------------------------------------------------------------------------- */
//!\brief PID configuration parameters for current-controlled outputs
typedef struct
{
   sint32 s32_Setpoint;       //!< current control setpoint in micro amps (also control start value)
   sint32 s32_P_Value;        //!< proportional gain in 0.001 KR
   sint16 s16_I_Value;        //!< integral time Ti in ms
   sint16 s16_D_Value;        //!< derivation time Td in ms
   sint16 s16_SampleTime;     //!< current control sample time T in ms
   sint16 s16_OutputLimitMin; //!< minimum output duty cycle in 0.01%
   sint16 s16_OutputLimitMax; //!< maximum output duty cycle in 0.01%
} T_x_out_pid_parameters;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

sint16 x_out_digital_init(const uint16 ou16_Channel);
sint16 x_out_digital_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_digital_group_init(const uint16 ou16_Channel, const uint16 ou16_Size);
sint16 x_out_digital_group_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_pwm_init(const uint16 ou16_Channel);
sint16 x_out_pwm_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_pwm_group_init(const uint16 ou16_Channel, const uint16 ou16_Size);
sint16 x_out_pwm_group_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_cc_init(const uint16 ou16_Channel);
sint16 x_out_cc_diag(const uint16 ou16_Channel, const uint16 ou16_PwmDti, const uint16 ou16_CcDti,
                     const sint32 os32_MaxCurrent);
sint16 x_out_cc_diag_v2(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                        const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs);

sint16 x_out_cc_group_init(const uint16 ou16_Channel, const uint16 ou16_Size);
sint16 x_out_cc_group_diag(const uint16 ou16_Channel, const uint16 ou16_PwmDti, const uint16 ou16_CcDti,
                           const sint32 os32_MaxCurrent);
sint16 x_out_cc_group_diag_v2(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                              const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs);

sint16 x_out_cc_redundant_init(const uint16 ou16_Channel);
sint16 x_out_cc_redundant_diag(const uint16 ou16_Channel, const uint16 ou16_PwmDti, const uint16 ou16_CcDti,
                               const sint32 os32_MaxCurrent);
sint16 x_out_cc_redundant_diag_v2(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                                  const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs);

sint16 x_out_cc_set_dither(const uint16 ou16_Channel, const uint16 ou16_DitherAmplitude);
sint16 x_out_cc_activate_current_filter_for_diagnosis(const uint16 ou16_Channel, const uint8 ou8_Status);

sint16 x_out_set_current_filter(const uint16 ou16_Channel, const uint16 ou16_Filter);
sint16 x_out_set_current_avg_filter(const uint16 ou16_Channel, const uint16 ou16_Filter, uint16 * const opu16_Buffer);
sint16 x_out_get_current_filter(const uint16 ou16_Channel, uint16 * const opu16_Filter);
sint16 x_out_set_voltage_filter(const uint16 ou16_Channel, const uint16 ou16_Filter);
sint16 x_out_set_voltage_avg_filter(const uint16 ou16_Channel, const uint16 ou16_Filter, uint16 * const opu16_Buffer);
sint16 x_out_get_voltage_filter(const uint16 ou16_Channel, uint16 * const opu16_Filter);

sint16 x_out_get_type(const uint16 ou16_Channel, uint16 * const opu16_Type);

sint16 x_out_set_circuit(const uint16 ou16_Channel, const uint32 ou32_Circuit);
sint16 x_out_get_circuit(const uint16 ou16_Channel, uint32 * const opu32_Circuit);

sint16 x_out_set_control_parameters(const uint16 ou16_Channel, const T_x_out_pid_parameters * const opt_Parameters);
sint16 x_out_get_control_parameters(const uint16 ou16_Channel, T_x_out_pid_parameters * const opt_Parameters);

sint16 x_out_set_duty_cycle(const uint16 ou16_Channel, const uint32 ou32_DutyCycle);
sint16 x_out_get_duty_cycle(const uint16 ou16_Channel, uint32 * const opu32_DutyCycle);
sint16 x_out_set_duty_cycle_v2(const uint16 ou16_Channel, const sint32 os32_DutyCycle);
sint16 x_out_get_duty_cycle_v2(const uint16 ou16_Channel, sint32 * const ops32_DutyCycle);

sint16 x_out_set_current_setpoint(const uint16 ou16_Channel, const sint32 os32_Current);
sint16 x_out_get_current_setpoint(const uint16 ou16_Channel, sint32 * const ops32_Current);

sint16 x_out_set_digital(const uint16 ou16_Channel, const uint8 ou8_Digital);
sint16 x_out_get_digital(const uint16 ou16_Channel, uint8 * const opu8_Digital);
sint16 x_out_set_digital_v2(const uint16 ou16_Channel, const sint32 os32_Digital);
sint16 x_out_get_digital_v2(const uint16 ou16_Channel, sint32 * const ops32_Digital);

sint16 x_out_set_frequency(const uint16 ou16_Channel, const uint32 ou32_Frequency);
sint16 x_out_get_frequency(const uint16 ou16_Channel, uint32 * const opu32_Frequency);

sint16 x_out_set_ramping(const uint16 ou16_Channel, const uint32 ou32_RisingEdge, const uint32 ou32_FallingEdge);

sint16 x_out_get_current_feedback(const uint16 ou16_Channel, sint32 * const ops32_Current);
sint16 x_out_get_group_current_feedback(const uint16 ou16_Channel, sint32 * const ops32_Current);
sint16 x_out_get_voltage_feedback(const uint16 ou16_Channel, sint32 * const ops32_Voltage);
sint16 x_out_get_digital_feedback(const uint16 ou16_Channel, uint8 * const opu8_Digital);

sint16 x_out_get_sticky_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_out_get_group_sticky_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_out_clear_sticky_faults(const uint16 ou16_Channel, const uint32 ou32_Faults);
sint16 x_out_clear_group_sticky_faults(const uint16 ou16_Channel, const uint32 ou32_Faults);
sint16 x_out_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_out_get_group_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults);
sint16 x_out_get_fault_health(const uint16 ou16_Channel, const uint32 ou32_Fault, uint32 * const opu32_Health);

sint16 x_out_enter_safe_state(const uint16 ou16_Channel);
sint16 x_out_leave_safe_state(const uint16 ou16_Channel);
sint16 x_out_get_safe_state(const uint16 ou16_Channel, uint8 * const opu8_State);

sint16 x_out_digital_half_bridge_init(const uint16 ou16_Channel, const uint16 ou16_Mode);
sint16 x_out_digital_half_bridge_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);
sint16 x_out_digital_half_bridge_group_init(const uint16 ou16_Channel, const uint16 ou16_Mode, const uint16 ou16_Size);
sint16 x_out_digital_half_bridge_group_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_pwm_half_bridge_init(const uint16 ou16_Channel, const uint16 ou16_Mode);
sint16 x_out_pwm_half_bridge_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);
sint16 x_out_pwm_half_bridge_group_init(const uint16 ou16_Channel, const uint16 ou16_Mode, const uint16 ou16_Size);
sint16 x_out_pwm_half_bridge_group_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_cc_half_bridge_init(const uint16 ou16_Channel, const uint16 ou16_Mode);
sint16 x_out_cc_half_bridge_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                                 const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs,
                                 const uint8 ou8_UseTolParams);
sint16 x_out_cc_half_bridge_group_init(const uint16 ou16_Channel, const uint16 ou16_Mode, const uint16 ou16_Size);
sint16 x_out_cc_half_bridge_group_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                                       const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs,
                                       const uint8 ou8_UseTolParams);

sint16 x_out_pwm_full_bridge_init(const uint16 ou16_Channel, const uint16 ou16_Mode);
sint16 x_out_pwm_full_bridge_diag(const uint16 ou16_Channel, const uint16 ou16_Dti);

sint16 x_out_cc_full_bridge_init(const uint16 ou16_Channel, const uint16 ou16_Mode);
sint16 x_out_cc_full_bridge_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                                 const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs,
                                 const uint8 ou8_UseTolParams);

// Backwards compatibility (deprecated)
DEPRECATED // Use x_out_set_current_filter() instead
sint16 x_out_cc_set_filter(const uint16 ou16_Channel, const uint16 ou16_Filter);
DEPRECATED // Use x_out_set_current_avg_filter() instead
sint16 x_out_cc_set_avg_filter(const uint16 ou16_Channel, const uint16 ou16_Filter, uint16 * const opu16_Buffer);

#ifdef __cplusplus
}
#endif

#endif

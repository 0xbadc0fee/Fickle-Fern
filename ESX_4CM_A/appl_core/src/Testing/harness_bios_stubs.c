//----------------------------------------------------------------------------------------------------------------------
/*
 * harness_bios_stubs.c
 *
 *  Host-DLL BIOS stub layer (x_in / x_out / x_msw / x_uext substitution).
 *  See harness_bios_stubs_spec.md for the build spec this file implements.
 *
 *  HOST BUILD ONLY. The real TriCore `.a` BIOS archives cannot link host-side
 *  (wrong architecture) - these stubs satisfy the linker for the MinGW-w64
 *  `.dll` harness variant instead. Never compiled into the TriCore `.hex`
 *  build; keep this file out of that source list.
 *
 *  Tier A symbols (multicore allocation handshake) are inert by construction:
 *  they accept their arguments, touch nothing, and return success. Tier B
 *  symbols (init/diag) have no hardware to configure or fault to report, so
 *  they do the same, populating any out-parameter with a benign "no fault"
 *  value. Tier C symbols (the behavioral surface) read/write harness-owned
 *  storage below - never hardware, never the vendor archive - so a stub can
 *  never call back into the BIOS surface and grow the undefined-symbol list.
 *
 *  Created on: Aug 7, 2026
 *      Author: silas.curfman
 */
//----------------------------------------------------------------------------------------------------------------------

#ifndef SVG_HARNESS
#error "harness_bios_stubs.c is a host-DLL harness artifact; build only with -DSVG_HARNESS (see CLAUDE.md)."
#endif

/* -- Includes ------------------------------------------------------------------------------------------------------ */
// Real vendor headers - included so the compiler checks every stub body against the official signature.
#include "x_out.h"
#include "x_out_client.h"
#include "x_msw.h"
#include "x_uext.h"
#include "x_in.h"
#include "x_in_client.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types ----------------------------------------------------------------------------------------------------------- */

//! \brief Harness-owned state for one digital/PWM/CC output channel (keyed by ou16_Channel, 0..X_OUT_COUNT-1)
typedef struct
{
    uint8  u8_digital;          //!< last value from x_out_set_digital
    uint32 u32_dutyCycle;       //!< last value from x_out_set_duty_cycle
    uint32 u32_frequency;       //!< last value from x_out_set_frequency
    sint32 s32_currentSetpoint; //!< last value from x_out_set_current_setpoint
    uint32 u32_circuit;         //!< last value from x_out_set_circuit
    uint16 u16_currentFilter;   //!< last value from x_out_set_current_filter
    uint16 u16_ccDither;        //!< last value from x_out_cc_set_dither
    T_x_out_pid_parameters t_controlParams; //!< last value from x_out_set_control_parameters
} T_HarnessOutChannel;

//! \brief Harness-owned state for one voltage/current/digital/frequency input channel (keyed by ou16_Channel, 0..X_IN_COUNT-1)
typedef struct
{
    uint8  u8_digitalDebounced; //!< value returned by x_in_get_digital_debounced
    sint32 s32_voltageRaw;      //!< value returned by x_in_get_voltage_raw
    sint32 s32_currentRaw;      //!< value returned by x_in_get_current_raw
    uint32 u32_frequency;       //!< value returned by x_in_get_frequency
    uint32 u32_dutyCycle;       //!< duty cycle returned alongside x_in_get_frequency
} T_HarnessInChannel;

//! \brief Harness-owned state for one main switch channel (keyed by ou16_Channel, 0..X_MSW_COUNT-1)
typedef struct
{
    uint8 u8_state; //!< last value from x_msw_set_state
} T_HarnessMswChannel;

//! \brief Harness-owned state for one UEXT (external sensor supply) channel (keyed by ou16_Channel, 0..X_UEXT_COUNT-1)
typedef struct
{
    sint32 s32_voltageSetpoint; //!< last value from x_uext_set_voltage_setpoint
} T_HarnessUextChannel;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Module Global Variables --------------------------------------------------------------------------------------- */

// Harness-owned storage. NOT hardware, NOT the vendor archive - purely so Tier-C stubs have somewhere to
// bottom out. Statics zero-initialize, so every channel starts "off"/zero with no explicit init needed.
static T_HarnessOutChannel  gat_harnessOut[X_OUT_COUNT];
static T_HarnessInChannel   gat_harnessIn[X_IN_COUNT];
static T_HarnessMswChannel  gat_harnessMsw[X_MSW_COUNT];
static T_HarnessUextChannel gat_harnessUext[X_UEXT_COUNT];

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */
/* -- Implementation ------------------------------------------------------------------------------------------------ */

//======================================================================================================================
// Section A - Tier A, INERT (out-of-scope: multicore allocation)
// link-only, multicore channel allocation not simulated host-side; harness is single-process.
//======================================================================================================================

sint16 x_out_client_await_allocations(const uint32 ou32_Timeout)
{
    (void)ou32_Timeout;
    return C_NO_ERR;
}

sint16 x_in_client_await_allocations(const uint32 ou32_Timeout)
{
    (void)ou32_Timeout;
    return C_NO_ERR;
}

//======================================================================================================================
// Section B - Tier B, INIT (in-scope surface, nothing to configure host-side)
//======================================================================================================================

sint16 x_out_digital_init(const uint16 ou16_Channel)
{
    (void)ou16_Channel;
    return C_NO_ERR;
}

sint16 x_out_pwm_init(const uint16 ou16_Channel)
{
    (void)ou16_Channel;
    return C_NO_ERR;
}

sint16 x_out_cc_init(const uint16 ou16_Channel)
{
    (void)ou16_Channel;
    return C_NO_ERR;
}

sint16 x_in_digital_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint8 ou8_Logic,
                         const uint16 ou16_DebouncePeriod)
{
    (void)ou16_Channel;
    (void)ou32_CircuitFlags;
    (void)ou8_Logic;
    (void)ou16_DebouncePeriod;
    return C_NO_ERR;
}

sint16 x_in_voltage_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter)
{
    (void)ou16_Channel;
    (void)ou32_CircuitFlags;
    (void)ou16_Filter;
    return C_NO_ERR;
}

sint16 x_in_current_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint16 ou16_Filter)
{
    (void)ou16_Channel;
    (void)ou32_CircuitFlags;
    (void)ou16_Filter;
    return C_NO_ERR;
}

sint16 x_in_frequency_init(const uint16 ou16_Channel, const uint32 ou32_CircuitFlags, const uint8 ou8_Logic,
                           const uint16 ou16_DebouncePeriod)
{
    (void)ou16_Channel;
    (void)ou32_CircuitFlags;
    (void)ou8_Logic;
    (void)ou16_DebouncePeriod;
    return C_NO_ERR;
}

//======================================================================================================================
// Section B - Tier B, DIAGNOSTIC (in-scope surface, no fault to report host-side)
// Return C_NO_ERR; any out-parameter is populated with a benign "no fault" value so caller
// fault-gating logic reads a defined value instead of garbage.
//======================================================================================================================

sint16 x_out_digital_diag(const uint16 ou16_Channel, const uint16 ou16_Dti)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    return C_NO_ERR;
}

sint16 x_out_pwm_diag(const uint16 ou16_Channel, const uint16 ou16_Dti)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    return C_NO_ERR;
}

sint16 x_out_cc_diag_v2(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_MaxCurrent,
                        const uint32 ou32_CurrentTolRel, const uint32 ou32_CurrentTolAbs)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    (void)os32_MaxCurrent;
    (void)ou32_CurrentTolRel;
    (void)ou32_CurrentTolAbs;
    return C_NO_ERR;
}

sint16 x_out_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults)
{
    (void)ou16_Channel;
    *opu32_Faults = 0u; // no active faults
    return C_NO_ERR;
}

sint16 x_in_digital_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_VoltageMin,
                         const sint32 os32_VoltageMax)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    (void)os32_VoltageMin;
    (void)os32_VoltageMax;
    return C_NO_ERR;
}

sint16 x_in_voltage_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_Min,
                         const sint32 os32_Max)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    (void)os32_Min;
    (void)os32_Max;
    return C_NO_ERR;
}

sint16 x_in_current_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const sint32 os32_Min,
                         const sint32 os32_Max)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    (void)os32_Min;
    (void)os32_Max;
    return C_NO_ERR;
}

sint16 x_in_frequency_diag(const uint16 ou16_Channel, const uint16 ou16_Dti, const uint32 ou32_FrequencyMin,
                           const uint32 ou32_FrequencyMax, const uint32 ou32_DutyCycleMin,
                           const uint32 ou32_DutyCycleMax)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    (void)ou32_FrequencyMin;
    (void)ou32_FrequencyMax;
    (void)ou32_DutyCycleMin;
    (void)ou32_DutyCycleMax;
    return C_NO_ERR;
}

sint16 x_in_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults)
{
    (void)ou16_Channel;
    *opu32_Faults = 0u; // no active faults
    return C_NO_ERR;
}

// x_msw_* / x_uext_* were demanded by the linker (output_handler_lib.c) but not covered by the
// spec's 31-symbol inventory - see report-back notes. Handled with the same Tier B treatment.

sint16 x_uext_diag(const uint16 ou16_Channel, const uint16 ou16_Dti)
{
    (void)ou16_Channel;
    (void)ou16_Dti;
    return C_NO_ERR;
}

sint16 x_uext_get_active_faults(const uint16 ou16_Channel, uint32 * const opu32_Faults)
{
    (void)ou16_Channel;
    *opu32_Faults = 0u; // no active faults
    return C_NO_ERR;
}

//======================================================================================================================
// Section C - Tier C, BEHAVIORAL (values that must actually move)
// Read/write harness-owned storage keyed by channel - never hardware, never the vendor archive.
//======================================================================================================================

// -- Output setters ---------------------------------------------------------------------------------------------------

sint16 x_out_set_digital(const uint16 ou16_Channel, const uint8 ou8_Digital)
{
    gat_harnessOut[ou16_Channel].u8_digital = ou8_Digital;
    return C_NO_ERR;
}

sint16 x_out_set_duty_cycle(const uint16 ou16_Channel, const uint32 ou32_DutyCycle)
{
    gat_harnessOut[ou16_Channel].u32_dutyCycle = ou32_DutyCycle;
    return C_NO_ERR;
}

sint16 x_out_set_frequency(const uint16 ou16_Channel, const uint32 ou32_Frequency)
{
    gat_harnessOut[ou16_Channel].u32_frequency = ou32_Frequency;
    return C_NO_ERR;
}

sint16 x_out_set_current_setpoint(const uint16 ou16_Channel, const sint32 os32_Current)
{
    gat_harnessOut[ou16_Channel].s32_currentSetpoint = os32_Current;
    return C_NO_ERR;
}

sint16 x_out_set_circuit(const uint16 ou16_Channel, const uint32 ou32_Circuit)
{
    gat_harnessOut[ou16_Channel].u32_circuit = ou32_Circuit;
    return C_NO_ERR;
}

sint16 x_out_set_current_filter(const uint16 ou16_Channel, const uint16 ou16_Filter)
{
    gat_harnessOut[ou16_Channel].u16_currentFilter = ou16_Filter;
    return C_NO_ERR;
}

sint16 x_out_set_control_parameters(const uint16 ou16_Channel, const T_x_out_pid_parameters * const opt_Parameters)
{
    gat_harnessOut[ou16_Channel].t_controlParams = *opt_Parameters;
    return C_NO_ERR;
}

sint16 x_out_cc_set_dither(const uint16 ou16_Channel, const uint16 ou16_DitherAmplitude)
{
    gat_harnessOut[ou16_Channel].u16_ccDither = ou16_DitherAmplitude;
    return C_NO_ERR;
}

// x_msw_set_state - same Tier C treatment as the output setters above (see report-back notes).
sint16 x_msw_set_state(const uint16 ou16_Channel, const uint8 ou8_State)
{
    gat_harnessMsw[ou16_Channel].u8_state = ou8_State;
    return C_NO_ERR;
}

// x_uext_set_voltage_setpoint - same Tier C treatment (see report-back notes).
sint16 x_uext_set_voltage_setpoint(const uint16 ou16_Channel, const sint32 os32_Voltage)
{
    gat_harnessUext[ou16_Channel].s32_voltageSetpoint = os32_Voltage;
    return C_NO_ERR;
}

// -- Input getters -----------------------------------------------------------------------------------------------------

sint16 x_in_get_digital_debounced(const uint16 ou16_Channel, uint8 * const opu8_DigitalState)
{
    *opu8_DigitalState = gat_harnessIn[ou16_Channel].u8_digitalDebounced;
    return C_NO_ERR;
}

sint16 x_in_get_voltage_raw(const uint16 ou16_Channel, sint32 * const ops32_Voltage)
{
    *ops32_Voltage = gat_harnessIn[ou16_Channel].s32_voltageRaw;
    return C_NO_ERR;
}

sint16 x_in_get_current_raw(const uint16 ou16_Channel, sint32 * const ops32_Current)
{
    *ops32_Current = gat_harnessIn[ou16_Channel].s32_currentRaw;
    return C_NO_ERR;
}

sint16 x_in_get_frequency(const uint16 ou16_Channel, uint32 * const opu32_Frequency, uint32 * const opu32_DutyCycle)
{
    *opu32_Frequency = gat_harnessIn[ou16_Channel].u32_frequency;
    *opu32_DutyCycle = gat_harnessIn[ou16_Channel].u32_dutyCycle;
    return C_NO_ERR;
}

//EOF

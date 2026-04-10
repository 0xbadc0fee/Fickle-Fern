//-----------------------------------------------------------------------------
/**
 * \file       output_handler_lib.H
 * \brief      HAL - Output Handler Library
 *
 * \addtogroup HAL
 * @{
 * \addtogroup OutputHandler Output Handler
 *
 * The Output Handler Library provides a standardized interface for managing
 * and commanding hardware outputs. It handles state translation, safety
 * limit enforcement, and diagnostic monitoring for physical actuators
 * and signals across the system.
 *
 * @par Project
 * FloryTemplate_4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Feb 4, 2026 STW Technic
 *
 * @{
 */
//-----------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_HAL_OUTPUT_HANDLER_LIB_H_
#define APPL_CORE_SRC_HAL_OUTPUT_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_out.h"
#include "x_msw.h"
#include "x_out_client.h"
#include "string.h"
#include "osy_com_j1939_dm1.h"
#include <math.h>
#include "alarm_handler_lib.h"



/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DEFAULT_PWM_CC_FREQ 100000u                  //!< Default PWM CC Frequency if not otherwise set
#define DEFAULT_CC_FILTER   20u                     //!< Default 10mS Filter if not otherwise set
#define DEFAULT_CC_DITHER   500u                    //!< Default dither is 5%% if not otherwise set

#define DEFAULT_CC_MAX_CURRENT 1500000              //!< Default 1.5 A max current if not otherwise set
#define DEFAULT_CC_TOL_REL     400                  //!< Default relative tolerance = 0.01% if not otherwise set
#define DEFAULT_CC_TOL_ABS     200000               //!< Default absolute tolerance = 200 mA if not otherwise set

#define DEFAULT_PID_SETPOINT      0                 //!< Default PID set point in micro amps
#define DEFAULT_PID_P             6000              //!< Default proportional gain in 0.001 KP
#define DEFAULT_PID_I             40                //!< Default integral time Ti in ms
#define DEFAULT_PID_D             3                 //!< Default derivative time Td in ms
#define DEFAULT_PID_SAMPLETIME    10                //!< Default PID loop sample time in ms
#define DEFAULT_PID_DUTYCYCLE_MIN 0                 //!< Default PID minimum dutycycle in 0.01%
#define DEFAULT_PID_DUTYCYCLE_MAX 10000             //!< Default PID maximum dutycyle in 0.01%

/* -- Types --------------------------------------------------------------------------------------------------------- */

/**
 * \enum E_OutputTypes
 * \brief List of all Output Types.
 */
typedef enum {
    OT_NONE = 0,
    OT_DIGITAL,      //!< output is digital type
    OT_PWM,          //!< output is PWM type
    OT_CC            //!< output is type current controlled
} E_OutputTypes;

/**
 * \enum E_OutputFaults
 * \brief List of all Possible Output Faults.
 */
typedef enum {

    e_OUTFAULT_NO_FAULT,        //!<No Fault Present
    e_OUTFAULT_SHORT_UB,        //!<Output Short to UB Fault
    e_OUTFAULT_SHORT_GND,      //!<Output Short to GND Fault
    e_OUTFAULT_OL,             //!<Output Open Load Fault
    e_OUTFAULT_OC,             //!<Output Overcurrent Fault
    e_OUTFAULT_DC_OOB,         //!<Duty Cycle OOB Fault
    e_NUM_OUTFAULTS            //!<Total Number of possible output faults
} E_OutputFaults;

/**
 * \struct T_VehicleOutput
 * \brief Struct for a Vehicle Output Object.
 */
typedef struct {
    //-----------------------------INIT PARAMS--------------------------------//
    char *Name_Description;          //!< Named Description of Hardware Output
    E_OutputTypes e_outputType;  //!< Configuration Type of Output
    uint16 u16_hardwareID;       //!< Output ID - Hardware PIN ID
    //-----------------------------OUTPUT VALUES------------------------------//
    float32 f32_outputValue;     //!< Output Value
    float32 f32_prevOutputValue;     //!< Array to track previously set output values
    uint8 mq_outputChanged;          //!< Output Changed Status
    //------------------------------DIAG PARAMS-------------------------------//
    uint8 u8_diagEnabled;        //!< Enable - Disable Toggle for Output Diagnostics / Alarm
    T_FloryFault t_fault;
    uint16 u16_dti;                  //!< Fault Test Interval

} T_VehicleOutput;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_VehicleOutput at_vehicleOutputs[X_OUT_COUNT]; //!< Global array storing the state of all vehicle outputs

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_outputHandler(void);
sint16 update_outputHandler(void);
sint16 add_hwOutput(T_VehicleOutput output);
sint16 get_numOutputs(uint8 *const opu8_Count);
sint16 clear_outputFaults(void);
sint16 get_outputFaultStatus(const char *targetName, uint8 *opu8_status);
sint16 set_outputValue(const char *targetName, float32 value);

#endif /* APPL_CORE_SRC_HAL_OUTPUT_HANDLER_LIB_H_ */


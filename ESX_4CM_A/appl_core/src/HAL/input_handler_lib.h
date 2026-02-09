//-----------------------------------------------------------------------------
/*! \file       input_handler_lib.h
    \brief      <description>

   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2025
   	license     use only under terms of contract / confidential

   	created     Dec 8, 2025 kyle.boch
*/
#ifndef APPL_CORE_SRC_SYSTEM_IO_INPUT_HANDLER_LIB_H_
#define APPL_CORE_SRC_SYSTEM_IO_INPUT_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "x_in.h"
#include "x_in_client.h"
#include "string.h"
#include "osy_com_j1939_dm1.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define DEFAULT_DIG_DEBOUNCE    100 //!< .1ms debounce used on digital input signals
#define DEFAULT_DIG_CIRCUIT       0 //!< Pullup/down disabled
#define DEFAULT_ADCINPUT_FILTER   0 //!< Default ADC input filter value
#define DEFAULT_ADCINPUT_CIRCUIT  0 //!< Default ADC input circuit value

/* -- Types --------------------------------------------------------------------------------------------------------- */

/*! \brief List of all Input Types **/
typedef enum {
    IT_NONE = 0,     //!< Input not configured
    IT_VOLTAGE,      //!< Analog voltage
    IT_CURRENT,      //!< Analog current
    IT_DIGITAL,      //!< Digital input
    IT_FREQ,         //!< Frequency input
    IT_INC,          //!< Incremental input
    IT_FREQSWITCH,   //!< Frequency switch input
} E_InputTypes;

/*! \brief List of all Possible Input Faults **/
typedef enum {

    e_INFAULT_SHORT_UB = 0,   //!<Input Short to UB Fault
    e_INFAULT_SHORT_GND,      //!<Input Short to GND Fault
    e_INFAULT_OL,             //!<Input Open Load Fault
    e_INFAULT_HIGH_FREQ,      //!<Input Frequency High Fault
    e_INFAULT_LOW_FREQ,       //!<Input Frequency Low Fault
    e_INFAULT_HIGH_DC,        //!<Input Duty Cycle High Fault
    e_INFAULT_LOW_DC,         //!<Input Duty Cycle Low Fault
    e_NUM_INFAULTS            //!<Total Number of possible output faults
} E_OutputFaults;


/*! \brief Struct for a Vehicle Input Object **/
typedef struct {
    //-----------------------------INIT PARAMS--------------------------------//
    char *Name_Description;         //!< Named Description of Hardware Output
    E_InputTypes e_inputType;       //!< Configuration Type of Input
    uint16 u16_hardwareID;          //!< Input ID - Hardware PIN ID
    //-----------------------------INPUT VALUES------------------------------//
    float32 f32_inputValue;         //!< Most recent Input Value
    float32 f32_prevInputValue;     //!< Previously captured input value
    uint8 mq_inputChanged;          //!< Input Changed Status
    //-----------------------------DIAG PARAMS-------------------------------//
    uint8 u8_diagEnabled;           //!<Enable - Disable Toggle for Input Diagnostics / Alarm
    uint32 u32_SPN;                 //!<DM1 SPN Assigned to Input
    T_osy_com_j1939_dtc t_dtc[e_NUM_INFAULTS];   //!<DM1 DTC Object - 7 possible faults for inputs
    uint16 u16_dti;                 //!< Fault Test Interval
    sint32 s32_diagMin;             //!<Minimum Diagnostic Parameter (Voltage, Current, etc (based on type))
    sint32 s32_diagMax;             //!<Maximum Diagnostic Parameter (Voltage, Current, etc (based on type))

} T_VehicleInput;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_inputHandler(void);
sint16 update_inputHandler(void);
sint16 add_hwInput(T_VehicleInput input);



#endif /* APPL_CORE_SRC_SYSTEM_IO_INPUT_HANDLER_LIB_H_ */


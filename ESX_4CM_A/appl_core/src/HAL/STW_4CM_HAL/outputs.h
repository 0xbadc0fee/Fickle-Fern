//-----------------------------------------------------------------------------
/**
 * \file        outputs.h
 * \brief       System - Outputs 4CM HAL File
 *
 * \addtogroup HAL
 * @{
 * \addtogroup Outputs Outputs HAL
 *
 * The Outputs module serves as the Hardware Abstraction Layer (HAL) for
 * the 4CM controller.
 *
 * @par Project
 * Flory_8772-4CM
 *
 * @par Copyright
 * STW Technic (c) 2026
 *
 * @par License
 * Use only under terms of contract / confidential
 *
 * @par Created
 * Apr 13, 2026 kyle.boch
 *
 * @{
 */
#ifndef APPL_CORE_SRC_HAL_STW_4CM_HAL_OUTPUTS_H_
#define APPL_CORE_SRC_HAL_STW_4CM_HAL_OUTPUTS_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_out.h"
#include "x_out_client.h"
#include "x_msw.h"
#include "x_uext.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

//Output Mapping
#define OUT_X3K1    OPHSP2A_1
#define OUT_X3K2    OPHSP2A_2
#define OUT_X3K3    OPHSP2A_3
#define OUT_X3K4    OPHSP2A_4
#define OUT_X4J1    OPHSP2A_5
#define OUT_X4J2    OPHSP2A_6
#define OUT_X4J3    OPHSP2A_7
#define OUT_X4J4    OPHSP2A_8
#define OUT_X4G1    OPHSP2A_9
#define OUT_X4G2    OPHSP2A_10
#define OUT_X4G3    OPHSP2A_11
#define OUT_X4G4    OPHSP2A_12
#define OUT_X4H1    OPHSP2A_13
#define OUT_X4H2    OPHSP2A_14
#define OUT_X4H3    OPHSP2A_15
#define OUT_X4H4    OPHSP2A_16

#define OUT_X4K1    OPL2A_1
#define OUT_X4K2    OPL2A_2
#define OUT_X4K3    OPL2A_3
#define OUT_X4K4    OPL2A_4

//Reference Voltage Outputs (Sensor Supplies)
#define VREF_5V_1      X_UEXT_5V_1         //!< 5V fixed voltage supply
#define VREF_5V_2      X_UEXT_5V_2         //!< 5V fixed voltage supply
#define VREF_ADJ_12V_1 X_UEXT_ADJ_5V_12V_1 //!< adjustable 5..12V voltage supply
#define VREF_ADJ_12V_2 X_UEXT_ADJ_5V_12V_2 //!< adjustable 5..12V voltage supply

/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
//set_output_dither();
///set_voltage_filter();
//set_current_filter();

#endif /* APPL_CORE_SRC_HAL_STW_4CM_HAL_OUTPUTS_H_ */


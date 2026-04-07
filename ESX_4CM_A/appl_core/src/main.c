//-----------------------------------------------------------------------------
/* Project:   FloryTemplate_4CM
 * Copyright: STW Technic (c) 2026
 * License:   use only under terms of contract / confidential
 * Created:   Jan 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/**
 * \file       main.c
 * \brief      ESX-4CM-A Template (appl_core)
 *
 * \addtogroup Main Main Entry Point
 * @{
 * \addtogroup Main Main Entry Point
 *
 * This module serves as the primary entry point for the ESX-4CM-A application.
 * It manages the hardware abstraction layer initialization, schedules the
 * cyclic task execution, and coordinates the startup sequences for the
 * AgvChassis, AgvHMI, and AgvWork sub-systems.
 *
 * @{
 */
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "x_stdtypes.h"
#include "x_memtypes.h"
#include "x_sys.h"

#include "x_icc_barrier.h"
#include "x_os.h"
#include "osy_srv.h"

#include "STW_4CM_HAL/system.h"

#include "hw_inputs.h"
#include "hw_outputs.h"


#include "hmi_definition.h"
#include "ethernet_init.h"

#include "nvm_handler.h"
#include "fault_handler.h"
#include "can_handler.h"
#include "checkpoint_handler.h"


#include "hitch_position_control.h"
#include "header_lift_control.h"
#include "auger_cart_control.h"
#include "lighting_control.h"
#include "cleaning_chains_control.h"
#include "front_sweeps_control.h"
#include "rotary_trap_control.h"
#include "stick_box_control.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */
const X_MEM_APPLICATION_INFO T_x_sys_application_information gt_ApplicationInformation =
{
   .acn_Magic              = X_SYS_INFO_APPL_MAGIC,
   .u8_StructVersion       = X_SYS_STRUCT_VERSION,
   .acn_Devicename         = X_SYS_DEVICE_NAME,
   .acn_Date               = __DATE__,
   .acn_Time               = __TIME__,
   .acn_ApplicationName    = "ESX-4CM-A FLORY 8772",
   .acn_ApplicationVersion = "V2.00r2",
   .u8_LenAdditionalInfo   = OSY_FL_LEN_ADDITIONAL_INFO,
   .acn_AdditionalInfo     = " "
};

// Definition of application heap
MEM_APPL_STATIC_HEAP_INT_DSPR uint8 gau8_DPR_HEAP[1024 * 50];
MEM_APPL_STATIC_HEAP_EMEM_SRAM uint8 gau8_EMEM_HEAP[1024 * 50];

// Global variables for main task configuration
const uint32 gu32_TaskTimerTick_us = 500u;              // Scheduler tick in microseconds, range 500 - 1000000
const uint32 gu32_TaskMainMemClass = X_OS_HEAP_ID_FAST; // Memory class used by this task (target specific:
                                                        // X_OS_HEAP_ID_FAST,X_OS_HEAP_ID_SRAM)
const uint32 gu32_TaskMainStackSize = 5000u;            // Task stack size in byte (8..FFFFFFF8)
const uint8 gu8_CpuCacheDisable = 0u;                   // Disable/enable CPU cache (default: enabled)

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Main routine */
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    sint16 s16_Error;
    sint16 s16_Return;
    uint8 u8_ResetRequest;
    uint8 u8_ign_status;

    //Initialize System
    s16_Error  = ethernet_init();       // Initialize Ethernet
    s16_Error += init_canInterfaces();  // Initialize CAN
    s16_Error += osy_srv_init();        // Initialize openSYDE System

    s16_Error += init_hwInputs();       // Initialize HW Inputs
    s16_Error += init_hwOutputs();      // Initialize HW Outputs
    s16_Error += init_nvmParameters();  // Initialize NVM Objects

    s16_Error += init_faultHandler();  // Initialize Fault / Alarm (DM1) Handler



    //Initialize AgvWork Controls
    if(C_NO_ERR == s16_Error)
    {
        s16_Error += init_elevatorControl(&gt_ui, &gt_elevatorCheckpoints, &gt_elevatorConfig); //Initialize Elevator Control
        s16_Error += init_headerControl(&gt_ui, &gt_headerCheckpoints, &gt_headerConfig);
        s16_Error += init_hitchPosControl(&gt_ui, &gt_headerConfig);
        s16_Error += init_augerControl(&gt_ui);
        s16_Error += init_lightControl(&gt_ui); //Initialize Light Control
        s16_Error += init_cChainsControl(&gt_ui, &gt_cleaningShaftCheckpoints);
        s16_Error += init_frontSweepsControl(&gt_ui, &gt_frontSweepsCheckpoints);
        s16_Error += init_rotaryTrapControl(&gt_ui, &gt_rotaryTrapCheckpoints);
        s16_Error += init_stickBControl(&gt_ui, &gt_stickBConfig);
    }

    // Call this to avoid deadlock in case other cores want to use x_icc_barrier_wait_for()
    // NOTE: this must be placed after creating tasks which will use barriers
    s16_Return = x_icc_barrier_wait_for(X_ICC_BARRIER_ID_MAX, X_ICC_BARRIER_TIMEOUT_NOWAIT);
    if (s16_Return != C_BUSY)
    {
      s16_Error += s16_Return;
    }

    system_keep_alive(TRUE);

    //add required startup delay here

    do
    {
        //Run Control Sequence

        //Inputs
        update_hwInputs();
        update_canInputs();

        //Run AgvChassis Controls
        update_lightControl();

        //Run AgvWork Controls
        update_elevatorControl();
        update_headerControl();
        update_hitchPosControl();
        update_augerControl();
        update_cChainsControl();
        update_frontSweepsControl();
        update_rotaryTrapControl();
        update_stickBControl();

        //Outputs
        update_faultHandler();
        update_checkpointHandler();
        update_canOutputs();
        update_hwOutputs();

        u8_ResetRequest = get_system_reset_status();

        s16_Error = get_ignition_status(&u8_ign_status);
        // get state of ignition
        if ((u8_ign_status == FALSE) && (s16_Error == C_NO_ERR))
        {
            //Shutdown Sequence
            //write_nvmParameters();
        }



    }
    while (u8_ResetRequest == FALSE);



   return 0;
}


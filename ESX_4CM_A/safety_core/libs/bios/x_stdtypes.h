//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief      standard defines and types for TriCore GCC projects
   \copyright  Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef X_STDTYPES_H
#define X_STDTYPES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <stddef.h> // we need this include because of the NULL define
#include "stwtypes.h"
#include "stwerrors.h"
#include "gcc_attributes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */

/*STW Deviation Coding Rules v4*/
/*Violated Rule: 20.10*/
/*Circumstances: Language features*/
/*Reason: C preprocessor need two steps*/
/*Potential risks: GCC supports the stringify operators*/
/*Prevention of risks:  nothing to do*/
/*PRQA S 341 1*/
#define STR(s)      #s     // the C preprocessor need two steps
#define XSTR(s)     STR(s) // to stringify a macro...

#ifndef TRUE
#define TRUE        1u // boolean TRUE
#endif

#ifndef FALSE
#define FALSE       0u // boolean FALSE
#endif

#define X_ON        1u // boolean on == TRUE
#define X_OFF       0u // boolean off == FALSE

/* -- Types --------------------------------------------------------------------------------------------------------- */

#endif /* X_STDTYPES_H */

//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       standard error defines for all STW modules
   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef STWERRORS_H
#define STWERRORS_H

/**********************************************************
 * standard error defines
 **********************************************************/

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define C_NO_ERR           (0)
#define C_UNKNOWN_ERR      (-1)
#define C_WARN             (-2)
#define C_DEFAULT          (-3)
#define C_BUSY             (-4)
#define C_RANGE            (-5)
#define C_OVERFLOW         (-6)
#define C_RD_WR            (-7)
#define C_NOACT            (-8)
#define C_COM              (-9)
#define C_CONFIG           (-10)
#define C_CHECKSUM         (-11)
#define C_TIMEOUT          (-12)
#define C_IN_PROGRESS      (-13)

#endif /* _STWERRORS_H */

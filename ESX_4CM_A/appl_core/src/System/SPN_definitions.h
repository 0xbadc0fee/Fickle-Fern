/*! \file       SPN_definitions.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Jan 6, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_SYSTEM_CAN_SPN_DEFINITIONS_H_
#define APPL_CORE_SRC_SYSTEM_CAN_SPN_DEFINITIONS_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define VAR_ASSIGN(tar_value, src_value) do { (tar_value) = (src_value); } while (0)

#define NVM_LIST_BIT(list_id)     (1UL << (list_id))

#define NVM_SYNC_ITEM(list_id, dp_lvalue, ctrl_rvalue, dirty_mask_lvalue) \
    do {                                                                  \
        if ((dp_lvalue) != (ctrl_rvalue))                                 \
        {                                                                 \
            (dp_lvalue) = (ctrl_rvalue);                                  \
            (dirty_mask_lvalue) |= NVM_LIST_BIT((list_id));               \
        }                                                                 \
    } while (0)

/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_SYSTEM_CAN_SPN_DEFINITIONS_H_ */


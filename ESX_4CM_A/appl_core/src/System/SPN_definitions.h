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



//SPN Table

//Hardware Inputs
#define SPN_520100 520100
#define SPN_520101 520101
#define SPN_520102 520102
#define SPN_520103 520103
#define SPN_520104 520104
#define SPN_520105 520105
#define SPN_520106 520106
#define SPN_520107 520107
#define SPN_520108 520108
#define SPN_520109 520109
#define SPN_520110 520110
#define SPN_520111 520111
#define SPN_520112 520112
#define SPN_520113 520113
#define SPN_520114 520114
#define SPN_520115 520115
#define SPN_520116 520116

//Hardware Outputs
#define SPN_520200 520200
#define SPN_520201 520201
#define SPN_520202 520202
#define SPN_520203 520203
#define SPN_520204 520204
#define SPN_520205 520205
#define SPN_520206 520206
#define SPN_520207 520207
#define SPN_520208 520208
#define SPN_520209 520209
#define SPN_520210 520210
#define SPN_520211 520211
#define SPN_520212 520212
#define SPN_520213 520213
#define SPN_520214 520214
#define SPN_520215 520215
#define SPN_520216 520216
#define SPN_520217 520217
#define SPN_520218 520218
#define SPN_520219 520219
#define SPN_520220 520220
#define SPN_520221 520221
#define SPN_520222 520222
#define SPN_520223 520223
#define SPN_520224 520224
#define SPN_520225 520225
#define SPN_520226 520226




/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

#endif /* APPL_CORE_SRC_SYSTEM_CAN_SPN_DEFINITIONS_H_ */


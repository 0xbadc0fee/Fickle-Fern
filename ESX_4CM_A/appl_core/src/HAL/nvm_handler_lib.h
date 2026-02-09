/*! \file       nvm_handler_lib.h
    \brief      <description>


   	\implementation
   	project     FloryTemplate_4CM
   	copyright   STW Technic (c) 2026
   	license     use only under terms of contract / confidential

   	created     Feb 5, 2026 kyle.boch
   	\endimplementation
*/
#ifndef APPL_CORE_SRC_HAL_NVM_HANDLER_LIB_H_
#define APPL_CORE_SRC_HAL_NVM_HANDLER_LIB_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_nvm.h"
#include "osy_dph_nvm_handler.h"
#include "osy_dph_data_set_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define NVM_NO_FAULT                 0  //!<NVM_NO_FAULT
#define NVM_EEPROM_OVERFLOW         -1  //!<NVM_EEPROM_OVERFLOW
#define NVM_EEPROM_DATA_OVERFLOW    -2  //!<NVM_EEPROM_DATA_OVERFLOW

#define NVM_FAULTS_START_ADDR    30000  //!<Start address index of the Faults Block in EEPROM
#define NVM_FAULTS_SIZE           1000  //!<Size of the Faults Block in EEPROM
/* -- Types --------------------------------------------------------------------------------------------------------- */
/*! \brief Struct for a Fault NVM Object Header**/
typedef struct
{
    uint32 u32_spn;         //!<SPN of Data to be stored
    uint8  u8_fmi;          //!<FMI of Data to be stored
    uint8  u8_dataLength;   //!<Length of Data to be stored
} T_HeaderNVM;

/*! \brief Struct for a Fault NVM Object Data**/
typedef struct
{
    uint8 data;             //!<Stored Data
} T_DataNVM;

/*! \brief Struct for a Fault NVM Object **/
typedef struct
{
    uint32 u32_spn;             //!<SPN of Object
    uint8  u8_fmi;              //!<FMI of Object
    uint16 u16_memAddress;      //!<Memory Storage address of Object
    uint8  u8_dataLength;       //!<Data length of Object
    uint8  u8_occurenceCount;   //!<Data (Occurence Count) of Object
} T_FaultNVM;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 fault_nvm_init(T_FaultNVM* mem, uint32 _spn, uint8 _fmi, uint8 _defaultVal);
uint8 fault_nvm_read(const T_FaultNVM* mem);
void fault_nvm_write(T_FaultNVM* mem, uint8 newVal);
void clear_fault_nvm(void);

sint16 init_osyNVMDatapools(uint8 u8_datapoolID, uint8 u8_numLists);
sint16 write_osyNVMDatapool(uint8 u8_datapoolID, uint8 u8_list);
sint16 apply_osyNVMDataset(uint8 u8_datapoolID, uint8 u8_list, uint8 u8_dataset);

#endif /* APPL_CORE_SRC_HAL_NVM_HANDLER_LIB_H_ */


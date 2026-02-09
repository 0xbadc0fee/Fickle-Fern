//-----------------------------------------------------------------------------
/*! \file       nvm_handler_lib.c
    \brief      <description>

    project     FloryTemplate_4CM
    copyright   STW Technic (c) 2026
    license     use only under terms of contract / confidential

    created     Feb 5, 2026 STW Technic
*/
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
//STW
//PROJECT
#include "nvm_handler_lib.h"

//STD
#include <string.h>

//STW BIOS
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_nvm.h"
#include "osy_dph_nvm_handler.h"
#include "osy_dph_data_set_handler.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Module Global Function Prototypes ---------------------------------------------------------------------------- */
/* -- Module Global Variables -------------------------------------------------------------------------------------- */

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

/*! \brief Initialize an NVM Fault Object
 *
 *  This function initializes a NVM fault object.
 *
 *  This function runs a search through the FAULTS specific block of EEPROM (30000-31000)
 *  and determines if a NVM entry has been made for a specific spn and fmi combination.
 *
 *  If a nvm entry is found pertaining to that specific spn and fmi, the value in EEPROM is
 *  read into RAM for quick access.
 *
 *  If a nvm entry is not found pertaining to that specific spn and fmi, a new entry is created
 *  and the _defaultValue passed to it is written into RAM and EEPROM.
 *
 *  \param mem Fault NVM Object
 *  \param _spn SPN of Fault Object to be initialized
 *  \param _fmi FMI of Fault Object to be initialized
 *  \param _defaultValue default value to be written into EEPROM if existing object is not found
 *
 *  \retval NVM_NO_FAULT (0)
 *  \retval NVM_EEPROM_OVERFLOW(-1)
 */
sint16 fault_nvm_init(T_FaultNVM* mem, uint32 _spn, uint8 _fmi, uint8 _defaultVal)
{
    uint16 index = NVM_FAULTS_START_ADDR;
    uint16 u16_endAddress = NVM_FAULTS_START_ADDR + NVM_FAULTS_SIZE;

    T_HeaderNVM t_hdr;
    T_DataNVM   t_data;
    uint8 u8_hdrSize = sizeof(T_HeaderNVM);
    uint8 u8_dataSize = sizeof(T_DataNVM);
    sint16 errorStatus = NVM_NO_FAULT;
    mem->u32_spn = _spn;
    mem->u8_fmi  = _fmi;
    mem->u8_dataLength = sizeof(uint8);
    uint8 q_runSearch = TRUE;

    while (q_runSearch)
    {
        if ((index + u8_hdrSize + u8_dataSize) < u16_endAddress)
        {
            x_nvm_read(index, u8_hdrSize, (uint8*)&t_hdr);

            if (t_hdr.u32_spn == mem->u32_spn && t_hdr.u8_fmi == mem->u8_fmi && t_hdr.u8_dataLength == mem->u8_dataLength)
            {
                q_runSearch = FALSE;
                x_nvm_read(index + u8_hdrSize, sizeof(uint8), (uint8*)&t_data.data);
            }
            else if (t_hdr.u32_spn == 0xFFFFFFFF)
            {
                q_runSearch = FALSE;
                t_hdr.u32_spn = mem->u32_spn;
                t_hdr.u8_dataLength = mem->u8_dataLength;

                t_data.data = _defaultVal;

                x_nvm_write(index, u8_hdrSize, (uint8*)&t_hdr);
                x_nvm_write(index + u8_hdrSize, u8_dataSize, (uint8*)&t_data);
            } else
            {
                index++;
            }
        }
        else
        {
            q_runSearch = FALSE;
            errorStatus = NVM_EEPROM_OVERFLOW;
        }
    }

    if (errorStatus == NVM_NO_FAULT)
    {
        mem->u16_memAddress = index;
        mem->u8_occurenceCount = t_data.data;
    }
    return errorStatus;
}

/*! \brief Read NVM Value
 *
 *  Return the value from the Fault NVM Object
 *
 *  \param mem Fault NVM Object
 */
uint8 fault_nvm_read(const T_FaultNVM* mem)
{
    return mem->u8_occurenceCount;
}

/*! \brief Write value to NVM (EEPROM)
 *
 *  Take the value stored in the NVM object RAM and write it into EEPROM memory
 *
 *  \param mem Fault NVM Object
 *  \param newVal New Value to be written into EEPROM
 */
void fault_nvm_write(T_FaultNVM* mem, uint8 u8_newVal)
{
    mem->u8_occurenceCount = u8_newVal;
    x_nvm_write(mem->u16_memAddress + sizeof(T_HeaderNVM), sizeof(uint8), (uint8*)&mem->u8_occurenceCount);
}

/*! \brief Clear the NVM Fault OC Block
 *
 *  Clear the entirety of the fault tracking NVM block to 0xFF
 *
 *  \Note
 *  All registered DM1 Fault codes will be entered back into NVM
 *  with an occurence count of 0 at next controller start up.
 *
*/
void clear_fault_nvm(void)
{
    uint8 eraseBuffer[32];
    memset(eraseBuffer, 0xFF, sizeof(eraseBuffer));

    for (uint16 addr =NVM_FAULTS_START_ADDR; addr < NVM_FAULTS_START_ADDR+NVM_FAULTS_SIZE; addr += sizeof(eraseBuffer))
    {
        uint8 remaining = x_nvm_get_size() - addr;
        uint8 chunkSize = (remaining < sizeof(eraseBuffer)) ? remaining : sizeof(eraseBuffer);
        x_nvm_write(addr, chunkSize, eraseBuffer);
    }
}


/** \brief Initialize all OSY NVM Datapools into RAM
 *
 *  Read into RAM (global osy variables) all the values from EEPROM.
 *
 *  \param u8_datapoolID ID of the NVM Datapool to be initialized
 *  \param u8_numLists Number of lists in corresponding datapool
 *  \return
 */
sint16 init_osyNVMDatapools(uint8 u8_datapoolID, uint8 u8_numLists)
{
    sint16 s16_error = C_NO_ERR;

    for(uint8 i = 0; i< u8_numLists; i++)
    {
        s16_error |= osy_dph_nvm_read_list(u8_datapoolID, i);
    }

    return s16_error;

}


/** \brief Write OSY NVM Datapool List into NVM
 *
 *  Write a specific datapool list into NVM
 *
 *  \param u8_datapoolID ID of the NVM Datapool to be written
 *  \param u8_listID ID of the NVM Datapool List to be written
 *  \return
 */
sint16 write_osyNVMDatapool(uint8 u8_datapoolID, uint8 u8_list)
{
    sint16 s16_error = C_NO_ERR;

    s16_error = osy_dph_nvm_write_list(u8_datapoolID, u8_list);

    return s16_error;

}

/** \brief Apply Dataset to NVM Datapool List
 *
 *  Populate all RAM globals with the values from the desired dataset and
 *  immediately update the NVM values.
 *
 *  \param u8_datapoolID ID of the NVM Datapool to be written
 *  \param u8_listID ID of the NVM Datapool List to be written
 *  \param u8_dataset ID of the dataset that is to be applied.
 *  \return
 */
sint16 apply_osyNVMDataset(uint8 u8_datapoolID, uint8 u8_list, uint8 u8_dataset)
{
    sint16 s16_error = C_NO_ERR;

    s16_error |= osy_dph_apply_data_set(u8_datapoolID, u8_list, u8_dataset);
    s16_error |= osy_dph_nvm_write_list(u8_datapoolID, u8_list);

    return s16_error;

}



//EOF

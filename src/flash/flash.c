#include "flash.h"

void flash_init(void)
{
    /* Write MSC unlock code to enable interface */
    MSC->LOCK = MSC_UNLOCK_CODE;
    /* Enable memory controller */
    MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
}


/*
Return codes for writing/erasing Flash.
typedef enum {
  mscReturnOk          =  0, < Flash write/erase successful. 
  mscReturnInvalidAddr = -1, < Invalid address. Write to an address that is not Flash. 
  mscReturnLocked      = -2, < Flash address is locked. 
  mscReturnTimeOut     = -3, < Timeout while writing to Flash. 
  mscReturnUnaligned   = -4  < Unaligned access to Flash. 
} MSC_Status_TypeDef;

*/
MSC_RAMFUNC_DEFINITION_BEGIN
void flash_write(uint32_t address, uint32_t data, uint32_t numBytes)
{
    /* Write MSC unlock code to enable interface */
    MSC->LOCK = MSC_UNLOCK_CODE;
    /* Enable memory controller */
    MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
    // Load ADDRB register with address
    MSC->ADDRB = address;
    // Load the address by setting LADDRIM bit in MSC_WRITECMD register
    MSC->WRITECMD |= MSC_WRITECMD_LADDRIM;
    //Load Data value
    MSC->WDATA = data;
    //write once command
    MSC->WRITECMD |= MSC_WRITECMD_WRITEONCE;
    while((MSC->STATUS & MSC_STATUS_BUSY)){}

}
MSC_RAMFUNC_DEFINITION_END

MSC_RAMFUNC_DEFINITION_BEGIN
void flash_erase_page(uint32_t address)
{
    /* Write MSC unlock code to enable interface */
    MSC->LOCK = MSC_UNLOCK_CODE;
    /* Enable memory controller */
    MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
    // Load ADDRB register with address
    MSC->ADDRB = address;
    // Load the address by setting LADDRIM bit in MSC_WRITECMD register
    MSC->WRITECMD |= MSC_WRITECMD_LADDRIM;
    // Erase Page
    MSC->WRITECMD |= MSC_WRITECMD_ERASEPAGE;
    while((MSC->STATUS & MSC_STATUS_BUSY)){}
}
MSC_RAMFUNC_DEFINITION_END

uint32_t flash_deinit(void)
{
    MSC_Deinit();
    return MSC->LOCK;
}

uint32_t flash_read_address(uint32_t *address)
{
    return *address;
}
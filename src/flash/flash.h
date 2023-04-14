#ifndef FLASH_H
#define FLASH_H

#include "em_msc.h"
#include "em_core.h"

void flash_init(void);
void flash_write(uint32_t address, uint32_t data, uint32_t numBytes);
void flash_erase_page(uint32_t address);
uint32_t flash_deinit(void);
uint32_t flash_read_address(uint32_t * address);

#endif
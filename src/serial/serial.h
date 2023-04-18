#ifndef SERIAL_H
#define SERIAL_H

#include "em_gpio.h"
#include "em_usart.h"

extern USART_TypeDef *usart;

void serial_init();
void serial_send_string(char* tx_buff, uint8_t buff_len);


#endif
#ifndef SERIAL_H
#define SERIAL_H

#include "em_gpio.h"
#include "em_usart.h"
#include "flash.h"

extern USART_TypeDef *usart;

void serial_init();
void serial_send_string(char* tx_buff, uint8_t buff_len);
void welcom_message(void);
void serial_main(volatile char *input_c);


#endif
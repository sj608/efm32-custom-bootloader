#include "serial.h"

USART_TypeDef *usart = USART2;

static char string_1[] = "Option 1 selected\r\n";
static char string_2[] = "Option 2 selected\r\n";
static char string_3[] = "Option 3 Read memeory\r\n";


void serial_init()
{
    GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortC, 3, gpioModeInput, 0);

    USART_InitAsync_TypeDef init_async = USART_INITASYNC_DEFAULT;

	// Prepare struct for initializing UART in asynchronous mode
	init_async.enable       = usartDisable;   // Don't enable UART upon intialization
	init_async.refFreq      = 0;              // Provide information on reference frequency. When set to 0, the reference frequency is
	init_async.oversampling = usartOVS16;     // Oversampling. Range is 4x, 6x, 8x or 16x, default is 16x
	init_async.databits     = usartDatabits8; // Number of data bits. Range is 4 to 10
	init_async.parity       = usartNoParity;  // Parity mode
	init_async.stopbits     = usartStopbits1; // Number of stop bits. Range is 0 to 2
    init_async.baudrate     = 115200;

    USART_InitAsync(usart, &init_async);

	// Prepare UART Rx and Tx interrupts
	USART_IntClear(usart, _USART_IF_MASK);
	USART_IntEnable(usart, USART_IF_RXDATAV);

	NVIC_ClearPendingIRQ(USART2_RX_IRQn);
	// NVIC_ClearPendingIRQ(USART2_TX_IRQn);
	NVIC_EnableIRQ(USART2_RX_IRQn);
	// NVIC_EnableIRQ(USART2_TX_IRQn);

	NVIC_SetPriority(USART2_RX_IRQn, 0);
	// NVIC_SetPriority(USART2_TX_IRQn, 0);

    usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | _USART_ROUTE_LOCATION_LOC0;

    USART_Enable(usart, usartEnable);
}

void serial_send_string(char* tx_buff, uint8_t buff_len)
{
    for (uint8_t i = 0; i<buff_len; i++){
        USART_Tx(usart, *(tx_buff+i));
    }
}

void welcom_message(void)
{
	char welcome_msg[] = "Welcome to bootlaoder\r\n\n Please input option 1~10 \r\n\n";
	serial_send_string(welcome_msg, (uint8_t)(sizeof(welcome_msg)/(sizeof(welcome_msg[0]))));
}

void serial_main(volatile char *input_c)
{
	if(*input_c == '1'){
	    serial_send_string(string_1, (uint8_t)(sizeof(string_1)/sizeof(string_1[0])));
	    *input_c = 0;
	}else if (*input_c == '2'){
	    serial_send_string(string_2, (uint8_t)(sizeof(string_2)/sizeof(string_2[0])));
		*input_c = 0;
		uint32_t target_addr = 0x1FE00;
		uint32_t data = 698979;
		CORE_DECLARE_IRQ_STATE;
		CORE_ENTER_CRITICAL();
		flash_init();
		flash_erase_page(target_addr);
		flash_write(target_addr, data, 1);
		target_addr = 0x0FE00000;
		flash_write(target_addr, data, 1);
		CORE_EXIT_CRITICAL();
	}else if (*input_c == '3'){
		*input_c = 0;
		serial_send_string(string_3, (uint8_t)(sizeof(string_3)/sizeof(string_3[0])));
		uint32_t mem_content = 0;
		char data_str[4];
		mem_content = flash_read_address((uint32_t*)(0x0FE081FE)); //expected value = 0x47 or 71
		for(uint8_t i = 0; i<4; i++){
			data_str[i] = mem_content>>(24-(8*i));
		}
		serial_send_string(data_str, (uint8_t)(sizeof(data_str)/sizeof(data_str[0])));
	}
}

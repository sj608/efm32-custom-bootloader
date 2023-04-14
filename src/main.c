#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_msc.h"
#include "em_gpio.h"
#include "serial.h"

volatile uint32_t msTicks = 0;
volatile char rx_char = 0;

void Delay(uint32_t dlyTicks);

int main (void)
{
    CHIP_Init();
    
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART2, true);

    if(SysTick_Config(SystemCoreClock / 1000)){
        while(1);
    }

    serial_init();
    GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 0);

    while(1){
        GPIO_PinOutClear(gpioPortC, 0);
        Delay(500);
        GPIO_PinOutSet(gpioPortC, 0);
        Delay(500);
        serial_main(&rx_char);
    }

}

void SysTick_Handler(void)
{
    msTicks++;
}

void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;
    curTicks = msTicks;
    while((msTicks - curTicks) < dlyTicks);
}

void USART2_RX_IRQHandler(void)
{
    USART_IntClear(usart, USART_IF_RXDATAV);
    rx_char = USART_Rx(usart);
}

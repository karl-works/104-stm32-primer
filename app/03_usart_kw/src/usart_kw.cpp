//SPDX-License-Identifier: MIT

#include <kw-stm-g0/drivers/pin.h>
#include <kw-stm-g0/drivers/usart.h>

#include <etl/string.h>

#include "stm32g0xx_ll_usart.h"
#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>

namespace
{

kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led;
kw::stm::g0::drivers::InPin<GPIOC_BASE, LL_GPIO_PIN_13> nucleo_button;
kw::stm::g0::drivers::Usart<USART2_BASE, 115200> nucleo_virtual_usart;



} 
void InitGPIO()
{
    nucleo_led.Init();
    nucleo_button.Init();
}

int main(void)
{
    nucleo_led.Init();
    nucleo_button.Init();
    nucleo_virtual_usart.Init();

    while (1)
    {
        if (nucleo_button.IsSet())
        {

            nucleo_led.Set();
        }
        else
        {
            nucleo_virtual_usart.Transmit( etl::make_string_view(u8"Test String"));
            nucleo_led.Reset();
        }
    }
}

//SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>

namespace
{

kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led;

constexpr auto BUTTON_PORT = GPIOC_BASE;
constexpr auto BUTTON_PIN = LL_GPIO_PIN_13;

} 


void InitGPIO()
{
    nucleo_led.Init();
    
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN, LL_GPIO_PULL_NO);

}

int main(void)
{
    InitGPIO();
    while (1)
    {
        if (LL_GPIO_IsInputPinSet(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN))
        {
            nucleo_led.Set();
        }
        else
        {
            nucleo_led.Reset();
        }
    }
}

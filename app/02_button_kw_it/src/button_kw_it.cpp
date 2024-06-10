//SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"
#include "kw-stm-g0/drivers/pin_exti.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_exti.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>
#include <vector>

namespace
{

kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led;
kw::stm::g0::drivers::InPin<GPIOC_BASE, LL_GPIO_PIN_13> nucleo_button;
kw::stm::g0::drivers::PinExti<LL_EXTI_CONFIG_PORTC, LL_EXTI_LINE_13, LL_EXTI_MODE_IT, LL_EXTI_TRIGGER_FALLING> nucleo_button_exti;

} // namespace

#ifdef __cplusplus
extern "C"
{
#endif
    void EXTI4_15_IRQHandler(void)
    {

        if (nucleo_button_exti.IRS() == kw::stm::g0::drivers::PinExtiTrigger::FALLING)
        {
            nucleo_led.Toggle();
        }
    }
#ifdef __cplusplus
}
#endif

void InitGPIO()
{
    nucleo_led.Init();
    nucleo_button.Init();
    nucleo_button_exti.Init();
    nucleo_button_exti.SetPriority(0);
    nucleo_button_exti.Enable();
}

int main(void)
{
    InitGPIO();
    while (1)
    {
    }
}

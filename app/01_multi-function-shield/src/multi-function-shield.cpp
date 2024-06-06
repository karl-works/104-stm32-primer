// SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>

namespace
{

    kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> MFS_D1;
    kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_6> MFS_D2;
    kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_7> MFS_D3;
    kw::stm::g0::drivers::OutPin<GPIOB_BASE, LL_GPIO_PIN_0> MFS_D4;

} // namespace

extern "C"
{
    void SysTick_Handler(void)
    {
        MFS_D1.Toggle();
        MFS_D2.Toggle();
        MFS_D3.Toggle();
        MFS_D4.Toggle();
    }
}

void InitGPIO()
{

    MFS_D1.Init();
    MFS_D2.Init();
    MFS_D3.Init();
    MFS_D4.Init();

    MFS_D1.Set();
    MFS_D2.Reset();
    MFS_D3.Set();
    MFS_D4.Reset();
}

int main(void)
{
    InitGPIO();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 8);
    while (1)
    {
    }
}

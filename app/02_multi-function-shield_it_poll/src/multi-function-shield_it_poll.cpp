//SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"
#include "kw-stm-g0/drivers/pin_exti.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_exti.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>


namespace
{

kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_5> MFS_D1;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_6> MFS_D2;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_7> MFS_D3;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOB_BASE, LL_GPIO_PIN_0> MFS_D4;

kw::stm::g0::drivers::InPin<GPIOA_BASE, LL_GPIO_PIN_1> MFS_S1;
kw::stm::g0::drivers::InPin<GPIOA_BASE, LL_GPIO_PIN_4> MFS_S2;
kw::stm::g0::drivers::InPin<GPIOB_BASE, LL_GPIO_PIN_1> MFS_S3;

} // namespace

extern "C"
{
    void SysTick_Handler(void)
    {
        MFS_D4.Toggle();
        if (MFS_S1.IsSet())
        {
            MFS_D1.Set();
        }
        else
        {
            MFS_D1.Reset();
        }

        if (MFS_S2.IsSet())
        {
            MFS_D2.Set();
        }
        else
        {
            MFS_D2.Reset();
        }
        if (MFS_S3.IsSet())
        {
            MFS_D3.Set();
        }
        else
        {
            MFS_D3.Reset();
        }
    }
}

void InitGPIO()
{

    MFS_S1.Init();
    MFS_S2.Init();
    MFS_S3.Init();

    MFS_D1.Init();
    MFS_D2.Init();
    MFS_D3.Init();
    MFS_D4.Init();

    MFS_D1.Set();
    MFS_D2.Set();
    MFS_D3.Set();
    MFS_D4.Set();
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

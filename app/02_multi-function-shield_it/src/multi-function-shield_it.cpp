//SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"
#include "kw-stm-g0/drivers/pin_exti.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>
#include <stm32g0xx_ll_exti.h>

namespace
{

kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_5> MFS_D1;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_6> MFS_D2;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOA_BASE, LL_GPIO_PIN_7> MFS_D3;
kw::stm::g0::drivers::OutOpenDrainPin<GPIOB_BASE, LL_GPIO_PIN_0> MFS_D4;

kw::stm::g0::drivers::InPin<GPIOA_BASE, LL_GPIO_PIN_1> MFS_S1;
kw::stm::g0::drivers::InPin<GPIOA_BASE, LL_GPIO_PIN_4> MFS_S2;
kw::stm::g0::drivers::InPin<GPIOB_BASE, LL_GPIO_PIN_1> MFS_S3;

kw::stm::g0::drivers::PinExti<LL_EXTI_CONFIG_PORTA, LL_EXTI_LINE_1, LL_EXTI_MODE_IT, LL_EXTI_TRIGGER_FALLING>
    mfs_s1_exti;
kw::stm::g0::drivers::PinExti<LL_EXTI_CONFIG_PORTA, LL_EXTI_LINE_4, LL_EXTI_MODE_IT, LL_EXTI_TRIGGER_FALLING>
    mfs_s2_exti;

/*
The STM32 generaly do not support this. RM0444 13.3.3  EXTI mux has the details.
The problem is that both are using the same pin number but on diffrent ports RM0444 Figure 29. EXTI GPIO mux
kw::stm::g0::drivers::PinExti<LL_EXTI_CONFIG_PORTB, LL_EXTI_LINE_1, LL_EXTI_MODE_IT, LL_EXTI_TRIGGER_FALLING>
   mfs_s3_exti;
*/

} // namespace

extern "C"
{
    void SysTick_Handler(void)
    {
        MFS_D4.Toggle();
    }

    void EXTI0_1_IRQHandler(void)
    {
        if (mfs_s1_exti.IRS() == kw::stm::g0::drivers::PinExtiTrigger::FALLING)
        {
            MFS_D1.Toggle();
        }
      
    }

    void EXTI4_15_IRQHandler(void)
    {
     
        if (mfs_s2_exti.IRS() == kw::stm::g0::drivers::PinExtiTrigger::FALLING)
        {
            MFS_D2.Toggle();
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


    mfs_s1_exti.Init();
    mfs_s1_exti.SetPriority(0);
    mfs_s1_exti.Enable();


    mfs_s2_exti.Init();
    mfs_s2_exti.SetPriority(0);
    mfs_s2_exti.Enable();


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

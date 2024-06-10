//SPDX-License-Identifier: MIT

#include "kw-stm-g0/drivers/pin.h"

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_exti.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>

namespace
{

kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led;

constexpr auto BUTTON_PORT = GPIOC_BASE;
constexpr auto BUTTON_PIN = LL_GPIO_PIN_13;
constexpr auto BUTTON_EXTI_LINE = LL_EXTI_LINE_13;
constexpr auto BUTTON_EXTI_IRQn = EXTI4_15_IRQn;
constexpr auto BUTTON_EXTI_CONFIG_PORT = LL_EXTI_CONFIG_PORTC;
constexpr auto BUTTON_LL_EXTI_CONFIG_LINE = LL_EXTI_CONFIG_LINE13;

} // namespace



#ifdef __cplusplus
extern "C"
{
#endif
    void EXTI4_15_IRQHandler(void)
    {

        if (LL_EXTI_IsActiveFallingFlag_0_31(BUTTON_EXTI_LINE) != RESET)
        {
            LL_EXTI_ClearFallingFlag_0_31(BUTTON_EXTI_LINE);
            nucleo_led.Toggle();
        }
    }
#ifdef __cplusplus
}
#endif

void InitGPIO()
{
    nucleo_led.Init();

    /* -1- GPIO Config */
    /* Enable GPIO Clock (to be able to program the configuration registers) */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    /* Configure IO */
    LL_GPIO_SetPinMode(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN, LL_GPIO_PULL_NO);

    /* -2- Connect External Line to the GPIO*/
    LL_EXTI_SetEXTISource(BUTTON_EXTI_CONFIG_PORT, BUTTON_LL_EXTI_CONFIG_LINE);

    /*-3- Enable a falling trigger EXTI line 13 Interrupt */
    LL_EXTI_EnableIT_0_31(BUTTON_EXTI_LINE);
    LL_EXTI_EnableFallingTrig_0_31(BUTTON_EXTI_LINE);

    /*-4- Configure NVIC for EXTI4_15_IRQn */
    NVIC_EnableIRQ(BUTTON_EXTI_IRQn);
    NVIC_SetPriority(BUTTON_EXTI_IRQn, 0);
}

int main(void)
{
    InitGPIO();
    while (1)
    {
        /*
        if (LL_GPIO_IsInputPinSet(reinterpret_cast<GPIO_TypeDef *>(BUTTON_PORT), BUTTON_PIN))
        {
            nucleo_led.Reset();
        }
        else
        {
            nucleo_led.Set();
        }*/
    }
}

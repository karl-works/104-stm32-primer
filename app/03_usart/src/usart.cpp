//SPDX-License-Identifier: MIT

#include <kw-stm-g0/drivers/pin.h>

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
constexpr auto USART_PORT_BASE = GPIOA_BASE;
constexpr auto USART_RESOURCE_BASE = USART2_BASE;
constexpr auto USART_RESOURCE_CLKSOURCE = LL_RCC_USART2_CLKSOURCE;
constexpr auto USART_RESOURCE_IRQn = USART2_LPUART2_IRQn;
constexpr auto USART_TX_PIN = LL_GPIO_PIN_2;
constexpr auto USART_RX_PIN = LL_GPIO_PIN_3;
constexpr auto USART_SPEED = 115200;

} // namespace

void InitUSART()
{
    const auto USART_PORT = reinterpret_cast<GPIO_TypeDef *>(USART_PORT_BASE);
    const auto USART_RESOURCE = reinterpret_cast<USART_TypeDef *>(USART_RESOURCE_BASE);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetPinMode(USART_PORT, USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinSpeed(USART_PORT, USART_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(USART_PORT, USART_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(USART_PORT, USART_TX_PIN, LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin_0_7(USART_PORT, USART_TX_PIN, LL_GPIO_AF_1);

    LL_GPIO_SetPinMode(USART_PORT, USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinSpeed(USART_PORT, USART_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(USART_PORT, USART_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(USART_PORT, USART_RX_PIN, LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin_0_7(USART_PORT, USART_RX_PIN, LL_GPIO_AF_1);

    LL_USART_Disable(USART_RESOURCE);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
    LL_USART_SetBaudRate(USART_RESOURCE, HSI_VALUE, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, USART_SPEED);
    LL_USART_SetDataWidth(USART_RESOURCE, LL_USART_DATAWIDTH_8B);
    LL_USART_SetStopBitsLength(USART_RESOURCE, LL_USART_STOPBITS_1);
    LL_USART_SetParity(USART_RESOURCE, LL_USART_PARITY_NONE);
    LL_USART_SetTransferDirection(USART_RESOURCE, LL_USART_DIRECTION_TX_RX);
    LL_USART_SetHWFlowCtrl(USART_RESOURCE, LL_USART_HWCONTROL_NONE);
    LL_USART_SetOverSampling(USART_RESOURCE, LL_USART_OVERSAMPLING_16);
    LL_USART_DisableFIFO(USART_RESOURCE);
    LL_USART_ConfigAsyncMode(USART_RESOURCE);
    LL_USART_Enable(USART_RESOURCE);

    while ((!(LL_USART_IsActiveFlag_TEACK(USART_RESOURCE))) || (!(LL_USART_IsActiveFlag_REACK(USART_RESOURCE))))
    {
    }
}

void InitGPIO()
{
    nucleo_led.Init();
    nucleo_button.Init();
}
void Transmit(USART_TypeDef *USARTx, uint8_t Value)
{
    while (!LL_USART_IsActiveFlag_TXE(USARTx))
    {
    }

    LL_USART_TransmitData8(USARTx, Value);

    while (!LL_USART_IsActiveFlag_TC(USARTx))
    {
    }
}

int main(void)
{
    const auto USART_RESOURCE = reinterpret_cast<USART_TypeDef *>(USART_RESOURCE_BASE);
    InitGPIO();
    InitUSART();

    while (1)
    {
        if (nucleo_button.IsSet())
        {

            nucleo_led.Set();
        }
        else
        {
            Transmit(USART_RESOURCE, 'S');
            nucleo_led.Reset();
        }
    }
}
/*
//SPDX-License-Identifier: MIT
#include <kw-stm-g0/drivers/pin.h>

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>
#include <stm32g0xx_ll_usart.h>

namespace
{
kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led;
kw::stm::g0::drivers::InPin<GPIOC_BASE, LL_GPIO_PIN_13> nucleo_button;

constexpr auto USART_PORT_BASE = GPIOA_BASE;
constexpr auto USART_RESOURCE_BASE = USART2_BASE;

constexpr auto USART_RESOURCE_CLKSOURCE = LL_RCC_USART2_CLKSOURCE;
constexpr auto USART_RESOURCE_IRQn = USART2_LPUART2_IRQn;
constexpr auto USART_TX_PIN = LL_GPIO_PIN_2;
constexpr auto USART_RX_PIN = LL_GPIO_PIN_3;
constexpr auto USART_SPEED = 115200;

} // namespace

void InitUSART()
{
    
    GPIO_TypeDef *USART_PORT = (GPIO_TypeDef *)(GPIOA_BASE);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetPinMode(USART_PORT, USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinSpeed(USART_PORT, USART_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(USART_PORT, USART_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(USART_PORT, USART_TX_PIN, LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin_0_7(USART_PORT, USART_TX_PIN, LL_GPIO_AF_1);

    LL_GPIO_SetPinMode(USART_PORT, USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinSpeed(USART_PORT, USART_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(USART_PORT, USART_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(USART_PORT, USART_RX_PIN, LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin_0_7(USART_PORT, USART_RX_PIN, LL_GPIO_AF_1);

    LL_USART_Disable(USART_RESOURCE);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
    LL_USART_SetBaudRate(USART_RESOURCE, HSI_VALUE, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, USART_SPEED);
    LL_USART_SetDataWidth(USART_RESOURCE, LL_USART_DATAWIDTH_8B);
    LL_USART_SetStopBitsLength(USART_RESOURCE, LL_USART_STOPBITS_1);
    LL_USART_SetParity(USART_RESOURCE, LL_USART_PARITY_NONE);
    LL_USART_SetTransferDirection(USART_RESOURCE, LL_USART_DIRECTION_TX_RX);
    LL_USART_SetHWFlowCtrl(USART_RESOURCE, LL_USART_HWCONTROL_NONE);
    LL_USART_SetOverSampling(USART_RESOURCE, LL_USART_OVERSAMPLING_16);
    LL_USART_DisableFIFO(USART_RESOURCE);
    LL_USART_ConfigAsyncMode(USART_RESOURCE);
    LL_USART_Enable(USART_RESOURCE);

    while ((!(LL_USART_IsActiveFlag_TEACK(USART_RESOURCE))) || (!(LL_USART_IsActiveFlag_REACK(USART_RESOURCE))))
    {
    }
}

void InitGPIO()
{
    nucleo_led.Init();
    nucleo_button.Init();
}
void Transmit(USART_TypeDef *USARTx, uint8_t Value)
{
    while (!LL_USART_IsActiveFlag_TXE(USARTx))
    {
    }

    LL_USART_TransmitData8(USARTx, Value);

    while (!LL_USART_IsActiveFlag_TC(USARTx))
    {
    }
}

int main(void)
{
    InitGPIO();
    InitUSART();
    while (1)
    {
        if (nucleo_button.IsSet())
        {

            nucleo_led.Set();
        }
        else
        {
            Transmit(USART_RESOURCE, 'S');
            nucleo_led.Reset();
        }
    }
}
*/
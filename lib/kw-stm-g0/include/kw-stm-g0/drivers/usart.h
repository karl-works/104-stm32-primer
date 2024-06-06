//SPDX-License-Identifier: MIT
#pragma once

#include <stm32g0xx.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_system.h>
#include <stm32g0xx_ll_usart.h>

#include <etl\span.h>
#include <etl\string_view.h>
#include <etl\optional.h>

#include <cstdint>
#include <type_traits>

namespace kw::stm::g0::drivers
{

template <uint64_t USART_RESOURCE_N> struct UsartConfig
{
};

template <> struct UsartConfig<USART2_BASE>
{
    static constexpr auto USART_PORT_BASE = GPIOA_BASE;
    static constexpr auto USART_RESOURCE_BASE = USART2_BASE;
    static constexpr auto USART_RESOURCE_CLKSOURCE = LL_RCC_USART2_CLKSOURCE;
    static constexpr auto USART_TX_PIN = LL_GPIO_PIN_2;
    static constexpr auto USART_RX_PIN = LL_GPIO_PIN_3;
    static void Init()
    {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    }
};

template <uint64_t USART_RESOURCE_N, uint64_t USART_SPEED_N>
class Usart 
{
    using CONFIG_T = UsartConfig<USART_RESOURCE_N>;

  public:
    void Init()
    {
        CONFIG_T::Init();
        const auto usartPort = reinterpret_cast<GPIO_TypeDef *>(CONFIG_T::USART_PORT_BASE);
        const auto usartResource = reinterpret_cast<USART_TypeDef *>(CONFIG_T::USART_RESOURCE_BASE);

        LL_USART_InitTypeDef USART_InitStruct = {0};
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

        GPIO_InitStruct.Pin = CONFIG_T::USART_TX_PIN;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
        LL_GPIO_Init(usartPort, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = CONFIG_T::USART_RX_PIN;
        LL_GPIO_Init(usartPort, &GPIO_InitStruct);

        LL_USART_Disable(usartResource);
        USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
        USART_InitStruct.BaudRate = USART_SPEED_N;
        USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
        USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
        USART_InitStruct.Parity = LL_USART_PARITY_NONE;
        USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
        USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
        USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
        LL_USART_Init(usartResource, &USART_InitStruct);
        LL_USART_SetTXFIFOThreshold(usartResource, LL_USART_FIFOTHRESHOLD_1_8);
        LL_USART_SetRXFIFOThreshold(usartResource, LL_USART_FIFOTHRESHOLD_1_8);
        LL_USART_DisableFIFO(usartResource);
        LL_USART_ConfigAsyncMode(usartResource);
        LL_USART_Enable(usartResource);

        while ((!(LL_USART_IsActiveFlag_TEACK(usartResource))) ||
               (!(LL_USART_IsActiveFlag_REACK(usartResource))))
        {
        }
    }

    template <class T>
    requires(std::is_same<T, etl::span<uint8_t>>::value || std::is_same<T, etl::u8string_view>::value)
    void Transmit(const T transmit_data)
    {
        const auto usartResource = reinterpret_cast<USART_TypeDef *>(CONFIG_T::USART_RESOURCE_BASE);
        for (auto data : transmit_data)
        {

            while (!LL_USART_IsActiveFlag_TXE(usartResource))
            {
            }

            LL_USART_TransmitData8(usartResource, data);

            while (!LL_USART_IsActiveFlag_TC(usartResource))
            {
            }
        }
    }
    /*
    void ISRReceive(etl::span<uint8_t>)
    {
        
        const auto usartResource = reinterpret_cast<USART_TypeDef *>(CONFIG_T::USART_RESOURCE_BASE);
        if(LL_USART_IsActiveFlag_RXNE(usartResource))
        {
          return LL_USART_ReceiveData8(usartResource);
        }
        return etl::optional<uint8_t>();
        
    }
    */
};
} // namespace kw::drivers::stm
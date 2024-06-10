//SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include <stm32g0xx.h>
#include <stm32g0xx_ll_exti.h>

namespace kw::stm::g0::drivers
{

enum PinExtiTrigger
{
    NONE = 0,
    RISING = 1,
    FALLING = 2,
    ALL = 4
};

template <uint64_t EXTI_CONFIG_PORT_N, uint64_t EXTI_LINE_N, uint64_t EXTI_MODE_N, uint64_t EXTI_TRIGGER_N>
struct PinExti
{
    static_assert(EXTI_LINE_N >= LL_EXTI_LINE_0 && EXTI_LINE_N <= LL_EXTI_LINE_15, "Invalid EXTI_LINE_N selected");

    static consteval uint64_t ConfigLine()
    {

        uint64_t lines[] = {
            LL_EXTI_LINE_0,  LL_EXTI_LINE_1,  LL_EXTI_LINE_2,  LL_EXTI_LINE_3,  LL_EXTI_LINE_4,  LL_EXTI_LINE_5,
            LL_EXTI_LINE_6,  LL_EXTI_LINE_7,  LL_EXTI_LINE_8,  LL_EXTI_LINE_9,  LL_EXTI_LINE_10, LL_EXTI_LINE_11,
            LL_EXTI_LINE_12, LL_EXTI_LINE_13, LL_EXTI_LINE_14, LL_EXTI_LINE_15,

        };

        uint64_t config_lines[] = {
            LL_EXTI_CONFIG_LINE0,  LL_EXTI_CONFIG_LINE1,  LL_EXTI_CONFIG_LINE2,  LL_EXTI_CONFIG_LINE3,
            LL_EXTI_CONFIG_LINE4,  LL_EXTI_CONFIG_LINE5,  LL_EXTI_CONFIG_LINE6,  LL_EXTI_CONFIG_LINE7,
            LL_EXTI_CONFIG_LINE8,  LL_EXTI_CONFIG_LINE9,  LL_EXTI_CONFIG_LINE10, LL_EXTI_CONFIG_LINE11,
            LL_EXTI_CONFIG_LINE12, LL_EXTI_CONFIG_LINE13, LL_EXTI_CONFIG_LINE14, LL_EXTI_CONFIG_LINE15,

        };
        static_assert(sizeof(lines) == sizeof(config_lines), "sizeof(lines) == sizeof(config_lines)");
        for (int index = 0; index < 16; index++)
        {
            if (EXTI_LINE_N == lines[index])
            {
                return config_lines[index];
            }
        }
    }

    static consteval IRQn_Type IRQ()
    {
        if constexpr (EXTI_LINE_N == LL_EXTI_LINE_0 || EXTI_LINE_N <= LL_EXTI_LINE_1)
        {
            return IRQn_Type::EXTI0_1_IRQn;
        }
        else if constexpr (EXTI_LINE_N == LL_EXTI_LINE_2 || EXTI_LINE_N <= LL_EXTI_LINE_3)
        {
            return IRQn_Type::EXTI2_3_IRQn;
        }
        else if constexpr (EXTI_LINE_N > LL_EXTI_LINE_3 && EXTI_LINE_N < LL_EXTI_LINE_16)
        {
            return IRQn_Type::EXTI4_15_IRQn;
        }
        else
        {
            static_assert(false, "Invalid EXTI_LINE_N selected");
        }
    }

    void Init()
    {
        LL_EXTI_SetEXTISource(EXTI_CONFIG_PORT_N, ConfigLine());
        LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
        EXTI_InitStruct.Line_0_31 = EXTI_LINE_N;
        EXTI_InitStruct.LineCommand = ENABLE;
        EXTI_InitStruct.Mode = EXTI_MODE_N;
        EXTI_InitStruct.Trigger = EXTI_TRIGGER_N;
        LL_EXTI_Init(&EXTI_InitStruct);
    }

    PinExtiTrigger IRS()
    {

        if constexpr (EXTI_TRIGGER_N == LL_EXTI_TRIGGER_RISING)
        {
            if (LL_EXTI_IsActiveRisingFlag_0_31(EXTI_LINE_N) != RESET)
            {
                LL_EXTI_ClearRisingFlag_0_31(EXTI_LINE_N);
                return PinExtiTrigger::RISING;
            }
        }
        else if constexpr (EXTI_TRIGGER_N == LL_EXTI_TRIGGER_FALLING)
        {
            if (LL_EXTI_IsActiveFallingFlag_0_31(EXTI_LINE_N) != RESET)
            {
                LL_EXTI_ClearFallingFlag_0_31(EXTI_LINE_N);
                return PinExtiTrigger::FALLING;
            }
        }
        else if constexpr (EXTI_TRIGGER_N == LL_EXTI_TRIGGER_RISING_FALLING)
        {
            PinExtiTrigger value = PinExtiTrigger::NONE;
            if (LL_EXTI_IsActiveRisingFlag_0_31(EXTI_LINE_N) != RESET)
            {
                LL_EXTI_ClearRisingFlag_0_31(EXTI_LINE_N);
                value = PinExtiTrigger::RISING;
            }

            if (LL_EXTI_IsActiveFallingFlag_0_31(EXTI_LINE_N) != RESET)
            {
                LL_EXTI_ClearFallingFlag_0_31(EXTI_LINE_N);
                if (value == PinExtiTrigger::RISING)
                {
                    return PinExtiTrigger::ALL;
                }
                else
                {
                    return PinExtiTrigger::FALLING;
                }
            }
        }

        return PinExtiTrigger::NONE;
    }
    void Enable()
    {

        NVIC_EnableIRQ(IRQ());
    }
    void Disable()
    {
        NVIC_DisableIRQ(IRQ());
    }

    void SetPriority(uint32_t priority)
    {
        NVIC_SetPriority(IRQ(), priority);
    }
};
} // namespace kw::stm::g0::drivers
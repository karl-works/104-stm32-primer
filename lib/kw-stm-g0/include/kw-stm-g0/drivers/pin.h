//SPDX-License-Identifier: MIT 
#pragma once 
#include <cstdint> 
#include <stm32g0xx.h> 
#include <stm32g0xx_ll_bus.h> 
#include <stm32g0xx_ll_gpio.h> 
 
namespace kw::stm::g0::drivers 
{ 
 
template <uint64_t GPIOX_BASE_N> struct PinConfig 
{ 
}; 
template <> struct PinConfig<GPIOB_BASE> 
{ 
    static constexpr auto LL_IOP_GRP1_PERIPH_GROUP = LL_IOP_GRP1_PERIPH_GPIOB; 
};
template <> struct PinConfig<GPIOA_BASE> 
{ 
    static constexpr auto LL_IOP_GRP1_PERIPH_GROUP = LL_IOP_GRP1_PERIPH_GPIOA; 
}; 
 
 
template <	uint64_t GPIOX_BASE_N, 
			uint64_t GPIO_LL_EC_PIN_N, 
			uint64_t GPIO_LL_EC_MODE_N, 
			uint64_t GPIO_LL_EC_SPEED_N,  
			uint64_t GPIO_LL_EC_OUTPUT_N, 
			uint64_t GPIO_LL_EC_PULL_N> 
struct Pin 
{ 
 
    using CONFIG_T = PinConfig<GPIOX_BASE_N>; 
    void Init() 
    { 
        LL_IOP_GRP1_EnableClock(CONFIG_T::LL_IOP_GRP1_PERIPH_GROUP); 
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0}; 
        GPIO_InitStruct.Pin = GPIO_LL_EC_PIN_N; 
        GPIO_InitStruct.Mode = GPIO_LL_EC_MODE_N; 
        GPIO_InitStruct.Speed = GPIO_LL_EC_SPEED_N; 
        GPIO_InitStruct.OutputType = GPIO_LL_EC_OUTPUT_N; 
        GPIO_InitStruct.Pull = GPIO_LL_EC_PULL_N; 
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0; 
        LL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(GPIOX_BASE_N), &GPIO_InitStruct); 
    } 
    void Set() 
    { 
        LL_GPIO_SetOutputPin(reinterpret_cast<GPIO_TypeDef *>(GPIOX_BASE_N), GPIO_LL_EC_PIN_N); 
    } 
    void Reset() 
    { 
        LL_GPIO_ResetOutputPin(reinterpret_cast<GPIO_TypeDef *>(GPIOX_BASE_N), GPIO_LL_EC_PIN_N); 
    } 
    void Toggle() 
    { 
        LL_GPIO_TogglePin(reinterpret_cast<GPIO_TypeDef *>(GPIOX_BASE_N), GPIO_LL_EC_PIN_N); 
    } 
    bool Value() 
    { 
        return LL_GPIO_IsInputPinSet(reinterpret_cast<GPIO_TypeDef *>(GPIOX_BASE_N), GPIO_LL_EC_PIN_N); 
    } 
}; 
 
template <uint64_t GPIOX_BASE_N, uint64_t GPIO_LL_EC_PIN_N> 
using OutPin = Pin<	GPIOX_BASE_N, 
					GPIO_LL_EC_PIN_N, 
					LL_GPIO_MODE_OUTPUT, 
					LL_GPIO_SPEED_FREQ_LOW, 
					LL_GPIO_OUTPUT_PUSHPULL, 
					LL_GPIO_PULL_NO>; 
 
 
} // namespace kw::stm::g0::drivers
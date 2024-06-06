//SPDX-License-Identifier: MIT 
 
#include <stm32g0xx.h> 
#include <stm32g0xx_ll_bus.h> 
#include <stm32g0xx_ll_gpio.h> 
#include <stm32g0xx_ll_rcc.h> 
 
constexpr auto LED_PORT = GPIOA_BASE; 
constexpr auto LED_PIN = LL_GPIO_PIN_5; 
 
extern "C" 
{ 
    void SysTick_Handler(void) 
    { 
        LL_GPIO_TogglePin(reinterpret_cast<GPIO_TypeDef *>(LED_PORT), LED_PIN); 
    } 
} 
 
void InitGPIO() 
{ 
 
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); 
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0}; 
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5; 
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; 
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW; 
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; 
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO; 
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0; 
    LL_GPIO_Init(reinterpret_cast<GPIO_TypeDef *>(GPIOA_BASE), &GPIO_InitStruct); 
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

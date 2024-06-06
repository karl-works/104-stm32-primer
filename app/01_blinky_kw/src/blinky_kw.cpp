//SPDX-License-Identifier: MIT 
 
#include "kw-stm-g0/drivers/pin.h" 
 
#include <stm32g0xx.h> 
#include <stm32g0xx_ll_bus.h> 
#include <stm32g0xx_ll_gpio.h> 
#include <stm32g0xx_ll_rcc.h> 
#include <stm32g0xx_ll_system.h> 
 
namespace 
{ 
 
kw::stm::g0::drivers::OutPin<GPIOA_BASE, LL_GPIO_PIN_5> nucleo_led; 
 
} // namespace 
 
extern "C" 
{ 
    void SysTick_Handler(void) 
    { 
        nucleo_led.Toggle(); 
    } 
} 
 
void InitGPIO() 
{ 
    nucleo_led.Init(); 
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

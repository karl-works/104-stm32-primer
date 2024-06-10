cmake_minimum_required(VERSION 3.28) 
 
project(kw-stm-g0 CXX) 
add_library(${PROJECT_NAME} INTERFACE) 
add_library(kw::stm::g0 ALIAS ${PROJECT_NAME}) 
 
set(PUBLIC_HEADERS  
  include/kw-stm-g0/drivers/pin.h 
  include/kw-stm-g0/drivers/pin_exti.h 
  
) 
 
target_sources( 
  ${PROJECT_NAME} 
  PUBLIC FILE_SET 
         public_headers 
         TYPE 
         HEADERS 
         BASE_DIRS 
         include 
         FILES 
         ${PUBLIC_HEADERS} 
) 
 
target_compile_options(${PROJECT_NAME} INTERFACE -fno-exceptions) 
target_compile_definitions(${PROJECT_NAME} INTERFACE USE_FULL_LL_DRIVER) 
 
target_link_libraries( 
  ${PROJECT_NAME} INTERFACE 
    CMSIS::STM32::G0B1RE 
    HAL::STM32::G0::LL_RCC  
    HAL::STM32::G0::LL_GPIO  
    HAL::STM32::G0::LL_USART 
    etl
)
#SPDX-License-Identifier: MIT 
cmake_minimum_required(VERSION 3.28) 
 
project(01_blinky_kw C CXX ASM) 
 
add_executable(${PROJECT_NAME} src/blinky_kw.cpp) 
target_compile_options(${PROJECT_NAME} PRIVATE -fno-exceptions) 
target_link_options(${PROJECT_NAME} PUBLIC "LINKER:--no-warn-rwx-segments,LINKER:--print-memory-usage") 
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_FULL_LL_DRIVER) 
 
target_link_libraries( 
  ${PROJECT_NAME}  
  CMSIS::STM32::G0B1RE 
  HAL::STM32::G0::LL_RCC  
  HAL::STM32::G0::LL_GPIO  
  kw::stm::g0
) 
 
stm32_print_size_of_target(${PROJECT_NAME}) 

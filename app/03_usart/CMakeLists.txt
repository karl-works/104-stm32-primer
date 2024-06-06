#SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.28)
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

project(04_usart C CXX ASM)

add_executable(${PROJECT_NAME} src/usart.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -fno-exceptions)
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_FULL_LL_DRIVER)
target_link_options(${PROJECT_NAME} PUBLIC "LINKER:--no-warn-rwx-segments,LINKER:--print-memory-usage")

target_link_libraries(
  ${PROJECT_NAME} 
  CMSIS::STM32::G0B1RE
  HAL::STM32::G0::LL_RCC 
  HAL::STM32::G0::LL_GPIO 
  HAL::STM32::G0::LL_USART
  kw::stm::g0
)

stm32_print_size_of_target(${PROJECT_NAME})

#SPDX-License-Identifier: MIT  
  
cmake_minimum_required(VERSION 3.28)  
 
  
file(TO_CMAKE_PATH "${CMAKE_CURRENT_LIST_FILE}/CMakeLists.txt" LOC_PATH)  
if(EXISTS "${LOC_PATH}")  
  message(  
    FATAL_ERROR  
      "You cannot build in a source directory"   
  )  
endif()  
  
include(FetchContent)  
  
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  
set(CMAKE_CXX_STANDARD 20)  
  
FetchContent_Declare(  
  stm32_cmake  
  GIT_REPOSITORY https://github.com/ObKo/stm32-cmake  
  GIT_TAG master  
)  
  
FetchContent_MakeAvailable(stm32_cmake)  
  
set(STM32_TOOLCHAIN_PATH  
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/arm-gnu-toolchain-13.2  
    CACHE STRING "Path to the ARM toolchain"  
)  
set(STM32_TARGET_TRIPLET  
    "arm-none-eabi"  
    CACHE STRING "Target triplet for the ARM toolchain"  
)  
set(STM32_CUBE_G0_PATH  
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/STM32CubeG0  
    CACHE STRING "Path to the STM32CubeG0 repository"  
)  
  
set(CMAKE_TOOLCHAIN_FILE ${stm32_cmake_SOURCE_DIR}/cmake/stm32_gcc.cmake)  
 
FetchContent_Declare(
  etl
  GIT_REPOSITORY https://github.com/ETLCPP/etl.git
  GIT_TAG 20.38.13
)

FetchContent_MakeAvailable(etl)

project(kw-stm-primer ASM C CXX)  
find_package(  
  CMSIS  
  COMPONENTS STM32G0  
  REQUIRED  
)  
  
find_package(  
  HAL  
  COMPONENTS STM32G0 LL_RCC LL_GPIO LL_USART LL_EXTI
  REQUIRED  
)  
 
add_subdirectory(app/01_blinky)
add_subdirectory(app/01_blinky_kw)
add_subdirectory(app/01_multi-function-shield)

add_subdirectory(app/02_button)
add_subdirectory(app/02_button_it)
add_subdirectory(app/02_button_kw)
add_subdirectory(app/02_multi-function-shield)

add_subdirectory(app/03_usart)
add_subdirectory(app/03_usart_kw)

add_subdirectory(app/04_bitbang_spi_mfc)

add_subdirectory(lib/kw-stm-g0)
add_subdirectory(lib/kw)

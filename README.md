# Intro

## Environment

This repository was developed on Windows, and efforts have been made to ensure it is OS-independent. Larger scripts are written in Python, while smaller scripts have both PowerShell and Bash variants. Support for non-Windows environments is provided on a best-effort basis.

## Building
```
cd extern
pip install -r requirements.txt
python fetch.py
cd ..
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

## Fetching

Some requirements are retrieved using FetchContent, while others need to be manually downloaded to the externals folder. There is a fetch.py script in the externals folder that may or may not work, as it has only been tested on Windows.

## System Requirements

- cmake
    - url: https://cmake.org/download/
    - version: >= 3.29.2
    
- Ninja
    - url: https://ninja-build.org/
    - version: >= v1.12.0

- Python
    - url: https://www.python.org/
    - version: >= 3.11.0
    
## FetchContent Packages

- stm32cmake
    - url: https://github.com/ObKo/stm32-cmake
    - version: master

## Externals

- STM32CubeG0 MCU Firmware Package
    - url: https://github.com/STMicroelectronics/STM32CubeG0.git
    - version: >=v1.6.2

- Arm GNU Toolchain
    - url: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads/13-2-rel1
    - version: 13.2

- OpenOCD, the Open On-Chip Debugger
    - url: https://github.com/openocd-org/openocd/
    - version: >=v0.12.0

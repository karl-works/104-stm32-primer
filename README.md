# Intro

## Environment

This repository was developed on Windows, and efforts have been made to ensure it is OS-independent. Larger scripts are written in Python, while smaller scripts in PowerShell. Support for non-Windows environment is provided on a best-effort basis.

## Directory Structure

The app folder contains the applications that can be built and deployed to the STM32G0, they are prepended with `NN_` where NN is a number that corresponds to the episode number. 

The lib folder contains the libraries that are used by the applications, they are not tied to any specific episode.

## Development Process

The development process is documented in the form of a video series called the STM Primer. The series is available on YouTube and is intended to be a companion to this repository. The videos are released on a best-effort basis and are intended to be a guide to the development process.

[Semver](https://semver.org/) is used to version the repository, with the following format: `MAJOR.MINOR.PATCH`. The `MAJOR` version is incremented when breaking changes are made, the `MINOR` version is incremented when new features are added, and the `PATCH` version is incremented when bug fixes are made. The Semver number is not tied to the episode number, but to the features and bug fixes in the repository.

Please note that there may be changes in later episodes that nessesitate changes in earlier episodes code. These changes will be documented in the form of a patch release. New episodes will be released as minor releases but this is not a hard rule.

### Episode Numbering Scheme

|Number|Status|Video|
|---|---|---|
|01|Released|[STM Primer - S01E01 - Adventures in the Art of Blinking](https://youtu.be/0vRnZDV55ms)|
|02|Recording|STM Primer - S01E02 - The Subtlie Art of Button Pressing|
|03|Recording|STM Primer - S01E03 - On the USART no one can hear your send|
|04|Development|STM Primer - S01E04 - Banging on Heavens door|

  
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

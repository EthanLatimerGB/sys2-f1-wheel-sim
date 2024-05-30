cmake_minimum_required(VERSION 3.12)

# Importing the Pico SDK and optional extras

include(pico_sdk_import.cmake)
#include(pico_extras_import_optional.cmake)

# Project name

project(F1Wheel C CXX ASM)

# Setting the C standard

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Checks

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialising the Pico SDK

pico_sdk_init()

# All src files

add_executable(${PROJECT_NAME}
	src/blink.c
	)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

target_link_libraries(${PROJECT_NAME} 
	pico_stdlib 
    	pico_cyw43_arch_none
    	)

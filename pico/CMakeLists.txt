# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/elatimer/Dev/Pico/pico-sdk")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")
set(CMAKE_BUILD_TYPE Debug)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(F1Wheel C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# This is where our configuration file for the Grpahics Library LVGL lives
add_compile_definitions(LV_CONF_PATH=\"${CMAKE_SOURCE_DIR}/src/lv_conf.h\")

# Compile source for Pico LCD Display libraries
add_subdirectory(lib/Config)
add_subdirectory(lib/Infrared)
add_subdirectory(lib/OLED)
add_subdirectory(lib/Icm20948)
add_subdirectory(lib/LCD)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
add_subdirectory(lvgl)

# Include headers from Pico LCD Display libraries
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/Icm20948)
include_directories(./lib/LCD)
include_directories(./lib/Infrared)
include_directories(./lib/OLED)
include_directories(./lvgl)

# Add executable. Default name is the project name, version 0.1
add_executable(F1Wheel 
  src/main.c 
  src/display.c
  src/communication.c)

pico_set_program_name(F1Wheel "F1Wheel")
pico_set_program_version(F1Wheel "0.1")

pico_enable_stdio_usb(F1Wheel 1)
pico_enable_stdio_uart(F1Wheel 0)


# Add the standard library to the build
target_link_libraries(F1Wheel
        pico_stdlib
        hardware_spi
        hardware_dma
        hardware_i2c
        hardware_pwm
)

# Add the standard include files to the build
target_include_directories(F1Wheel PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(F1Wheel 
        pico_cyw43_arch_none
        LCD OLED Infrared Icm20948 GUI Fonts Config
        lvgl
        )

pico_add_extra_outputs(F1Wheel)


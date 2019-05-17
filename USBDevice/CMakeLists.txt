cmake_minimum_required(VERSION 3.5)

project(mbed_usb)

############################################################
# Create a library
############################################################

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR} CACHE STRING "Project root directory")

set(CMAKE_MODULE_PATH "${PROJECT_ROOT_DIR}")

set(MBED_LOCATION "/home/alax/git/mbed-os/BUILD/mbed")


find_package(mbed REQUIRED)

 

add_definitions(
    "-DTARGET_STM32F4 -DTARGET_STM32F411RE"
#    "-DTARGET_STM32F4 -DTARGET_STM32F411RE -DDEBUG"
)


#message(${MBED_FOUND})
#message(${MBED_INCLUDE_DIRS})


#Generate the static library from the library sources
add_library(mbed_usb STATIC
    targets/TARGET_STM/USBHAL_STM32.cpp
    targets/TARGET_STM/USBHAL_STM32F4.cpp
    USBMIDI/USBMIDI.cpp
    USBDevice/USBDevice.cpp
    targets/TARGET_STM/USBHAL_STM32F4.cpp
)

target_include_directories(mbed_usb
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/USBMIDI
        ${PROJECT_SOURCE_DIR}/USBDevice
        ${PROJECT_SOURCE_DIR}/targets/TARGET_STM
        ${PROJECT_SOURCE_DIR}/targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F411xE/TARGET_NUCLEO_F411RE

)

target_link_libraries(mbed_usb
        mbed::mbed
)
#find_package(mbed REQUIRED)

#add_executable(blink_NUCLEO_L411RE main.cpp)
#target_compile_options(blink_NUCLEO_L411RE
#	PRIVATE
#		-fno-exceptions -fno-rtti --include ${CMAKE_SOURCE_DIR}/mbed_config.h
#)

#target_link_libraries(blink_NUCLEO_L411RE mbed::mbed)

#enable_binary_generation(blink_NUCLEO_L411RE)
#add_flash_target(blink_NUCLEO_L411RE)



############################################################
# Create an executable
############################################################

# Add an executable with the above sources
#add_executable(hello_binary 
#    USBMIDI/USBMIDI.cpp
#)

# link the new mbed_usb target with the hello_binary target
#target_link_libraries( hello_binary
#    PRIVATE 
#        mbed_usb
#)

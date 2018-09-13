cmake_minimum_required(VERSION 3.6)
project(nucleo_f411re VERSION "0.1")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(USBDevice)
add_subdirectory(src bin)

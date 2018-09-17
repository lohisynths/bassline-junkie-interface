cmake_minimum_required(VERSION 3.6)
project(nucleo_f411re VERSION "0.1")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#set(CMAKE_CXX_CPPCHECK "cppcheck")
#find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK
            #"--enable=warning"
            "--inconclusive"
            "--force"
            "--inline-suppr"
           # "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
endif()

add_subdirectory(USBDevice)
add_subdirectory(src bin)

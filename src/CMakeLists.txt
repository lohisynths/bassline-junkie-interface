find_package(mbed REQUIRED)

add_executable(blink_NUCLEO_L411RE
	main.cpp
	Knob.cpp 
	Pwm.cpp
	Button.cpp
	Enc.cpp
	PCA9685.cpp
	Mux.cpp
	MIDI.cpp
)

target_compile_options(blink_NUCLEO_L411RE
	PRIVATE
		-Wall -fno-exceptions -fno-rtti --include ${CMAKE_SOURCE_DIR}/mbed_config.h
)

target_include_directories(blink_NUCLEO_L411RE
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(blink_NUCLEO_L411RE PRIVATE cxx_std_11)

set(CMAKE_C_FLAGS "-include ${PROJECT_SOURCE_DIR}/mbed_config.h")

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32F411XE.ld")


target_link_libraries(blink_NUCLEO_L411RE mbed::mbed mbed_usb)

enable_binary_generation(blink_NUCLEO_L411RE)
add_flash_target(blink_NUCLEO_L411RE)

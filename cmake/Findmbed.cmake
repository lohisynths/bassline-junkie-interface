link_directories("${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TOOLCHAIN_GCC_ARM/")

file(GLOB hal "${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TOOLCHAIN_GCC_ARM/*.o")

add_library(mbed_nucleo_l432kc INTERFACE)
add_library(mbed::mbed ALIAS mbed_nucleo_l432kc)

target_sources(mbed_nucleo_l432kc INTERFACE ${hal})
target_include_directories(mbed_nucleo_l432kc
	 INTERFACE
		"${MBED_LOCATION}"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F411xE/"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM/TARGET_STM32F4/"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F411xE/device/"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM/TARGET_STM32F4/device"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM"
		"${MBED_LOCATION}/TARGET_NUCLEO_F411RE/TARGET_STM/TARGET_STM32F4/TARGET_STM32F411xE/TARGET_NUCLEO_F411RE/"
		"${MBED_LOCATION}/platform"
		"${MBED_LOCATION}/hal"
)
target_link_libraries(mbed_nucleo_l432kc
	INTERFACE
		mbed
)

function(enable_binary_generation target)
	add_custom_command(TARGET ${target}
	POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O binary "${CMAKE_CURRENT_BINARY_DIR}/${target}" "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
	BYPRODUCTS "${target}.bin"
	)
endfunction()

function(add_flash_target target)
	if(MBED_CONTROLLER_DIR)
			add_custom_target(${target}_flash)
			add_custom_command(TARGET ${target}_flash
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin" "${MBED_CONTROLLER_DIR}/${target}.bin")
			add_dependencies(${target}_flash ${target})
	endif()
endfunction()

function(rap target)
#set_target_properties(${target} PROPERTIES SUFFIX ".elf")
add_custom_command(TARGET ${target}
	POST_BUILD
	COMMAND ${CMAKE_SIZE} --format=berkeley ${target}
	COMMENT "Invoking: Cross ARM GNU Print Size"
    )
endfunction()


cmake_minimum_required(VERSION 3.10)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mico_ros C CXX)

pico_sdk_init()

add_executable(temperature_node
  src/temperature_node.cpp
  src/pico_ros.c
)
target_link_libraries(temperature_node
  hardware_adc
  pico_stdlib
  ${PROJECT_SOURCE_DIR}/microros/libmicroros.a
)
target_include_directories(temperature_node PUBLIC
  microros/include
  include
)
set_target_properties(temperature_node PROPERTIES
  C_STANDARD 11
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

# Configure Pico
pico_enable_stdio_usb(temperature_node 1)
pico_enable_stdio_uart(temperature_node 0)

target_compile_definitions(temperature_node
  PUBLIC PICO_UART_ENABLE_CRLF_SUPPORT=0
  PUBLIC PICO_STDIO_ENABLE_CRLF_SUPPORT=0
  PUBLIC PICO_STDIO_DEFAULT_CRLF=0
)

# Generate UF2
pico_add_extra_outputs(temperature_node)

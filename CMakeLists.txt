cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mico_ros C CXX ASM)

pico_sdk_init()

######################
## Temperature node ##
######################

add_executable(temperature_node
  src/temperature_node.cpp
  src/pico_uart_transport.c
)
target_include_directories(temperature_node PUBLIC
  include
  SYSTEM libmicroros/include
)
target_link_directories(temperature_node PRIVATE libmicroros)
target_link_libraries(temperature_node
  hardware_adc
  pico_stdlib
  microros
)
set_target_properties(temperature_node PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS YES
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

################
## Range node ##
################

add_executable(range_node
  src/range_node.cpp
  src/pico_uart_transport.c
)
target_include_directories(range_node PRIVATE
  include
  SYSTEM libmicroros/include
)
target_link_directories(range_node PRIVATE libmicroros)
target_link_libraries(range_node PRIVATE
  pico_stdlib
  microros
)
set_target_properties(range_node PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS YES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

# Configure Pico
pico_enable_stdio_usb(range_node 1)
pico_enable_stdio_uart(range_node 0)

target_compile_definitions(range_node
  PUBLIC PICO_UART_ENABLE_CRLF_SUPPORT=0
  PUBLIC PICO_STDIO_ENABLE_CRLF_SUPPORT=0
  PUBLIC PICO_STDIO_DEFAULT_CRLF=0
)

# Generate UF2
pico_add_extra_outputs(range_node)

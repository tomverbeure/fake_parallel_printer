add_executable(parallel2usb)

# generate the header file into the source tree as it is included in the RP2040 datasheet
#pico_generate_pio_header(parallel2usb OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_sources(parallel2usb PRIVATE main.cc 
    )

target_link_libraries(parallel2usb PRIVATE pico_stdlib)
pico_add_extra_outputs(parallel2usb)
pico_enable_stdio_usb(parallel2usb 1)
pico_enable_stdio_uart(parallel2usb 0)

# add url via pico_set_program_url
example_auto_set_url(parallel2usb)


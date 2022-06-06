cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD "pico")

include(pico_sdk_import.cmake)

project(usb-midiflower)

pico_sdk_init()


file(GLOB_RECURSE FILES src/*.c src/*.cpp src/*.h
  
)


message(STATUS "project files : ${FILES}")

add_executable(usb-midiflower
  ${FILES}
  )

target_include_directories(usb-midiflower PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/HAL
    ${CMAKE_CURRENT_LIST_DIR}/src/BOARD
    ${CMAKE_CURRENT_LIST_DIR}/src/HARD
  )



target_link_libraries(usb-midiflower PRIVATE
  pico_stdlib
  hardware_pwm
  tinyusb_device
  tinyusb_board
  )

pico_enable_stdio_usb(usb-midiflower 1)
pico_enable_stdio_uart(usb-midiflower 1)

pico_add_extra_outputs(usb-midiflower)

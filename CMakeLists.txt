cmake_minimum_required(VERSION 3.12)



set(PICO_BOARD "pico")

include(pico_sdk_import.cmake)


###################
# build makefsdata
###################
include(ExternalProject)
set(MAKEFSDATA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lwip-2.1.3/src/apps/http/makefsdata)

set(MAKEFSDATA_BINARY_DIR ${CMAKE_BINARY_DIR}/makefsdata)
set(MAKEFSDATA_BUILD_TARGET MakeFsDataBuild)

if (NOT TARGET ${MAKEFSDATA_BUILD_TARGET})
        pico_message_debug("makefsdata will need to be built")
        ExternalProject_Add(${MAKEFSDATA_BUILD_TARGET}
                PREFIX makefsdata SOURCE_DIR ${MAKEFSDATA_SOURCE_DIR}
                BINARY_DIR ${MAKEFSDATA_BINARY_DIR}
                BUILD_ALWAYS 1 # force dependency checking
                INSTALL_COMMAND ""
                )
    endif()

message(STATUS "***********************************************")
message(STATUS "MAKEFSDATA_BUILD_TARGET: ${MAKEFSDATA_BUILD_TARGET}")
message(STATUS "MAKEFSDATA_BINARY_DIR: ${MAKEFSDATA_BINARY_DIR}")
message(STATUS "***********************************************")

add_custom_target (www 
  
    COMMAND  ${MAKEFSDATA_BINARY_DIR}/makefsdata.exe ${CMAKE_CURRENT_SOURCE_DIR}/www/fs -svr:MidiFlower -f:${CMAKE_CURRENT_SOURCE_DIR}/www/fsdata.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   
)


project(usb-midiflower)



pico_sdk_init()


file(GLOB_RECURSE FILES src/*.c src/*.cpp src/*.h
  
)

message(STATUS "***********************************************")
message(STATUS "project files : ${FILES}")
message(STATUS "***********************************************")

# LWIP
set(LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lwip-2.1.3)
set (LWIP_INCLUDE_DIRS
    "${LWIP_DIR}/src/include"
    "${LWIP_DIR}/src/include/lwip/apps"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/www"
)

include(${LWIP_DIR}/src/Filelists.cmake)



# Extra stuff from TinyUSB, that is not part of tinyusb_device library
set(TINYUSB_LIBNETWORKING_SOURCES
    ${PICO_TINYUSB_PATH}/lib/networking/dhserver.c
    ${PICO_TINYUSB_PATH}/lib/networking/dnserver.c
    ${PICO_TINYUSB_PATH}/lib/networking/rndis_reports.c
)

add_executable(usb-midiflower
  ${FILES}
  ${TINYUSB_LIBNETWORKING_SOURCES}
  )

target_include_directories(usb-midiflower PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/HAL
    ${CMAKE_CURRENT_LIST_DIR}/src/BOARD
    ${CMAKE_CURRENT_LIST_DIR}/src/HARD
    ${CMAKE_CURRENT_LIST_DIR}/src/MIDI
    ${CMAKE_CURRENT_LIST_DIR}/src/TOOLS
    ${LWIP_INCLUDE_DIRS} 
    ${PICO_TINYUSB_PATH}/src 
    ${PICO_TINYUSB_PATH}/lib/networking
  )



target_link_libraries(usb-midiflower PRIVATE
  pico_stdlib
  hardware_pwm
  tinyusb_device
  tinyusb_board
  lwipallapps 
  lwipcore
  )

  target_link_options(usb-midiflower PRIVATE 
  -Xlinker -print-memory-usage
  )
pico_enable_stdio_usb(usb-midiflower 0)
pico_enable_stdio_uart(usb-midiflower 0)

pico_add_extra_outputs(usb-midiflower)

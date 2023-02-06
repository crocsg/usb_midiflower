cmake_minimum_required(VERSION 3.12)


set (PROJECT_NAME "usb-midiflower")
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


file(GLOB_RECURSE FILES 
  src/*.c 
  src/*.cpp 
  src/*.h
  src/HTTP/*.cpp
  
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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/HTTP"
)

include(${LWIP_DIR}/src/Filelists.cmake)



# Extra stuff from TinyUSB, that is not part of tinyusb_device library
set(TINYUSB_LIBNETWORKING_SOURCES
    ${PICO_TINYUSB_PATH}/lib/networking/dhserver.c
    ${PICO_TINYUSB_PATH}/lib/networking/dnserver.c
    ${PICO_TINYUSB_PATH}/lib/networking/rndis_reports.c
)

add_executable(${PROJECT_NAME} 
  ${FILES}
  ${TINYUSB_LIBNETWORKING_SOURCES}
  )

pico_generate_pio_header(${PROJECT_NAME}  
${CMAKE_CURRENT_LIST_DIR}/src/HARD/WS2812.pio
)

target_include_directories(${PROJECT_NAME}  PRIVATE
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



target_link_libraries(${PROJECT_NAME}  PRIVATE
  pico_stdlib
  hardware_pwm
  hardware_dma
  hardware_pio
  pico_multicore
  tinyusb_device
  tinyusb_board
  lwipallapps 
  lwipcore

  )

  target_link_options(${PROJECT_NAME}  PRIVATE 
  -Xlinker -print-memory-usage
  )
pico_enable_stdio_usb(${PROJECT_NAME}  0)
pico_enable_stdio_uart(${PROJECT_NAME}  1)

pico_set_linker_script(${PROJECT_NAME}  ${CMAKE_SOURCE_DIR}/memmap_usb_midiflower.ld)

pico_add_extra_outputs(${PROJECT_NAME} )

message (STATUS, ${PICO_TARGET_LINKER_SCRIPT})
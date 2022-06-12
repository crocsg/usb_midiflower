cmake_minimum_required(VERSION 3.12)
project(makefsdata)

#gcc -o makefsdata -DMAKEFS_SUPPORT_DEFLATE=1 -DMAKEFS_SUPPORT_DEFLATE_ZLIB=1 -Ilib/lwip/src/include -I/usr/include/zopfli -Isrc lib/lwip/src/apps/http/makefsdata/makefsdata.c -lz

set(CMAKE_CXX_STANDARD 14)



add_executable(makefsdata makefsdata.c)
target_include_directories(makefsdata PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../../../include
    ../../../../../../src
)
target_link_libraries(makefsdata)
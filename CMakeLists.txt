cmake_minimum_required(VERSION 3.30)
project(infocom)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/coder)

set(CODER_SOURCES src/coder/coder.cpp)

add_library(CODER ${CODER_SOURCES})
target_include_directories(CODER PUBLIC CODER)

include_directories(src/decoder)

set(DECODER_SOURCES src/decoder/decoder.cpp)

add_library(DECODER ${DECODER_SOURCES})
target_include_directories(DECODER PUBLIC DECODER)

target_link_libraries(DECODER PUBLIC CODER)

add_executable(infocom main.cpp)
target_link_libraries(infocom PUBLIC CODER DECODER)
cmake_minimum_required(VERSION 3.30)
project(infocom)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/coder
                    src/decoder)

add_library(coder
        src/coder/coder.h
        src/coder/coder.cpp
)

# Add the decoder library
add_library(decoder
        src/decoder/decoder.h
        src/decoder/decoder.cpp
)


target_link_libraries(coder PUBLIC decoder)

add_executable(infocom main.cpp)
target_link_libraries(infocom PUBLIC decoder coder)
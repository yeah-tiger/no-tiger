cmake_minimum_required(VERSION 3.1)
project(no-tiger)

set(CMAKE_CXX_STANDARD 14)

include_directories(cxxopts/include src/ ${CMAKE_BINARY_DIR})

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(ntc
    ${SOURCE_FILES}
)
cmake_minimum_required(VERSION 3.1)
project(no-tiger)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    FIND_PACKAGE(BISON)
    FIND_PACKAGE(FLEX)
    include_directories(/usr/local/include)
endif()


include_directories(cxxopts/include src/ ${CMAKE_BINARY_DIR})

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(ntc
    ${CMAKE_BINARY_DIR}/parser.cpp
    ${CMAKE_BINARY_DIR}/scanner.cpp
    ${SOURCE_FILES}
)
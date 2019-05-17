cmake_minimum_required(VERSION 3.1)
project(no-tiger)

set(CMAKE_CXX_STANDARD 11)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(include/ src/ ${CMAKE_BINARY_DIR})
BISON_TARGET(Parser src/parser.y ${CMAKE_BINARY_DIR}/parser.cpp
            DEFINES_FILE ${CMAKE_BINARY_DIR}/parser.hpp)
FLEX_TARGET(Scanner src/scanner.l ${CMAKE_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser) 

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(ntc
   ${BISON_Parser_OUTPUTS}
   ${FLEX_Scanner_OUTPUTS}
    ${SOURCE_FILES}
)


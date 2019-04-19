cmake_minimum_required(VERSION 3.7)
project(no-tiger)

set(CMAKE_CXX_STANDARD 14)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parse src/parse.y ${CMAKE_BINARY_DIR}/y.tab.cpp
             DEFINES_FILE ${CMAKE_BINARY_DIR}/y.tab.h)
FLEX_TARGET(Scan src/scan.l ${CMAKE_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scan Parse)

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(ntcc
    ${BISON_Parse_OUTPUTS}
    ${FLEX_Scan_OUTPUTS}
    ${SOURCE_FILES}
)


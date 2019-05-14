cmake_minimum_required(VERSION 3.7)
project(no-tiger)

set(CMAKE_CXX_STANDARD 11)

find_package(BISON)
find_package(FLEX)

include_directories(include/)

#BISON_TARGET(Parse src/parse.y ${CMAKE_BINARY_DIR}/y.tab.cpp
#             DEFINES_FILE ${CMAKE_BINARY_DIR}/y.tab.h)
#FLEX_TARGET(Scan src/lex.l ${CMAKE_BINARY_DIR}/lex.yy.cpp)
#ADD_FLEX_BISON_DEPENDENCY(Scan Parse)

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(ntc
   # ${BISON_Parse_OUTPUTS}
   # ${FLEX_Scan_OUTPUTS}
    ${SOURCE_FILES}
)


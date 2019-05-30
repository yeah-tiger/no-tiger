cmake_minimum_required(VERSION 3.1)
project(no-tiger)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    FIND_PACKAGE(BISON)
    FIND_PACKAGE(FLEX 2.5.37 EXACT)
    include_directories(/usr/local/include)
endif()

find_package(LLVM REQUIRED CONFIG)
if(NOT LLVM_FOUND OR LLVM_VERSION VERSION_LESS 7.0)
    message(SEND_ERROR "LLVM >= 7.0 is required to build Glow")
endif()
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(cxxopts/include src/ ${CMAKE_BINARY_DIR} ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

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

#llvm_map_components_to_libnames(LLVM_LIBS core)
target_link_libraries(ntc LLVM)
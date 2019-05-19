cmake_minimum_required(VERSION 3.1)
project(no-tiger)

set(CMAKE_CXX_STANDARD 14)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(cxxopts/include src/ ${CMAKE_BINARY_DIR})


# set(BISON_OUTPUT ${CMAKE_BINARY_DIR}/parser.cpp)
# set(BISON_DEFINES ${CMAKE_BINARY_DIR}/parser.hpp)
# if (BISON_FOUND)
#     add_custom_command(
#         OUTPUT ${BISON_OUTPUT}
#         COMMAND ${BISON_EXECUTABLE}
#         --output=${BISON_OUTPUT}
#         --defines=${BISON_DEFINES}
#         ${CMAKE_SOURCE_DIR}/src/parser.y
#         COMMAND sed -i "'s/return \*new (yyas_<T> ()) T (t)/return \*new (yyas_<T> ()) T (std\:\:move((T\&)t))/'" "${CMAKE_BINARY_DIR}/parser.hpp"
#         COMMENT "generate bison parser"
#     )
# endif ()

# set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/scanner.cpp)
# if (FLEX_FOUND)
#     add_custom_command(
#         OUTPUT ${FLEX_OUTPUT}
#         COMMAND ${FLEX_EXECUTABLE}
#         --outfile=${FLEX_OUTPUT}
#         ${CMAKE_SOURCE_DIR}/src/scanner.l
#         COMMENT "generate flex scanner"
#     )
# endif ()

BISON_TARGET(Parser src/parser.y ${CMAKE_BINARY_DIR}/parser.cpp
           DEFINES_FILE ${CMAKE_BINARY_DIR}/parser.hpp)
FLEX_TARGET(Scanner src/scanner.l ${CMAKE_BINARY_DIR}/scanner.cpp)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

# add_custom_command(OUTPUT ${LIBFOO_TAR_HEADERS}
#   COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_CURRENT_SOURCE_DIR}/libfoo/foo.tar"
#   COMMAND ${CMAKE_COMMAND} -E touch ${LIBFOO_TAR_HEADERS}
#   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/foo"
#   DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/libfoo/foo.tar"
#   COMMENT "Unpacking foo.tar"
#   VERBATIM
# )

# add_custom_target(replace DEPENDS ${BISON_Parser_OUTPUT})


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
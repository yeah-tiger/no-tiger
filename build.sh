#/usr/bin/sh
mkdir build
cd build
bison ../src/parser.y --output=parser.cpp --defines=parser.hpp
flex -o scanner.cpp ../src/scanner.l
cmake ..
make
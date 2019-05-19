#/usr/bin/sh
mkdir build
cd build
bison ../src/parser.y --output=parser.cpp --defines=parser.hpp
flex -o scanner.cpp ../src/scanner.l
sed -i 's/return \*new (yyas_<T> ()) T (t)/return \*new (yyas_<T> ()) T (std\:\:move((T\&)t))/' parser.hpp
cmake ..
make
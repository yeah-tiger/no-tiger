#/usr/bin/sh
mkdir build
cd build
bison ../src/parser.y --output=parser.cpp --defines=parser.hpp
flex -o scanner.cpp ../src/scanner.l
# this command is intended to add std::move to generated parser
sed -i 's/return \*new (yyas_<T> ()) T (t)/return \*new (yyas_<T> ()) T (std\:\:move((T\&)t))/' parser.hpp
# for macOS:
# sed -i "" 's/return \*new (yyas_<T> ()) T (t)/return \*new (yyas_<T> ()) T (std\:\:move((T\&)t))/' parser.hpp
cmake -DLLVM_DIR=/usr/lib/llvm-7/cmake ..
make
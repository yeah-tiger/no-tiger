#/usr/bin/sh
mkdir build
cd build
cmake3 ..
make
sed -i 's/return \*new (yyas_<T> ()) T (t)/return \*new (yyas_<T> ()) T (std\:\:move((T\&)t))/' parser.hpp
make
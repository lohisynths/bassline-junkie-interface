mkdir BUILD
rm -rf BUILD/*
cd BUILD
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ../ -DCMAKE_TOOLCHAIN_FILE=../cmake/nucleo_l432kc_gcc.cmake -DMBED_DIR=/home/alax/git/nucleo_mbed/mbed-os/BUILD/mbed -DCMAKE_BUILD_TYPE=Debug
make -j8
cd ../


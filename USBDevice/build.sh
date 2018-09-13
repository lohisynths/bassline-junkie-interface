mkdir build
cd build
rm -rf *
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../../nucleo-f411re/cmake/nucleo_l432kc_gcc.cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DMBED_LOCATION=/home/alax/git/nucleo_mbed/mbed-os/BUILD/mbed
make


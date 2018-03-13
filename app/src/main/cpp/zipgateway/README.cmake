For debian:
------------

1. $ mkdir build
2. $ cd build/
3. $ cmake ..
4. $ make
5. $ make package

For Beaglebone:
---------------

1. $ mkdir buildbb/
2. $ cd buildbb/
3. $ source the ti beaglebone sdk environment setup
    a. $ cd <path>/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/
    b. $ source environment-setup
4. $  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/BeagleBoneToolchain.cmake ..
5. export ZWLIBROOT=<sdk> (for e.g. export ZWLIBROOT=/mnt/extra/sdk670/Z-Wave/)
6. $ make
7. $ make package

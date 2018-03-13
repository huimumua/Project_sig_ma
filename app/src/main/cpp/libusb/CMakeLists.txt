project(libusb)

set(CMAKE_C_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "-fPIC")

set(src libusb/core.c
        libusb/descriptor.c
        libusb/io.c
        libusb/sync.c
        libusb/os/linux_usbfs.c)

INCLUDE_DIRECTORIES(. ./libusb ./libusb/os)

add_library(usb SHARED ${src})
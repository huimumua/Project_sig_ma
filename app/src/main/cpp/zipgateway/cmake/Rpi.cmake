# this one is important
SET (CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR armhf) 
SET(CONFIGURE_HOST arm-linux)
# this one not so much
SET (CMAKE_SYSTEM_VERSION 1)

# search for programs in the build host directories
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (OPENSSL_INCLUDE_DIRS "${OPENSSL_SRC}/include/")
set (CMAKE_EXE_LINKER_FLAGS "-L${OPENSSL_SRC}/build/lib/")
set (LibUSB_LIBRARIES ${LIBUSB_SRC}/libusb/.libs/libusb-1.0.so)
set (LibUSB_INCLUDE_DIRS ${LIBUSB_SRC}/libusb/)
set (OPENSSL_LIBRARIES "-L${OPENSSL_SRC}/build/lib/ -lssl -lcrypto")

message(STATUS "openssl_include: ${OPENSSL_INCLUDE_DIRS}")
message(STATUS "libusb_include: ${LibUSB_INCLUDE_DIRS}")
message(STATUS "libpcap_include: ${LIBPCAP_SRC}")

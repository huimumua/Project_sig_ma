# Add gcov support
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")

# flex does not like unused and sign-compare
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused -Wno-sign-compare")

set(PVL_SRC
  ${CMAKE_SOURCE_DIR}/src/pvs_parse_help.c
  ${CMAKE_SOURCE_DIR}/src/provisioning_list.c
  ${CMAKE_SOURCE_DIR}/src/RD_internal.c
  ${CMAKE_SOURCE_DIR}/src/RD_DataStore.c
  ${CMAKE_SOURCE_DIR}/src/smalloc.c
  ${CMAKE_SOURCE_DIR}/contiki/core/lib/assert.c
  ${CMAKE_SOURCE_DIR}/contiki/core/lib/list.c
  ${CMAKE_SOURCE_DIR}/test/eeprom-stub.c
)

#add_definitions(-DUIP_CONF_IPV6=1)
add_definitions(-DUIP_CONF_IPV6=1 -DAUTOSTART_ENABLE -DUIP_CONF_ROUTER=1)

include_directories(
  ${CMAKE_SOURCE_DIR}/src/
  ${CMAKE_SOURCE_DIR}/src/serialapi
  ${CMAKE_SOURCE_DIR}/src/transport
  ${CMAKE_SOURCE_DIR}/contiki/platform/linux/
  ${CMAKE_SOURCE_DIR}/contiki/core
  ${CMAKE_SOURCE_DIR}/contiki/core/sys
  ${CMAKE_SOURCE_DIR}/contiki/core/lib # list.h
  ${CMAKE_SOURCE_DIR}/contiki/core/net # uip.h
  ${CMAKE_SOURCE_DIR}/contiki/cpu/native/
  ${CMAKE_SOURCE_DIR}/Z-Wave/include
  ${CMAKE_SOURCE_DIR}/test
)

add_subdirectory(pvs)

add_subdirectory(CC_provisioning_list)

add_subdirectory(ZIP_router)

add_subdirectory( zwlog )

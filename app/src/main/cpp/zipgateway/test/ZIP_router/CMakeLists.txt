
include_directories(${CMAKE_SOURCE_DIR}/contiki/core/sys  ${CMAKE_SOURCE_DIR}/Z-Wave/include
${CMAKE_SOURCE_DIR}/contiki/cpu/native ${CMAKE_SOURCE_DIR}/src/transport  ${CMAKE_SOURCE_DIR}/test
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/src/serialapi
${CMAKE_SOURCE_DIR}/contiki/core/net
${CMAKE_SOURCE_DIR}/contiki/core/lib
${CMAKE_SOURCE_DIR}/libs2/transport_service
${CMAKE_SOURCE_DIR}/libs2/include
)

add_definitions(-DUIP_CONF_ROUTER=1)

# Sadly, we have to disable these compiler checks to build the contiki code.
# Someday we must refactor ZIP_router (and all other modules) so they can also be built without
# the contiki code. 
add_definitions(-Wno-shadow -Wno-unused-variable -Wno-missing-field-initializers -Wno-unused-parameter)

# This ignores any unimplemented functions in the linking phase so we can build and test small parts 
# of ZIP_router module without stubbing out all the unused external functions.
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--unresolved-symbols=ignore-all" )

add_executable(test_ZIP_router test_ZIP_router.c ../test_helpers.c )
add_test(ZIP_router test_ZIP_router)
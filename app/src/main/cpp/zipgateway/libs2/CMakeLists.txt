
# Libs2 is designed to be used included as a subproject, but this part allows to build libs2
# standalone, as is done for C51 in SDK670.
IF ("${PROJECT_NAME}" STREQUAL "")
  cmake_minimum_required (VERSION 2.8)
  project (libs2 C ASM)

  set(CMAKE_BUILD_TYPE debug)
  IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "C51")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -WL 1 --listfile")
    add_definitions( -DZW050x -DSINGLE_CONTEXT -DNDEBUG )
    set(DISABLE_MOCK 1)
    #add_definitions( -DZW_DEBUG )
  else()
    add_definitions( -Dx86 )
  endif()
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang" )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
  endif()
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-self-assign -Wno-error=unused-command-line-argument")
  endif()

  find_path(ZWAVE_INCLUDE_DIR ZW_classcmd.h
            HINTS ENV ZWLIBROOT ${PROJECT_SOURCE_DIR}/..
            PATH_SUFFIXES include )
ENDIF ("${PROJECT_NAME}" STREQUAL "")

include(CMakeFunctions.cmake)

# C51 libraries are placed in lib folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
add_definitions(-DDllExport=__declspec\(dllexport\) )
else()
add_definitions( -DDllExport=extern  )
endif()

CROSS_SYSTEM_DIR("include" "" CROSS_SYSTEM_INCLUDE)
# Setting -DOPTIONAL_SYSTEM_INCLUDE=<optional_include_path> allows to specify addional include path(s) when building libs2.
include_directories( include ${ZWAVE_INCLUDE_DIR} ${CROSS_SYSTEM_INCLUDE} ${OPTIONAL_SYSTEM_INCLUDE})

enable_testing()

add_subdirectory(TestFramework)
#add_subdirectory(test)
add_subdirectory(crypto)
add_subdirectory(protocol)
add_subdirectory(inclusion)
if (NOT DEFINED DISABLE_MOCK)
  add_subdirectory(include/mock)
  add_subdirectory(inclusion/mock)
  add_subdirectory(crypto/mock)
endif()

# Only add transport service to libs2 for c51 build. It is no longer cross-platform.
# TODO: Move transport service 2 out of libs2.
if(${CMAKE_SYSTEM_NAME} MATCHES "C51")
  add_library(s2_slave protocol/S2.c inclusion/s2_inclusion.c  transport_service/transport_service2.c transport_service/transport2_fsm.c)
else()
  add_library(s2_slave protocol/S2.c inclusion/s2_inclusion.c)
endif()
target_link_libraries(s2_slave s2crypto aes)
target_include_directories(s2_slave PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(s2_controller protocol/S2.c inclusion/s2_inclusion.c)
set_target_properties(s2_controller PROPERTIES COMPILE_DEFINITIONS "ZW_CONTROLLER")
target_link_libraries(s2_controller s2crypto aes)
target_include_directories(s2_controller PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )


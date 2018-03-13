

# This function defines a unity test. 
# The function takes the following arguments:
# NAME  <name>                      : Name of test executeable to build
# TEST_BASE <file.<c|cpp>>          : Unit test file from which the runner should be generated, if TEST_BASE is not provided, the <name>.<c|cpp> from first parameter will be used.
# FILES <file1> <file2> ... <fileN> : List of files used for building the test
# LIBRARIES <lib1> <lib2> ... <libN>: List of libraries to link for the test executeable (unity is automatically included)
# USE_CPP                           : Set this flag if the test executable must be compiled using C++
# example: add_unity_test(NAME TestFileSystem FILES ../FileSystem.c TestFileSystem.cpp LIBRARIES Assert USE_CPP)
if (NOT COMMAND ADD_UNITY_TEST)
function(ADD_UNITY_TEST)
  set(OPTIONS "USE_CPP")
  set(SINGLE_VALUE_ARGS "NAME" "TEST_BASE")
  set(MULTI_VALUE_ARGS "FILES" "LIBRARIES")
  cmake_parse_arguments(ADD_UNITY_TEST "${OPTIONS}" "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

  set(RUNNER_EXTENSION c)
  if (ADD_UNITY_TEST_USE_CPP)
    set(RUNNER_EXTENSION cpp)
  endif (ADD_UNITY_TEST_USE_CPP)

  set(RUNNER_BASE "${ADD_UNITY_TEST_NAME}.${RUNNER_EXTENSION}")
  if (NOT ${ADD_UNITY_TEST_TEST_BASE} STREQUAL "")
    set(RUNNER_BASE "${ADD_UNITY_TEST_TEST_BASE}")
  endif (NOT ${ADD_UNITY_TEST_TEST_BASE} STREQUAL "")

  if ("${ADD_UNITY_TEST_NAME}" STREQUAL "")
    list(GET ADD_UNITY_TEST_UNPARSED_ARGUMENTS 0 ADD_UNITY_TEST_NAME)
    list(REMOVE_AT ADD_UNITY_TEST_UNPARSED_ARGUMENTS 0})
    set(RUNNER_BASE "${ADD_UNITY_TEST_NAME}.${RUNNER_EXTENSION}")
    set(ADD_UNITY_TEST_FILES "${ADD_UNITY_TEST_UNPARSED_ARGUMENTS}")
  endif ("${ADD_UNITY_TEST_NAME}" STREQUAL "")

  add_executable(${ADD_UNITY_TEST_NAME} ${ADD_UNITY_TEST_NAME}_runner.${RUNNER_EXTENSION} ${ADD_UNITY_TEST_FILES})

  add_custom_command(OUTPUT ${ADD_UNITY_TEST_NAME}_runner.${RUNNER_EXTENSION}
       COMMAND ${PYTHON_EXECUTABLE} ${TEST_TOOLS_DIR}/gen_test_runner.py  ${RUNNER_BASE} > ${CMAKE_CURRENT_BINARY_DIR}/${ADD_UNITY_TEST_NAME}_runner.${RUNNER_EXTENSION}
       DEPENDS ${RUNNER_BASE} ${TEST_TOOLS_DIR}/gen_test_runner.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  add_test(${ADD_UNITY_TEST_NAME} ${ADD_UNITY_TEST_NAME} )
  target_link_libraries( ${ADD_UNITY_TEST_NAME} unity ${ADD_UNITY_TEST_LIBRARIES})
  install(TARGETS ${ADD_UNITY_TEST_NAME} DESTINATION test)
endfunction(ADD_UNITY_TEST)
endif (NOT COMMAND ADD_UNITY_TEST)


# This function calls a python script in order to generate the unity runner for the test.
# The name of the test suite is mandatory and the runner will be named: ${TEST}_runner.c
# The function also takes the following optional arguments:
# Optional arg 0: Extension of runner, e.g. .cpp instead of .c
# Optional argument can be used as: unity_test( my_unity_test cpp )
# Optional arg 1: Name of source without extension which is hardcoded to be ".c". This can be used
# if several targets with different defines, etc. based on the same source file are desired.
if (NOT COMMAND unity_test)
function(unity_test TEST )
  message(WARNING "Use of unity_test function is depricated please use add_unity_test(..)")
  set(RUNNER_EXTENSION c)
  set(SOURCE_NAME ${TEST})
  set(OPTIONAL_ARGS ${ARGN})
  list(LENGTH OPTIONAL_ARGS OPTIONAL_ARGS_LENGTH)
  if (OPTIONAL_ARGS_LENGTH GREATER 0)
    list (GET OPTIONAL_ARGS 0 RUNNER_EXTENSION)
  endif(OPTIONAL_ARGS_LENGTH GREATER 0)
  if (OPTIONAL_ARGS_LENGTH GREATER 1)
    list (GET OPTIONAL_ARGS 1 SOURCE_NAME)
  endif (OPTIONAL_ARGS_LENGTH GREATER 1)
  #message( "RUNNER_EXTENSION = ${RUNNER_EXTENSION}")
  #message( "SOURCE_NAME = ${SOURCE_NAME}")

  add_custom_command(OUTPUT ${TEST}_runner.${RUNNER_EXTENSION}
       COMMAND ${PYTHON_EXECUTABLE} ${TEST_TOOLS_DIR}/gen_test_runner.py  ${SOURCE_NAME}.${RUNNER_EXTENSION} > ${CMAKE_CURRENT_BINARY_DIR}/${TEST}_runner.${RUNNER_EXTENSION}
       DEPENDS ${SOURCE_NAME}.${RUNNER_EXTENSION} ${TEST_TOOLS_DIR}/gen_test_runner.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  add_test(${TEST} ${TEST})
  install(TARGETS ${TEST} DESTINATION test)
endfunction(unity_test)
endif (NOT COMMAND unity_test)

###############################################################################
# CROSS_SYSTEM_DIR(IN_SEARCH_PATH IN_MATCH_PATTERN OUT_CROSS_SYSTEM_DIR)
#
# Function for searching and returning the correct folder during cross compilation
# Folders are returned according to first match:
# If IN_MATCH_PATTERN is empty string, i.e. "", then the first matched cross folder is returned.
# 1) ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}: If two level folder with same name exists.
# 2) ${CMAKE_SYSTEM_NAME}/generic: If ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR} does not exists but folder ${CMAKE_SYSTEM_NAME}/${IN_MATCH_PATTERN}/generic exists.
# 3) ${CMAKE_SYSTEM_NAME}/: If folder with same name exists.
# 4) ${CMAKE_SYSTEM_PROCESSOR}: If folder with same name exists.
# 5) generic: If ${CMAKE_SYSTEM_PROCESSOR} does not exists but folder 'generic' exists.
# 6) ${IN_MATCH_PATTERN}: The folder itself is returned
# 7) '': If no matches are found
# For all tests above, it applies that if ${IN_MATCH_PATTERN} is set then the folder is only returned if a file/folder matching the pattern is found.
# e.g. if ${IN_MATCH_PATTERN} is set to 'include' and ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR} does not contain an 'include' file/folder then the tests continues to 2)
# @param[in]  IN_SEARCH_PATH Name of path/folder that should be seached
# @param[in]  IN_MATCH_PATTERN  Name of subfolder/file in search path that should be found, set to "" / or empty variable if no match folder is needed
# @param[out] OUT_CROSS_SYSTEM_DIR Name of path/folder which matched above rule
###############################################################################
IF (NOT COMMAND CROSS_SYSTEM_DIR)
  FUNCTION(CROSS_SYSTEM_DIR IN_SEARCH_PATH IN_MATCH_PATTERN OUT_CROSS_SYSTEM_DIR)
    IF(NOT (IS_ABSOLUTE ${IN_SEARCH_PATH}))
      SET(IN_SEARCH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${IN_SEARCH_PATH}")
    ENDIF(NOT (IS_ABSOLUTE ${IN_SEARCH_PATH}))

    IF((CMAKE_SYSTEM_NAME) AND (CMAKE_SYSTEM_PROCESSOR) AND (EXISTS ${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}/${IN_MATCH_PATTERN}))
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}" PARENT_SCOPE)
    ELSEIF((CMAKE_SYSTEM_NAME) AND (EXISTS ${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}/generic/${IN_MATCH_PATTERN}))
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}/generic" PARENT_SCOPE)
    ELSEIF((CMAKE_SYSTEM_NAME) AND (NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Generic")) AND (EXISTS ${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}/${IN_MATCH_PATTERN}))
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}/${CMAKE_SYSTEM_NAME}" PARENT_SCOPE)
    ELSEIF((CMAKE_SYSTEM_PROCESSOR) AND (EXISTS ${IN_SEARCH_PATH}/${CMAKE_SYSTEM_PROCESSOR}/${IN_MATCH_PATTERN}))
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}/${CMAKE_SYSTEM_PROCESSOR}" PARENT_SCOPE)
    ELSEIF(EXISTS ${IN_SEARCH_PATH}/generic/${IN_MATCH_PATTERN})
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}/generic" PARENT_SCOPE)
    ELSEIF(EXISTS ${IN_SEARCH_PATH}/${IN_MATCH_PATTERN})
      SET(${OUT_CROSS_SYSTEM_DIR} "${IN_SEARCH_PATH}" PARENT_SCOPE)
    ELSE()
      IF(${IN_MATCH_PATTERN} STREQUAL "")
        MESSAGE(WARNING "CROSS_SYSTEM_DIR failed to find cross platform implementation, returning empty string.\n Fix this warning by creating a cross platform folder for ${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR} in ${IN_SEARCH_PATH}.")
      ELSE(${IN_MATCH_PATTERN} STREQUAL "")
        MESSAGE(WARNING "CROSS_SYSTEM_DIR failed to find cross platform implementation, returning empty string.\n Fix this warning by creating a cross platform folder for ${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR} in ${IN_SEARCH_PATH} containing ${IN_MATCH_PATTERN}")
      ENDIF(${IN_MATCH_PATTERN} STREQUAL "")
      SET(${OUT_CROSS_SYSTEM_DIR} "" PARENT_SCOPE)
    ENDIF()
  ENDFUNCTION(CROSS_SYSTEM_DIR IN_SEARCH_PATH IN_MATCH_PATTERN OUT_CROSS_SYSTEM_DIR)
ENDIF (NOT COMMAND CROSS_SYSTEM_DIR)

IF (NOT COMMAND ADD_CROSS_SUBDIRECTORY)
  FUNCTION(add_cross_subdirectory IN_SUBDIRECTORY)
  CROSS_SYSTEM_DIR("${IN_SUBDIRECTORY}" "CMakeLists.txt" CROSS_SUBDIRECTORY)
  add_subdirectory(${CROSS_SUBDIRECTORY})
  ENDFUNCTION(add_cross_subdirectory IN_SUBDIRECTORY)
ENDIF (NOT COMMAND ADD_CROSS_SUBDIRECTORY)

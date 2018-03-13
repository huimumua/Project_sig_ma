add_definitions(-DUIP_CONF_IPV6=1 -DPVS_TEST -DPROVISIONING_LIST_SIZE=232)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wparentheses -Wno-array-bounds -Wall -Wno-unused -Wextra -Wno-sign-compare -Wcomment -Wno-format-extra-args -Wmaybe-uninitialized -Wno-shadow -Wundef -ggdb")

find_package(BISON 3.0.4)
find_package(FLEX)

if (BISON_FOUND)
  include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/contiki/core/lib/
    )
  #BISON_TARGET(testparser ${CMAKE_SOURCE_DIR}/src/pvs_cfg.y ${CMAKE_SOURCE_DIR}/src-gen/pvs_cfg.tab.c VERBOSE)
  BISON_TARGET(testparser ${CMAKE_SOURCE_DIR}/src/pvs_cfg.y ${CMAKE_CURRENT_BINARY_DIR}/pvs_cfg.tab.c VERBOSE)

  if (FLEX_FOUND)
    #FLEX_TARGET(testlexer ${CMAKE_SOURCE_DIR}/src/pvs_cfg.l ${CMAKE_SOURCE_DIR}/src-gen/lex.yy.c )
    FLEX_TARGET(testlexer ${CMAKE_SOURCE_DIR}/src/pvs_cfg.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c )
    ADD_FLEX_BISON_DEPENDENCY(testlexer testparser)
  endif(FLEX_FOUND)

  set(PVL_SRC
    ${PVL_SRC}
    ${CMAKE_SOURCE_DIR}/contiki/platform/linux/zgw_log_int.c
    ${CMAKE_SOURCE_DIR}/contiki/core/lib/assert.c
    ${BISON_testparser_OUTPUTS} 
    ${FLEX_testlexer_OUTPUTS}
    )
    
endif(BISON_FOUND)

if (BISON_FOUND)
  add_executable(test_provisioning_list test_provisioning_list.c
    pvs_cfg_test_help.c
    ${CMAKE_SOURCE_DIR}/test/test_helpers.c
    ${PVL_SRC} 
    )
  target_compile_definitions(test_provisioning_list PUBLIC
    PROVISIONING_LIST_STORE_FILENAME_DEFAULT="provisioning_list_store.dat"
    )
  #PROVISIONING_CONFIG_FILENAME_DEFAULT

  add_executable(test_pvs_dev_lookups test_pvs_dev_lookups.c
#    pvs_cfg_test_help.c
    ${CMAKE_SOURCE_DIR}/test/test_helpers.c
    ${PVL_SRC} 
    )
  target_compile_definitions(test_pvs_dev_lookups PUBLIC
    PROVISIONING_LIST_STORE_FILENAME_DEFAULT="provisioning_list_store.dat"
    )

  add_executable(test_pvs_cfg_reader test_pvs_cfg_reader.c
    pvs_cfg_test_help.c
    ${CMAKE_SOURCE_DIR}/test/test_helpers.c
    ${PVL_SRC}
    )
  target_compile_definitions(test_pvs_cfg_reader PUBLIC
    PROVISIONING_LIST_STORE_FILENAME_DEFAULT="provisioning_list_store.dat"
    )

  add_executable(test_pvs_cfg_errors test_pvs_cfg_errors.c
    pvs_cfg_test_help.c
    ${CMAKE_SOURCE_DIR}/test/test_helpers.c
    ${PVL_SRC}
    )
  target_compile_definitions(test_pvs_cfg_errors PUBLIC
    PROVISIONING_LIST_STORE_FILENAME_DEFAULT="provisioning_list_store.dat"
    )

  add_executable(test_pvs_persistence test_pvs_persistence.c
    pvs_cfg_test_help.c
    ${CMAKE_SOURCE_DIR}/test/test_helpers.c
    ${PVL_SRC}
    )
  target_compile_definitions(test_pvs_persistence PUBLIC
    PROVISIONING_LIST_STORE_FILENAME_DEFAULT="provisioning_list_store.dat"
    ZGW_LOG ZGW_LOG_LVL_INIT=5)
endif(BISON_FOUND)

add_custom_target(pvs_gcov
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles
  COMMAND gcov test_pvs_persistence.dir/__/src/provisioning_list.c test_provisioning_list.dir/__/src/provisioning_list.c
  DEPENDS test_pvs_persistence test_pvs_cfg_reader test_provisioning_list
  )

add_test(pvs_cfg_reader test_pvs_cfg_reader)
add_test(pvs_cfg_errors test_pvs_cfg_errors)
add_test(pvs_persistence test_pvs_persistence)
add_test(provisioning_list test_provisioning_list)
add_test(pvs_dev_lookups test_pvs_dev_lookups)

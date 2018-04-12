project(zwcontrol C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wparentheses -Wno-array-bounds -fPIE -DOS_ANDROID -DOS_LINUX -DZIP_V2 -DUSE_OPENSSL -fPIC")
set(CMAKE_LD_FLAGS "-fPIE -fPIC")

set(OPENSSL_LIBRARIES "zssl")
set(LIBCRYPTO_LIBRARIES "zcrypto")

set(OPENSSL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../openssl-1.0.2l/include")

include_directories(
    ${OPENSSL_INCLUDE_DIRS}
    ./zwapi/include
    ./zwapi/zwave/include
    .)

set(ZWAPI_SRC
    zwapi/src/cjson.c
    zwapi/src/zip_api_base.c
    zwapi/src/zip_api_intf.c
    zwapi/src/zip_api_network.c
    zwapi/src/zip_api_sm.c
    zwapi/src/zip_api_util.c
    zwapi/src/zip_dev_cfg.c
    zwapi/src/zip_dev_poll.c
    zwapi/src/zip_event_gen.c
    zwapi/src/zip_network_save.c
    zwapi/src/zip_node_sts.c
    zwapi/src/zip_poll.c
    zwapi/src/zip_set_poll.c
    zwapi/src/zip_supervision.c
    zwapi/src/zip_wkup_poll.c
    zwapi/src/zip_intf_sm.c
    zwapi/src/zip_ima.c
    zwapi/lib/zip_application.c
    zwapi/lib/zip_dtls.c
    zwapi/lib/zip_frame.c
    zwapi/lib/zip_mdns.c
    zwapi/lib/zip_netutil.c
    zwapi/lib/zip_platform.c
    zwapi/lib/zip_session.c
    zwapi/lib/zip_tls_server.c
    zwapi/lib/zip_transport.c
    zwapi/lib/zip_util.c
    )

set(SRC
    ${ZWAPI_SRC}
    zwcontrol_api.c)

add_library(zwcontrol SHARED ${SRC})

find_library(log-lib log)

target_link_libraries(zwcontrol ${OPENSSL_LIBRARIES} ${LIBCRYPTO_LIBRARIES} zipgateway ${log-lib})